// Albany 3.0: Copyright 2016 National Technology & Engineering Solutions of
// Sandia, LLC (NTESS). This Software is released under the BSD license detailed
// in the file license.txt in the top-level Albany directory.

#include "Albany_Macros.hpp"
#include "Intrepid2_FunctionSpaceTools.hpp"
#include "PHAL_MapToPhysicalFrame.hpp"
#include "Phalanx_DataLayout.hpp"

namespace PHAL {

//*****
template <typename EvalT, typename Traits>
MapToPhysicalFrame<EvalT, Traits>::MapToPhysicalFrame(
    Teuchos::ParameterList const&        p,
    const Teuchos::RCP<Albany::Layouts>& dl)
    : coords_vertices(p.get<std::string>("Coordinate Vector Name"), dl->vertices_vector),
      cubature(p.get<Teuchos::RCP<Intrepid2::Cubature<PHX::Device>>>("Cubature")),
      intrepidBasis(p.get<Teuchos::RCP<Intrepid2::Basis<PHX::Device, RealType, RealType>>>("Intrepid2 Basis")),
      cellType(p.get<Teuchos::RCP<shards::CellTopology>>("Cell Type")),
      coords_qp(p.get<std::string>("Coordinate Vector Name"), dl->qp_gradient)
{
  this->addDependentField(coords_vertices.fieldTag());
  this->addEvaluatedField(coords_qp);

  // Get Dimensions
  std::vector<PHX::DataLayout::size_type> dims;
  dl->qp_gradient->dimensions(dims);

  numQPs = dims[1];
  numDim = dims[2];

  this->setName("MapToPhysicalFrame" + PHX::print<EvalT>());
}

//*****
template <typename EvalT, typename Traits>
void
MapToPhysicalFrame<EvalT, Traits>::postRegistrationSetup(typename Traits::SetupData d, PHX::FieldManager<Traits>& fm)
{
  this->utils.setFieldData(coords_vertices, fm);
  this->utils.setFieldData(coords_qp, fm);

  // Compute cubature points in reference elements
  refPoints  = Kokkos::DynRankView<RealType, PHX::Device>("XXX", numQPs, numDim);
  refWeights = Kokkos::DynRankView<RealType, PHX::Device>("XXX", numQPs);
  cubature->getCubature(refPoints, refWeights);

  d.fill_field_dependencies(this->dependentFields(), this->evaluatedFields());
}
//*****
template <typename EvalT, typename Traits>
void
MapToPhysicalFrame<EvalT, Traits>::evaluateFields(typename Traits::EvalData workset)
{
  if (intrepidBasis != Teuchos::null) {
    Intrepid2::CellTools<PHX::Device>::mapToPhysicalFrame(
        coords_qp.get_view(), refPoints, coords_vertices.get_view(), intrepidBasis);
  } else {
    Intrepid2::CellTools<PHX::Device>::mapToPhysicalFrame(
        coords_qp.get_view(), refPoints, coords_vertices.get_view(), *cellType);
  }
}

//*****
}  // namespace PHAL
