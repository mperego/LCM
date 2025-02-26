// Albany 3.0: Copyright 2016 National Technology & Engineering Solutions of
// Sandia, LLC (NTESS). This Software is released under the BSD license detailed
// in the file license.txt in the top-level Albany directory.

#ifndef PHAL_RESPONSE_THERMAL_ENERGY_HPP
#define PHAL_RESPONSE_THERMAL_ENERGY_HPP

#include "PHAL_SeparableScatterScalarResponse.hpp"

namespace PHAL {
/**
 * \brief Response Description
 * This response evaluate the thermal energy : e = int_{\Omega} \rho * c_{p} T
 * Where \Omega is the domain, \rho density, C_{p} specific heat and T is
 * temperature.
 */
template <typename EvalT, typename Traits>
class ResponseThermalEnergy : public PHAL::SeparableScatterScalarResponse<EvalT, Traits>
{
 public:
  typedef typename EvalT::ScalarT     ScalarT;
  typedef typename EvalT::MeshScalarT MeshScalarT;

  ResponseThermalEnergy(Teuchos::ParameterList& p, const Teuchos::RCP<Albany::Layouts>& dl);

  void
  postRegistrationSetup(typename Traits::SetupData d, PHX::FieldManager<Traits>& vm);

  void
  preEvaluate(typename Traits::PreEvalData d);

  void
  evaluateFields(typename Traits::EvalData d);

  void
  postEvaluate(typename Traits::PostEvalData d);

 private:
  Teuchos::RCP<Teuchos::ParameterList const>
  getValidResponseParameters() const;

  // temperature
  PHX::MDField<ScalarT const> field;
  // time
  //    PHX::MDField<ScalarT,Dummy> time;
  //    PHX::MDField<ScalarT,Dummy> deltaTime;
  // coordinates
  PHX::MDField<const MeshScalarT> coordVec;
  // Quadrature points
  PHX::MDField<const MeshScalarT>         weights;
  std::vector<PHX::DataLayout::size_type> field_dims;
  std::size_t                             numQPs;
  std::size_t                             numDims;
  // density: assumed constant in element block
  ScalarT density;
  // specific heat : assumed constant element block
  ScalarT heat_capacity;
};

}  // namespace PHAL

#endif  // PHAL_RESPONSE_THERMAL_ENERGY_HPP
