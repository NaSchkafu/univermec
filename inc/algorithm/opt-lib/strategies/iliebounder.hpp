#ifndef __ILIEBOUNDER_HPP__
#define __ILIEBOUNDER_HPP__

#include "functions/ifunction.hpp"
#include "functions/iffeatures.hpp"
#include "functions/icustomconsistency.hpp"
#include "functions/enclosures/ilieencl.hpp"

#include "../optenv.hpp"
#include "../optbox.hpp"
#include "../stratele.hpp"


namespace GlobOpt
{
  /// Bounds the objective function using ILIEs and prunes
  /**
   * The objective function is bounded through affine arithmetic
   * and during this process linearized. In a second step the
   * linearization is used for pruning the objective function
   * using the consistency constraint
   * obj <= ubound
   */
  struct ilie_bounder_t : public strat_ele_t 
  {
    virtual bool apply(opt_worker_state_t &env) const
      {
	functions::encl::ILIEEncl encl(*env.obj);
	std::unique_ptr<functions::IFunction> lobj(encl.encl(env.cbox->ref_X()));
	if(lobj) {
	  core::arith::interval bound((*lobj)(env.cbox->ref_X()));
	  if(is_empty(bound)){
	    std::cout << "FFun Bound: " << bound << std::endl;
	    assert(!"Fehler in Zielfunktion!");
	    return true;
	  }
	  core::arith:: interval oldbound(env.cbox->bound());
	  env.cbox->bound() &= bound;
	  if(is_empty(env.cbox->bound())) {
	    std::cout << "Fehler im ILIE Bounder" << std::endl;
	    std::cout << "Box: " << env.cbox->ref_X() << std::endl;
	    std::cout << "ILIE Bound: " << bound << std::endl;
	    std::cout << "Aff Bound: " << (*env.obj)(core::arith::avector(env.cbox->ref_X())) << std::endl;
	    std::cout << "oldbound: " << oldbound << std::endl;
	    assert(false);
	  }
	  if(!is_infinity(env.ubound()))
	    return lobj->features().custom_consistency()->get(0).prune(*env.cbox, 
								       core::arith::interval(-core::arith::infinity(),env.ubound()));
	}
	return false;
      }
    
    virtual std::string name() const
      {
	return "ILIE Bounder";
      }
  };
}


#endif /*__ILIEBOUNDER_HPP__*/
