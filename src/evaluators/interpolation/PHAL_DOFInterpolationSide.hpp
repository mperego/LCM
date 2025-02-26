// Albany 3.0: Copyright 2016 National Technology & Engineering Solutions of
// Sandia, LLC (NTESS). This Software is released under the BSD license detailed
// in the file license.txt in the top-level Albany directory.

#ifndef PHAL_DOF_INTERPOLATION_SIDE_HPP
#define PHAL_DOF_INTERPOLATION_SIDE_HPP 1

#include "Albany_Layouts.hpp"
#include "Albany_ScalarOrdinalTypes.hpp"
#include "PHAL_Dimension.hpp"
#include "PHAL_Utilities.hpp"
#include "Phalanx_Evaluator_Derived.hpp"
#include "Phalanx_Evaluator_WithBaseImpl.hpp"
#include "Phalanx_MDField.hpp"
#include "Phalanx_config.hpp"

namespace PHAL {
/** \brief Finite Element InterpolationSide Evaluator

    This evaluator interpolates nodal DOF values to quad points.

*/

template <typename EvalT, typename Traits, typename ScalarT>
class DOFInterpolationSideBase : public PHX::EvaluatorWithBaseImpl<Traits>, public PHX::EvaluatorDerived<EvalT, Traits>
{
 public:
  DOFInterpolationSideBase(Teuchos::ParameterList const& p, const Teuchos::RCP<Albany::Layouts>& dl_side);

  void
  postRegistrationSetup(typename Traits::SetupData d, PHX::FieldManager<Traits>& vm);

  void
  evaluateFields(typename Traits::EvalData d);

 private:
  std::string sideSetName;

  // Input:
  //! Values at nodes
  PHX::MDField<ScalarT const, Cell, Side, Node> val_node;
  //! Basis Functions
  PHX::MDField<const RealType, Cell, Side, Node, QuadPoint> BF;

  // Output:
  //! Values at quadrature points
  PHX::MDField<ScalarT, Side, Cell, QuadPoint> val_qp;

  int numSideNodes;
  int numSideQPs;
};

// Some shortcut names
template <typename EvalT, typename Traits>
using DOFInterpolationSide = DOFInterpolationSideBase<EvalT, Traits, typename EvalT::ScalarT>;

template <typename EvalT, typename Traits>
using DOFInterpolationSideMesh = DOFInterpolationSideBase<EvalT, Traits, typename EvalT::MeshScalarT>;

template <typename EvalT, typename Traits>
using DOFInterpolationSideParam = DOFInterpolationSideBase<EvalT, Traits, typename EvalT::ParamScalarT>;

}  // Namespace PHAL

#endif  // PHAL_DOF_INTERPOLATION_SIDE_HPP
