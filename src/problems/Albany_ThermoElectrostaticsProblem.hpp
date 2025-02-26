// Albany 3.0: Copyright 2016 National Technology & Engineering Solutions of
// Sandia, LLC (NTESS). This Software is released under the BSD license detailed
// in the file license.txt in the top-level Albany directory.

#ifndef ALBANY_THERMOELECTROSTATICSPROBLEM_HPP
#define ALBANY_THERMOELECTROSTATICSPROBLEM_HPP

#include "Albany_AbstractProblem.hpp"
#include "PHAL_Dimension.hpp"
#include "PHAL_Workset.hpp"
#include "Teuchos_ParameterList.hpp"
#include "Teuchos_RCP.hpp"

namespace Albany {

/*!
 * \brief Abstract interface for representing a 1-D finite element
 * problem.
 */
class ThermoElectrostaticsProblem : public AbstractProblem
{
 public:
  //! Default constructor
  ThermoElectrostaticsProblem(
      const Teuchos::RCP<Teuchos::ParameterList>& params,
      const Teuchos::RCP<ParamLib>&               paramLib,
      int const                                   numDim_);

  //! Return number of spatial dimensions
  virtual int
  spatialDimension() const
  {
    return numDim;
  }

  //! Get boolean telling code if SDBCs are utilized
  virtual bool
  useSDBCs() const
  {
    return use_sdbcs_;
  }

  //! Destructor
  ~ThermoElectrostaticsProblem();

  //! Build the PDE instantiations, boundary conditions, and initial solution
  virtual void
  buildProblem(Teuchos::ArrayRCP<Teuchos::RCP<Albany::MeshSpecsStruct>> meshSpecs, StateManager& stateMgr);

  // Build evaluators
  virtual Teuchos::Array<Teuchos::RCP<const PHX::FieldTag>>
  buildEvaluators(
      PHX::FieldManager<PHAL::AlbanyTraits>&      fm0,
      Albany::MeshSpecsStruct const&              meshSpecs,
      Albany::StateManager&                       stateMgr,
      Albany::FieldManagerChoice                  fmchoice,
      const Teuchos::RCP<Teuchos::ParameterList>& responseList);

  //! Each problem must generate it's list of valide parameters
  Teuchos::RCP<Teuchos::ParameterList const>
  getValidProblemParameters() const;

 private:
  //! Private to prohibit copying
  ThermoElectrostaticsProblem(const ThermoElectrostaticsProblem&);

  //! Private to prohibit copying
  ThermoElectrostaticsProblem&
  operator=(const ThermoElectrostaticsProblem&);

 public:
  //! Main problem setup routine. Not directly called, but indirectly by
  //! following functions
  template <typename EvalT>
  Teuchos::RCP<const PHX::FieldTag>
  constructEvaluators(
      PHX::FieldManager<PHAL::AlbanyTraits>&      fm0,
      Albany::MeshSpecsStruct const&              meshSpecs,
      Albany::StateManager&                       stateMgr,
      Albany::FieldManagerChoice                  fmchoice,
      const Teuchos::RCP<Teuchos::ParameterList>& responseList);

  void
  constructDirichletEvaluators(Albany::MeshSpecsStruct const& meshSpecs);

 protected:
  //! Boundary conditions on source term
  int numDim;

  /// Boolean marking whether SDBCs are used
  bool use_sdbcs_;
};

}  // namespace Albany

#include "Albany_EvaluatorUtils.hpp"
#include "Albany_ProblemUtils.hpp"
#include "Albany_ResponseUtilities.hpp"
#include "Albany_Utils.hpp"
#include "Intrepid2_DefaultCubatureFactory.hpp"
#include "PHAL_HeatEqResid.hpp"
#include "PHAL_JouleHeating.hpp"
#include "PHAL_PoissonResid.hpp"
#include "PHAL_TEProp.hpp"
#include "Shards_CellTopology.hpp"

template <typename EvalT>
Teuchos::RCP<const PHX::FieldTag>
Albany::ThermoElectrostaticsProblem::constructEvaluators(
    PHX::FieldManager<PHAL::AlbanyTraits>&      fm0,
    Albany::MeshSpecsStruct const&              meshSpecs,
    Albany::StateManager&                       stateMgr,
    Albany::FieldManagerChoice                  fieldManagerChoice,
    const Teuchos::RCP<Teuchos::ParameterList>& responseList)
{
  using PHAL::AlbanyTraits;
  using PHX::DataLayout;
  using PHX::MDALayout;
  using std::string;
  using std::vector;
  using Teuchos::ParameterList;
  using Teuchos::RCP;
  using Teuchos::rcp;

  RCP<shards::CellTopology>                              cellType      = rcp(new shards::CellTopology(&meshSpecs.ctd));
  RCP<Intrepid2::Basis<PHX::Device, RealType, RealType>> intrepidBasis = Albany::getIntrepid2Basis(meshSpecs.ctd);

  int const numNodes    = intrepidBasis->getCardinality();
  int const worksetSize = meshSpecs.worksetSize;

  Intrepid2::DefaultCubatureFactory     cubFactory;
  RCP<Intrepid2::Cubature<PHX::Device>> cubature =
      cubFactory.create<PHX::Device, RealType, RealType>(*cellType, meshSpecs.cubatureDegree);

  int const numQPts     = cubature->getNumPoints();
  int const numVertices = cellType->getNodeCount();

  *out << "Field Dimensions: Workset=" << worksetSize << ", Vertices= " << numVertices << ", Nodes= " << numNodes
       << ", QuadPts= " << numQPts << ", Dim= " << numDim << std::endl;

  RCP<Albany::Layouts> dl = rcp(new Albany::Layouts(worksetSize, numVertices, numNodes, numQPts, numDim));
  Albany::EvaluatorUtils<EvalT, PHAL::AlbanyTraits> evalUtils(dl);
  bool                                              supportsTransient = false;

  // Problem is not transient
  ALBANY_PANIC(
      number_of_time_deriv != 0,
      "Albany_ThermoElectroStaticsProblem cannot be defined as a transient "
      "calculation.");

  // Temporary variable used numerous times below
  Teuchos::RCP<PHX::Evaluator<AlbanyTraits>> ev;

  // Define Field Names

  Teuchos::ArrayRCP<string> dof_names(neq);
  dof_names[0] = "Potential";
  dof_names[1] = "Temperature";

  Teuchos::ArrayRCP<string> dof_names_dot(neq);
  if (supportsTransient) {
    for (int i = 0; i < neq; i++) dof_names_dot[i] = dof_names[i] + "_dot";
  }

  Teuchos::ArrayRCP<string> resid_names(neq);
  for (int i = 0; i < neq; i++) resid_names[i] = dof_names[i] + " Residual";

  if (supportsTransient)
    fm0.template registerEvaluator<EvalT>(evalUtils.constructGatherSolutionEvaluator(false, dof_names, dof_names_dot));
  else
    fm0.template registerEvaluator<EvalT>(evalUtils.constructGatherSolutionEvaluator_noTransient(false, dof_names));

  fm0.template registerEvaluator<EvalT>(evalUtils.constructScatterResidualEvaluator(false, resid_names));

  fm0.template registerEvaluator<EvalT>(evalUtils.constructGatherCoordinateVectorEvaluator());

  fm0.template registerEvaluator<EvalT>(evalUtils.constructMapToPhysicalFrameEvaluator(cellType, cubature));

  fm0.template registerEvaluator<EvalT>(
      evalUtils.constructComputeBasisFunctionsEvaluator(cellType, intrepidBasis, cubature));

  for (int i = 0; i < neq; i++) {
    fm0.template registerEvaluator<EvalT>(evalUtils.constructDOFInterpolationEvaluator(dof_names[i], i));

    if (supportsTransient)
      fm0.template registerEvaluator<EvalT>(evalUtils.constructDOFInterpolationEvaluator(dof_names_dot[i], i));

    fm0.template registerEvaluator<EvalT>(evalUtils.constructDOFGradInterpolationEvaluator(dof_names[i], i));
  }

  {  // Thermal conductivity
    RCP<ParameterList> p = rcp(new ParameterList);

    p->set<string>("QP Variable Name", "ThermalConductivity");
    p->set<string>("QP Variable Name 2",
                   "Permittivity");  // really electrical conductivity
    p->set<string>("QP Variable Name 3", "Rho Cp");
    p->set<string>("Coordinate Vector Name", "Coord Vec");
    p->set<string>("Temperature Variable Name", "Temperature");
    p->set<RCP<DataLayout>>("QP Scalar Data Layout", dl->qp_scalar);
    p->set<RCP<DataLayout>>("QP Vector Data Layout", dl->qp_vector);

    p->set<RCP<ParamLib>>("Parameter Library", paramLib);
    Teuchos::ParameterList& paramList = params->sublist("TE Properties");
    p->set<Teuchos::ParameterList*>("Parameter List", &paramList);

    ev = rcp(new PHAL::TEProp<EvalT, AlbanyTraits>(*p));
    fm0.template registerEvaluator<EvalT>(ev);
  }

  {
    RCP<ParameterList> p = rcp(new ParameterList);

    // Input
    p->set<string>("Gradient Variable Name", "Potential Gradient");
    p->set<string>("Flux Variable Name", "Potential Flux");
    p->set<RCP<DataLayout>>("QP Vector Data Layout", dl->qp_vector);

    p->set<RCP<ParamLib>>("Parameter Library", paramLib);

    // Output
    p->set<string>("Source Name", "Joule");
    p->set<RCP<DataLayout>>("QP Scalar Data Layout", dl->qp_scalar);

    ev = rcp(new PHAL::JouleHeating<EvalT, AlbanyTraits>(*p));
    fm0.template registerEvaluator<EvalT>(ev);
  }

  {  // Potential Resid
    RCP<ParameterList> p = rcp(new ParameterList("Potential Resid"));

    // Input
    p->set<string>("Weighted BF Name", "wBF");
    p->set<string>("QP Variable Name", "Potential");

    p->set<string>("Permittivity Name", "Permittivity");

    p->set<string>("Gradient QP Variable Name", "Potential Gradient");
    p->set<string>("Flux QP Variable Name", "Potential Flux");

    p->set<string>("Weighted Gradient BF Name", "wGrad BF");

    p->set<bool>("Have Source", false);
    p->set<string>("Source Name", "None");

    // Output
    p->set<string>("Residual Name", "Potential Residual");

    ev = rcp(new PHAL::PoissonResid<EvalT, AlbanyTraits>(*p, dl));
    fm0.template registerEvaluator<EvalT>(ev);
  }

  {  // Temperature Resid
    RCP<ParameterList> p = rcp(new ParameterList("Temperature Resid"));

    // Input
    p->set<string>("Weighted BF Name", "wBF");
    p->set<RCP<DataLayout>>("Node QP Scalar Data Layout", dl->node_qp_scalar);
    p->set<string>("QP Variable Name", "Temperature");

    p->set<bool>("Have Source", true);
    p->set<string>("Source Name", "Joule");

    p->set<bool>("Have Absorption", false);

    p->set<string>("ThermalConductivity Name", "ThermalConductivity");
    p->set<RCP<DataLayout>>("QP Scalar Data Layout", dl->qp_scalar);

    p->set<string>("Gradient QP Variable Name", "Temperature Gradient");
    p->set<RCP<DataLayout>>("QP Vector Data Layout", dl->qp_vector);

    p->set<string>("Weighted Gradient BF Name", "wGrad BF");
    p->set<RCP<DataLayout>>("Node QP Vector Data Layout", dl->node_qp_vector);

    // Poisson solve does not have transient terms
    p->set<bool>("Disable Transient", true);
    p->set<string>("QP Time Derivative Variable Name", "Temperature_dot");

    if (params->isType<string>("Convection Velocity")) {
      p->set<string>("Convection Velocity", params->get<string>("Convection Velocity"));
      p->set<string>("Rho Cp Name", "Rho Cp");
    }

    // Output
    p->set<string>("Residual Name", "Temperature Residual");
    p->set<RCP<DataLayout>>("Node Scalar Data Layout", dl->node_scalar);

    ev = rcp(new PHAL::HeatEqResid<EvalT, AlbanyTraits>(*p));
    fm0.template registerEvaluator<EvalT>(ev);
  }

  if (fieldManagerChoice == Albany::BUILD_RESID_FM) {
    PHX::Tag<typename EvalT::ScalarT> res_tag("Scatter", dl->dummy);
    fm0.requireField<EvalT>(res_tag);
    return res_tag.clone();
  }

  else if (fieldManagerChoice == Albany::BUILD_RESPONSE_FM) {
    Albany::ResponseUtilities<EvalT, PHAL::AlbanyTraits> respUtils(dl);
    return respUtils.constructResponses(fm0, *responseList, Teuchos::null, stateMgr);
  }

  return Teuchos::null;
}
#endif  // ALBANY_HEATNONLINEARSOURCEPROBLEM_HPP
