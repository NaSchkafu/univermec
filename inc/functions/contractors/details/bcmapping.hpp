#ifndef __BCMAPPING_HPP__
#define __BCMAPPING_HPP__

#include "stable.hpp"

namespace functions
{
  namespace contractors
  {
    namespace details 
    {
      struct BCMapping 
      {
	unsigned dim() const 
	  {
	    return 1;
	  }

	std::string text() const 
	  {
	    std::stringstream sstr;
	    sstr << "BCMapping of " << m_F.text() << " at " << m_box << " at coordinate " << m_exp;
	    return sstr.str();
	  }

	template <typename T>
	typename T::value_type operator()(const T& args) 
	  {
	    T nargs(core::arith::ArithTraits<T>::force_convert(m_box));
	    nargs[m_exp] = args[m_exp];
	    return m_F(nargs);
	  }

      private:
	function::IFunction &m_F;
	core::arith::ivector m_box;
	unsigned m_exp;
      };
    }
  }
}




#endif /*__BCMAPPING_HPP__*/
