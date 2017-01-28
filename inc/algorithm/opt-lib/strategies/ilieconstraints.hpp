#ifndef __ILIECBOUNDER_HPP__
#define __ILIECBOUNDER_HPP__

#include "functions/ifunction.hpp"
#include "functions/iffeatures.hpp"
#include "functions/icustomconsistency.hpp"
#include "functions/enclosures/ilieencl.hpp"

#include "../optenv.hpp"
#include "../optbox.hpp"
#include "../stratele.hpp"


namespace GlobOpt
{
  /// Bounds the constraints using ILIEs and prunes
  /**
   * The constraint functions are bounded through affine arithmetic
   * and are during this process linearized. In a second step the
   * linearization is used for pruning the constraint functions
   * using the consistency constraint
   * ineq_const <= 0
   * eq_const = 0
y   */
  struct ilie_cbounder_t : public strat_ele_t 
  {
    virtual bool apply(opt_worker_state_t &env) const
      {
	for(auto cit(env.ineq_constraints->begin()); cit != env.ineq_constraints->end(); ++cit) {
	  // std::cout << "****************************************************************" << std::endl;
	  functions::encl::ILIEEncl encl(*cit);
	  std::unique_ptr<functions::IFunction> lobj(encl.encl(env.cbox->ref_X()));
	  if(lobj && lobj->features().custom_consistency()->get(0).prune(*env.cbox, 
									 core::arith::interval(-core::arith::infinity(), 0.0))) {
	    // std::cout << "NB: " << cit->get().text() << std::endl;
	    // std::cout << "Alte Box " << old_x << std::endl;
	    // std::cout << "Verworfene Neue Box: " << env.cbox->ref_X() << std::endl;
	    // std::cout << "Bound old_x cIt " << (cit->get()(old_x)) << std::endl;  
	    // std::cout << "Bound old_x encl " << (*lobj)(old_x) << std::endl;
	    return true;
	  }
	  // if(old_x != env.cbox->ref_X()) {
	  //   std::cout << "NB: " << cit->get().text() << std::endl;
	  //   std::cout << "Alte Box " << old_x << std::endl;
	  //   std::cout << "Verkleinerte Neue Box: " << env.cbox->ref_X() << std::endl;
	  //   std::cout << "Bound old_x cIt " << (cit->get()(old_x)) << std::endl;  
	  //   std::cout << "Bound old_x encl " << (*lobj)(old_x) << std::endl;
	  //   std::cout << "Neue Box: " << env.cbox->ref_X() << std::endl;
	  //   std::cout << "";
	  // }
	}
	return false;
      }
    
    virtual std::string name() const
      {
	return "ILIE CBounder";
      }
  };
}


#endif /*__ILIECBOUNDER_HPP__*/
