// Albany 3.0: Copyright 2016 National Technology & Engineering Solutions of
// Sandia, LLC (NTESS). This Software is released under the BSD license detailed
// in the file license.txt in the top-level Albany directory.

#ifndef ALBANY_ODEPROBLEM_HPP
#define ALBANY_ODEPROBLEM_HPP

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
class ODEProblem : public AbstractProblem
{
 public:
  //! Default constructor
  ODEProblem(
      const Teuchos::RCP<Teuchos::ParameterList>& params,
      const Teuchos::RCP<ParamLib>&               paramLib,
      int const                                   numDim_);

  //! Destructor
  ~ODEProblem();

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
  ODEProblem(const ODEProblem&);

  //! Private to prohibit copying
  ODEProblem&
  operator=(const ODEProblem&);

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
  int numDim;

  /// Boolean marking whether SDBCs are used
  bool use_sdbcs_;
};

}  // namespace Albany

#include "Albany_EvaluatorUtils.hpp"
#include "Albany_ProblemUtils.hpp"
#include "Albany_ResponseUtilities.hpp"
#include "Albany_Utils.hpp"
#include "PHAL_ODEResid.hpp"
#include "Shards_CellTopology.hpp"

template <typename EvalT>
Teuchos::RCP<const PHX::FieldTag>
Albany::ODEProblem::constructEvaluators(
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

  int const numNodes = 1;

  int const numVertices = 1;
  int const worksetSize = meshSpecs.worksetSize;

  *out << "Field Dimensions: Workset=" << worksetSize << ", Vertices= " << numVertices << ", Nodes= " << numNodes
       << ", Dim= " << numDim << std::endl;

  RCP<Albany::Layouts> dl = rcp(new Albany::Layouts(worksetSize, numVertices, numNodes, 1, numDim));
  Albany::EvaluatorUtils<EvalT, PHAL::AlbanyTraits> evalUtils(dl);
  bool                                              supportsTransient = true;

  // Problem is transient
  ALBANY_PANIC(number_of_time_deriv != 1, "Albany_ODEProblem must be defined as a transient calculation.");

  // Temporary variable used numerous times below
  Teuchos::RCP<PHX::Evaluator<AlbanyTraits>> ev;

  // Define Field Names

  Teuchos::ArrayRCP<string> dof_names(neq);
  dof_names[0] = "X";
  dof_names[1] = "Y";

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

  {  // X Resid
    RCP<ParameterList> p = rcp(new ParameterList("ODE Resid"));

    // Input
    p->set<RCP<DataLayout>>("Node Scalar Data Layout", dl->node_scalar);
    p->set<string>("Variable Name", "X");
    p->set<string>("Time Derivative Variable Name", "X_dot");
    p->set<string>("Y Variable Name", "Y");
    p->set<string>("Y Time Derivative Variable Name", "Y_dot");

    // Output
    p->set<string>("Residual Name", "X Residual");
    p->set<string>("Y Residual Name", "Y Residual");

    ev = rcp(new PHAL::ODEResid<EvalT, AlbanyTraits>(*p));
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
#endif
