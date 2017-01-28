#ifndef __DUMMYU_HPP__
#define __DUMMYU_HPP__

#include "functions/ivfunction.hpp"

namespace ivp
{
  namespace details
  {
    // /// A dummy function R^0 -> R^0
    // /**
    //  * This function should be used for u(t) if the IVP has no
    //  * time dependent parameters.
    //  */
    // class DummyU : public functions::IVFunction
    // {
    // public:
    //   virtual ~DummyU() {}

    // private:
    //   // IVFunction
    //   virtual core::arith::ivector calc_(const core::arith::ivector &args) const;
    //   virtual core::arith::avector calc_(const core::arith::avector &args) const;
    //   virtual core::arith::rvector calc_(const core::arith::rvector &args) const;
    //   virtual core::arith::tvector calc_(const core::arith::tvector &args) const;
    //   virtual const functions::IFunction& fun_(unsigned i) const;
    //   virtual unsigned dim_() const;
    //   virtual unsigned cdim_() const;
    //   virtual IVFunction* clone_() const;
    //   virtual std::string text_() const;  
    // };

    /**
     * This function should be used for u(t) if the IVP has no
     * time dependent parameters.
     */
    functions::IVFunction* create_dummyu();
    
  }
}

#endif /*__DUMMYU_HPP__*/
