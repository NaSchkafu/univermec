#include "functions/enclosures/meanvalue.hpp"

#include "functions/iffeatures.hpp"
#include "functions/iderivative.hpp"
#include "functions/ifunction.hpp"
#include "functions/details/fvbase.hpp"

namespace functions 
{
  namespace encl
  {
    namespace details
    {
      template<bool PROG>
      struct MeanValue_F 
      {
	MeanValue_F(const IFunction &f,
		    const IDerivative *df)
	  :m_f(f), m_df(df)
	  {
	  }

	template<typename T>
	typename T::value_type operator()(const T& args) const
	  {
	    T midp(mid(enclosure_cast<core::arith::ivector>(args)));
	    typename T::value_type f(m_f(midp));
	    if(PROG) {
	      T nargs(midp);
	      for(unsigned i = 0; i < size(args); i++) {
		nargs[i] = args[i];
		f += enclosure_cast<typename T::value_type>(m_df->grad(i+1)(nargs) * (args[i] - midp[i]));
	      }
	    } 
	    else {
	      T df(m_df->grad(args)[mtl::irange(1, mtl::imax)]);
	      f += enclosure_cast<typename T::value_type>(dot(df,T(args - midp)));
	    }
	    return f;
	  }

	unsigned dim() const 
	  {
	    return m_f.dim();
	  }

	std::string text() const 
	  {
	    std::stringstream sstr;
	    sstr << "MeanValueForm for " << m_f.text();
	    return sstr.str();
	  }

      private:
	const IFunction &m_f;
	const IDerivative *m_df;
      };
    }

    MeanValueForm::MeanValueForm(const IFunction &f, bool prog)
      :m_f(f), m_prog(prog)
    { }

    bool MeanValueForm::is_global_() const
    {
      return true;
    }
    
    IFunction* MeanValueForm::encl_(const::core::arith::ivector &box) const
    {
      return encl_();
    }
    
    IFunction* MeanValueForm::encl_() const
    {
      const IDerivative *df = m_f.features().derivative();
      if(df){
	if(m_prog) 
	  return new functions::details::FVBase<details::MeanValue_F<true> >(new details::MeanValue_F<true>(m_f, df));
	return new functions::details::FVBase<details::MeanValue_F<false> >(new details::MeanValue_F<false>(m_f, df));
      }
      return 0;
    }
  }
}
