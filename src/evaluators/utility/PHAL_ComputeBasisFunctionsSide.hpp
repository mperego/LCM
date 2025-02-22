// Albany 3.0: Copyright 2016 National Technology & Engineering Solutions of
// Sandia, LLC (NTESS). This Software is released under the BSD license detailed
// in the file license.txt in the top-level Albany directory.

#ifndef PHAL_COMPUTE_BASIS_FUNCTIONS_SIDE_HPP
#define PHAL_COMPUTE_BASIS_FUNCTIONS_SIDE_HPP 1

#include "Albany_Layouts.hpp"
#include "Intrepid2_CellTools.hpp"
#include "Intrepid2_Cubature.hpp"
#include "PHAL_Utilities.hpp"
#include "Phalanx_Evaluator_Derived.hpp"
#include "Phalanx_Evaluator_WithBaseImpl.hpp"
#include "Phalanx_MDField.hpp"
#include "Phalanx_config.hpp"

namespace PHAL {

/** \brief Finite Element Interpolation Evaluator

    This evaluator interpolates nodal DOF values to quad points.

*/
template <typename EvalT, typename Traits>
class ComputeBasisFunctionsSide : public PHX::EvaluatorWithBaseImpl<Traits>, public PHX::EvaluatorDerived<EvalT, Traits>
{
 public:
  ComputeBasisFunctionsSide(Teuchos::ParameterList const& p, const Teuchos::RCP<Albany::Layouts>& dl);

  void
  postRegistrationSetup(typename Traits::SetupData d, PHX::FieldManager<Traits>& vm);

  void
  evaluateFields(typename Traits::EvalData d);

 private:
  typedef typename EvalT::MeshScalarT MeshScalarT;
  int                                 numSides, numSideNodes, numSideQPs, numCellDims, numSideDims, numNodes;

  //! The side set where to compute the Basis Functions
  std::string sideSetName;

  // Input:
  //! Coordinate vector at side's vertices
  PHX::MDField<const MeshScalarT, Cell, Side, Vertex, Dim> sideCoordVec;
  PHX::MDField<const MeshScalarT, Cell, Vertex, Dim>       coordVec;

  // Temporary Kokkos Views
  Kokkos::DynRankView<RealType, PHX::Device> val_at_cub_points;
  Kokkos::DynRankView<RealType, PHX::Device> grad_at_cub_points;
  Kokkos::DynRankView<RealType, PHX::Device> cub_weights;
  Kokkos::DynRankView<RealType, PHX::Device> cub_points;

  Teuchos::RCP<Intrepid2::Cubature<PHX::Device>>                  cubature;
  Teuchos::RCP<Intrepid2::Basis<PHX::Device, RealType, RealType>> intrepidBasis;

  // Output:
  //! Basis Functions and other quantities at quadrature points
  PHX::MDField<MeshScalarT, Cell, Side, QuadPoint>            metric_det;
  PHX::MDField<MeshScalarT, Cell, Side, QuadPoint, Dim, Dim>  tangents;
  PHX::MDField<MeshScalarT, Cell, Side, QuadPoint, Dim, Dim>  metric;
  PHX::MDField<MeshScalarT, Cell, Side, QuadPoint>            w_measure;
  PHX::MDField<MeshScalarT, Cell, Side, QuadPoint, Dim, Dim>  inv_metric;
  PHX::MDField<RealType, Cell, Side, Node, QuadPoint>         BF;
  PHX::MDField<MeshScalarT, Cell, Side, Node, QuadPoint, Dim> GradBF;
  PHX::MDField<MeshScalarT, Cell, Side, QuadPoint, Dim>       normals;

  std::vector<std::vector<int>>                      sideNodes;
  std::vector<Kokkos::DynRankView<int, PHX::Device>> cellsOnSides;
  std::vector<int>                                   numCellsOnSide;
  Teuchos::RCP<shards::CellTopology>                 cellType;
  bool                                               compute_normals;
};

}  // Namespace PHAL

#endif  // PHAL_COMPUTE_BASIS_FUNCTIONS_SIDE_HPP
