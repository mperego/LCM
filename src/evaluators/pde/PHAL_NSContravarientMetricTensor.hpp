// Albany 3.0: Copyright 2016 National Technology & Engineering Solutions of
// Sandia, LLC (NTESS). This Software is released under the BSD license detailed
// in the file license.txt in the top-level Albany directory.

#ifndef PHAL_NSCONTRAVARIENTMETRICTENSOR_HPP
#define PHAL_NSCONTRAVARIENTMETRICTENSOR_HPP

#include "Albany_ScalarOrdinalTypes.hpp"
#include "Intrepid2_CellTools.hpp"
#include "Intrepid2_Cubature.hpp"
#include "PHAL_Dimension.hpp"
#include "Phalanx_Evaluator_Derived.hpp"
#include "Phalanx_Evaluator_WithBaseImpl.hpp"
#include "Phalanx_MDField.hpp"
#include "Phalanx_config.hpp"

namespace PHAL {
/** \brief Finite Element Interpolation Evaluator

    This evaluator interpolates nodal DOF values to quad points.

*/

template <typename EvalT, typename Traits>
class NSContravarientMetricTensor : public PHX::EvaluatorWithBaseImpl<Traits>,
                                    public PHX::EvaluatorDerived<EvalT, Traits>
{
 public:
  NSContravarientMetricTensor(Teuchos::ParameterList const& p);

  void
  postRegistrationSetup(typename Traits::SetupData d, PHX::FieldManager<Traits>& vm);

  void
  evaluateFields(typename Traits::EvalData d);

 private:
  typedef typename EvalT::MeshScalarT MeshScalarT;
  int                                 numDims, numQPs, numCells;

  // Input:
  //! Coordinate vector at vertices
  PHX::MDField<const MeshScalarT, Cell, Vertex, Dim> coordVec;
  Teuchos::RCP<Intrepid2::Cubature<PHX::Device>>     cubature;
  Teuchos::RCP<shards::CellTopology>                 cellType;

  // Temporary Kokkos Views
  Kokkos::DynRankView<RealType, PHX::Device>    refPoints;
  Kokkos::DynRankView<RealType, PHX::Device>    refWeights;
  Kokkos::DynRankView<MeshScalarT, PHX::Device> jacobian;
  Kokkos::DynRankView<MeshScalarT, PHX::Device> jacobian_inv;

  // Output:
  PHX::MDField<MeshScalarT, Cell, QuadPoint, Dim, Dim> Gc;
};
}  // namespace PHAL

#endif
