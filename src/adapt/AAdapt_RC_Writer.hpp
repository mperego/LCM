// Albany 3.0: Copyright 2016 National Technology & Engineering Solutions of
// Sandia, LLC (NTESS). This Software is released under the BSD license detailed
// in the file license.txt in the top-level Albany directory.

#ifndef AADAPT_RC_WRITER_HPP
#define AADAPT_RC_WRITER_HPP

#include "Albany_Layouts.hpp"
#include "PHAL_AlbanyTraits.hpp"
#include "Phalanx_Evaluator_Derived.hpp"
#include "Phalanx_Evaluator_WithBaseImpl.hpp"
#include "Phalanx_MDField.hpp"

namespace AAdapt {
namespace rc {

class Manager;

/*! \brief This evaluator writes data from the upstream evaluators to the
 *         rc::Manager.
 *
 *  Only the Residual specialization is used, because only RealType state values
 *  are used in the reference configuration update.
 */

template <typename EvalT, typename Traits>
class WriterBase : public PHX::EvaluatorWithBaseImpl<Traits>, public PHX::EvaluatorDerived<EvalT, Traits>
{
 public:
  WriterBase();
  void
  postRegistrationSetup(typename Traits::SetupData d, PHX::FieldManager<Traits>& fm) = 0;
  void
  evaluateFields(typename Traits::EvalData d) = 0;
  const Teuchos::RCP<const PHX::FieldTag>&
  getNoOutputTag();

 private:
  Teuchos::RCP<const PHX::FieldTag> nooutput_tag_;
};

template <typename EvalT, typename Traits>
class Writer : public WriterBase<EvalT, Traits>
{
 public:
  void
  postRegistrationSetup(typename Traits::SetupData /* d */, PHX::FieldManager<Traits>& /* fm */)
  {
  }
  void evaluateFields(typename Traits::EvalData /* d */) {}
};

template <typename Traits>
class Writer<PHAL::AlbanyTraits::Residual, Traits> : public WriterBase<PHAL::AlbanyTraits::Residual, Traits>
{
 public:
  Writer(const Teuchos::RCP<Manager>& rc_mgr, const Teuchos::RCP<Albany::Layouts>& dl);
  void
  postRegistrationSetup(typename Traits::SetupData d, PHX::FieldManager<Traits>& fm);
  void
  evaluateFields(typename Traits::EvalData d);

 private:
  typedef typename std::vector<PHX::MDField<const RealType>> FieldsVector;
  typedef typename FieldsVector::iterator                    FieldsIterator;
  Teuchos::RCP<Manager>                                      rc_mgr_;
  FieldsVector                                               fields_;
  PHX::MDField<const RealType, Cell, Node, QuadPoint>        bf_, wbf_;
};

}  // namespace rc
}  // namespace AAdapt

#endif  // AADAPT_RC_WRITER_HPP
