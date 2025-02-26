// Albany 3.0: Copyright 2016 National Technology & Engineering Solutions of
// Sandia, LLC (NTESS). This Software is released under the BSD license detailed
// in the file license.txt in the top-level Albany directory.

#ifndef PHAL_HEATEQRESID_HPP
#define PHAL_HEATEQRESID_HPP

#include "Albany_Types.hpp"
#include "Phalanx_Evaluator_Derived.hpp"
#include "Phalanx_Evaluator_WithBaseImpl.hpp"
#include "Phalanx_MDField.hpp"
#include "Phalanx_config.hpp"

namespace PHAL {

/** \brief Finite Element Interpolation Evaluator

    This evaluator interpolates nodal DOF values to quad points.

*/

template <typename EvalT, typename Traits>
class HeatEqResid : public PHX::EvaluatorWithBaseImpl<Traits>, public PHX::EvaluatorDerived<EvalT, Traits>
{
 public:
  HeatEqResid(Teuchos::ParameterList const& p);

  void
  postRegistrationSetup(typename Traits::SetupData d, PHX::FieldManager<Traits>& vm);

  void
  evaluateFields(typename Traits::EvalData d);

 private:
  typedef typename EvalT::ScalarT     ScalarT;
  typedef typename EvalT::MeshScalarT MeshScalarT;

  // Input:
  PHX::MDField<const MeshScalarT, Cell, Node, QuadPoint>      wBF;
  PHX::MDField<ScalarT const, Cell, QuadPoint>                Temperature;
  PHX::MDField<ScalarT const, Cell, QuadPoint>                Tdot;
  PHX::MDField<ScalarT const, Cell, QuadPoint>                ThermalCond;
  PHX::MDField<const MeshScalarT, Cell, Node, QuadPoint, Dim> wGradBF;
  PHX::MDField<ScalarT const, Cell, QuadPoint, Dim>           TGrad;
  PHX::MDField<ScalarT const, Cell, QuadPoint>                Source;
  PHX::MDField<ScalarT const, Cell, QuadPoint>                rhoCp;
  PHX::MDField<ScalarT const, Cell, QuadPoint>                Absorption;
  Teuchos::Array<double>                                      convectionVels;

  // Output:
  PHX::MDField<ScalarT, Cell, Node> TResidual;

  bool                                      haveSource;
  bool                                      haveConvection;
  bool                                      haveAbsorption;
  bool                                      enableTransient;
  bool                                      haverhoCp;
  unsigned int                              numQPs, numDims, numNodes, worksetSize;
  Kokkos::DynRankView<ScalarT, PHX::Device> flux;
  Kokkos::DynRankView<ScalarT, PHX::Device> aterm;
  Kokkos::DynRankView<ScalarT, PHX::Device> convection;
};
}  // namespace PHAL

#endif
