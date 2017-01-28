#ifndef __NENCL_HPP__
#define __NENCL_HPP__

#include "../iencl.hpp"

namespace functions 
{
  namespace encl
  {
    /// natural enclosures
    template<int N>
    class NEncl : public IEncl 
    {
    public:
      explicit NEncl(const functions::IFunction &f);
      
      virtual ~NEncl() {}
      
    private:
      // IEncl implementation
      virtual bool is_global_() const;
      virtual IFunction* encl_(const::core::arith::ivector &box) const;
      virtual IFunction* encl_() const;

    private:
      const IFunction& m_F;
    };

    typedef NEncl<0> IVEncl;
    typedef NEncl<1> AAFEncl;
    typedef NEncl<2> TMEncl;
  }
}

#endif /*__NENCL_HPP__*/
