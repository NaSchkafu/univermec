#ifndef __MEANVALUE_HPP__
#define __MEANVALUE_HPP__

#include "../iencl.hpp"

namespace functions 
{
  class IDerivative;
  
  namespace encl 
  {
    /// function enclosure using the mean value form for a function
    /**
     * Constructs the mean value form enclosure for a function using first-order
     * information. The enclosure is only locally valid for a specific box x.
     * 
     */
    class MeanValueForm : public IEncl
    {
    public:
      /** 
       * Ctor
       * Constructrs a mean-value form enclosure for a function
       * 
       * @param f reference to function
       * @param prog if true the progressive gradient technique due to Hansen is used
       * 
       */
      MeanValueForm(const IFunction &f, bool prog = false);
      
    private:
      // IEncl implementation
      virtual bool is_global_() const;
      virtual IFunction* encl_(const::core::arith::ivector &box) const;
      virtual IFunction* encl_() const;
    
    private:
      const IFunction &m_f;
      const bool m_prog;
    };
  }
}

#endif /*__MEANVALUE_HPP__*/
