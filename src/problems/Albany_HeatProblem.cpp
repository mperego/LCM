// Albany 3.0: Copyright 2016 National Technology & Engineering Solutions of
// Sandia, LLC (NTESS). This Software is released under the BSD license detailed
// in the file license.txt in the top-level Albany directory.

#include "Albany_HeatProblem.hpp"

#include "Albany_BCUtils.hpp"
#include "Albany_Utils.hpp"
#include "Intrepid2_DefaultCubatureFactory.hpp"
#include "PHAL_FactoryTraits.hpp"
#include "Shards_CellTopology.hpp"

Albany::HeatProblem::HeatProblem(
    const Teuchos::RCP<Teuchos::ParameterList>& params_,
    const Teuchos::RCP<ParamLib>&               paramLib_,
    // const Teuchos::RCP<DistributedParameterLibrary>& distParamLib_,
    int const                               numDim_,
    Teuchos::RCP<Teuchos::Comm<int> const>& commT_)
    : Albany::AbstractProblem(params_, paramLib_ /*, distParamLib_*/),
      params(params_),
      haveSource(false),
      haveAbsorption(false),
      numDim(numDim_),
      commT(commT_),
      use_sdbcs_(false)
{
  this->setNumEquations(1);

  if (numDim == 1)
    periodic = params->get("Periodic BC", false);
  else
    periodic = false;
  if (periodic) *out << " Periodic Boundary Conditions being used." << std::endl;

  haveAbsorption = params->isSublist("Absorption");

  if (params->isType<std::string>("MaterialDB Filename")) {
    std::string mtrlDbFilename = params->get<std::string>("MaterialDB Filename");
    // Create Material Database
    materialDB = Teuchos::rcp(new Albany::MaterialDatabase(mtrlDbFilename, commT));
  }

  conductivityIsDistParam = false;
  if (params->isSublist("Distributed Parameters")) {
    int numDistParams = params->sublist("Distributed Parameters").get<int>("Number of Parameter Vectors", 0);
    for (int i = 0; i < numDistParams; ++i) {
      Teuchos::ParameterList p =
          params->sublist("Distributed Parameters").sublist(Albany::strint("Distributed Parameter", i));
      if (p.get<std::string>("Name", "") == "thermal_conductivity") conductivityIsDistParam = true;
      break;
    }
  }
  dirichletIsDistParam = false;
  if (params->isSublist("Distributed Parameters")) {
    int numDistParams = params->sublist("Distributed Parameters").get<int>("Number of Parameter Vectors", 0);
    for (int i = 0; i < numDistParams; ++i) {
      Teuchos::ParameterList p =
          params->sublist("Distributed Parameters").sublist(Albany::strint("Distributed Parameter", i));
      if (p.get<std::string>("Name", "") == "dirichlet_field") dirichletIsDistParam = true;
      meshPartDirichlet = p.get<std::string>("Mesh Part", "");
      break;
    }
  }
}

Albany::HeatProblem::~HeatProblem() {}

void
Albany::HeatProblem::buildProblem(
    Teuchos::ArrayRCP<Teuchos::RCP<Albany::MeshSpecsStruct>> meshSpecs,
    Albany::StateManager&                                    stateMgr)
{
  /* Construct All Phalanx Evaluators */
  int physSets = meshSpecs.size();
  std::cout << "Heat Problem Num MeshSpecs: " << physSets << std::endl;
  fm.resize(physSets);

  for (int ps = 0; ps < physSets; ps++) {
    fm[ps] = Teuchos::rcp(new PHX::FieldManager<PHAL::AlbanyTraits>);
    buildEvaluators(*fm[ps], *meshSpecs[ps], stateMgr, BUILD_RESID_FM, Teuchos::null);
  }

  if (meshSpecs[0]->nsNames.size() > 0) {  // Build a nodeset evaluator if nodesets are present
    constructDirichletEvaluators(meshSpecs[0]->nsNames);
  }

  // Check if have Neumann sublist; throw error if attempting to specify
  // Neumann BCs, but there are no sidesets in the input mesh
  bool isNeumannPL = params->isSublist("Neumann BCs");
  if (isNeumannPL && !(meshSpecs[0]->ssNames.size() > 0)) {
    ALBANY_ABORT("You are attempting to set Neumann BCs on a mesh with no sidesets!");
  }

  if (meshSpecs[0]->ssNames.size() > 0) {  // Build a sideset evaluator if sidesets are present
    constructNeumannEvaluators(meshSpecs[0]);
  }
}

Teuchos::Array<Teuchos::RCP<const PHX::FieldTag>>
Albany::HeatProblem::buildEvaluators(
    PHX::FieldManager<PHAL::AlbanyTraits>&      fm0,
    Albany::MeshSpecsStruct const&              meshSpecs,
    Albany::StateManager&                       stateMgr,
    Albany::FieldManagerChoice                  fmchoice,
    const Teuchos::RCP<Teuchos::ParameterList>& responseList)
{
  // Call constructEvaluators<EvalT>(*rfm[0], *meshSpecs[0], stateMgr);
  // for each EvalT in PHAL::AlbanyTraits::BEvalTypes
  ConstructEvaluatorsOp<HeatProblem>                    op(*this, fm0, meshSpecs, stateMgr, fmchoice, responseList);
  Sacado::mpl::for_each<PHAL::AlbanyTraits::BEvalTypes> fe(op);
  return *op.tags;
}

// Dirichlet BCs
void
Albany::HeatProblem::constructDirichletEvaluators(std::vector<std::string> const& nodeSetIDs)
{
  // Construct BC evaluators for all node sets and names
  std::vector<std::string> bcNames(neq);
  bcNames[0] = "T";
  Albany::BCUtils<Albany::DirichletTraits> bcUtils;
  dfm         = bcUtils.constructBCEvaluators(nodeSetIDs, bcNames, this->params, this->paramLib);
  use_sdbcs_  = bcUtils.useSDBCs();
  offsets_    = bcUtils.getOffsets();
  nodeSetIDs_ = bcUtils.getNodeSetIDs();
}

// Neumann BCs
void
Albany::HeatProblem::constructNeumannEvaluators(const Teuchos::RCP<Albany::MeshSpecsStruct>& meshSpecs)
{
  // Note: we only enter this function if sidesets are defined in the mesh file
  // i.e. meshSpecs.ssNames.size() > 0

  Albany::BCUtils<Albany::NeumannTraits> bcUtils;

  // Check to make sure that Neumann BCs are given in the input file

  if (!bcUtils.haveBCSpecified(this->params)) return;

  // Construct BC evaluators for all side sets and names
  // Note that the string index sets up the equation offset, so ordering is
  // important
  std::vector<std::string>            bcNames(neq);
  Teuchos::ArrayRCP<std::string>      dof_names(neq);
  Teuchos::Array<Teuchos::Array<int>> offsets;
  offsets.resize(neq);

  bcNames[0]   = "T";
  dof_names[0] = "Temperature";
  offsets[0].resize(1);
  offsets[0][0] = 0;

  // Construct BC evaluators for all possible names of conditions
  // Should only specify flux vector components (dudx, dudy, dudz), or dudn, not
  // both
  std::vector<std::string> condNames(5);
  // dudx, dudy, dudz, dudn, scaled jump (internal surface), or robin (like DBC
  // plus scaled jump)

  // Note that sidesets are only supported for two and 3D currently
  if (numDim == 2)
    condNames[0] = "(dudx, dudy)";
  else if (numDim == 3)
    condNames[0] = "(dudx, dudy, dudz)";
  else
    ALBANY_ABORT(std::endl << "Error: Sidesets only supported in 2 and 3D." << std::endl);

  condNames[1] = "dudn";
  condNames[2] = "scaled jump";
  condNames[3] = "robin";
  condNames[4] = "radiate";

  nfm.resize(1);  // Heat problem only has one physics set
  nfm[0] = bcUtils.constructBCEvaluators(
      meshSpecs, bcNames, dof_names, false, 0, condNames, offsets, dl, this->params, this->paramLib, materialDB);
}

Teuchos::RCP<Teuchos::ParameterList const>
Albany::HeatProblem::getValidProblemParameters() const
{
  Teuchos::RCP<Teuchos::ParameterList> validPL = this->getGenericProblemParams("ValidHeatProblemParams");

  if (numDim == 1) validPL->set<bool>("Periodic BC", false, "Flag to indicate periodic BC for 1D problems");
  validPL->sublist("ThermalConductivity", false, "");
  validPL->set("Convection Velocity", "{0,0,0}", "");
  validPL->set<bool>("Have Rho Cp", false, "Flag to indicate if rhoCp is used");
  validPL->set<std::string>("MaterialDB Filename", "materials.xml", "Filename of material database xml file");

  return validPL;
}
