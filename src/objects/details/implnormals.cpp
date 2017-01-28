#include <objects/details/implnormals.hpp>

#include "functions/ifunction.hpp"
#include "functions/iffeatures.hpp"
#include "functions/iderivative.hpp"
#include "functions/ivfunction.hpp"
#include "functions/details/fvbase.hpp"


namespace objects
{
  namespace details
  {
    struct impl_normal_t 
    {
      impl_normal_t(const functions::IFunction &f)
	:m_f(f), m_df(*f.features().derivative())
	{}

      template <typename T>
      T operator()(const T& args) const
	{
	  return m_df.grad(args);
	}

      core::arith::ivector operator()(const core::arith::ivector& args) const
	{
	  // TODO: FIXME
	  // Workaround wg. fehler pow2 Funktion und SQ Normalen
	  // mittels fadbad::B
	  core::arith::ivector grad(m_df.grad(args));
	  for(unsigned i = 0; i < size(grad);i++)
	    if(is_nan(inf(grad[i])) || is_nan(sup(grad[i])))
	      return core::arith::ivector(0);
	  return grad;
	}

      unsigned dim() const
	{
	  return m_f.dim();
	}

      unsigned cdim() const
	{
	  return m_f.dim()+1;
	}

      std::string text() const
	{
	  std::stringstream sstr;
	  sstr << "Normals of implicit surface described by " << std::endl
	       << m_f.text();
	  return sstr.str();
	}

      const functions::IFunction& m_f;
      const functions::IDerivative& m_df;
    };


    functions::IVFunction* normal_fun(const functions::IFunction &ifunc)
    {
      const functions::IDerivative* dif = ifunc.features().derivative();
      if(!dif)
	return nullptr;
      // std::vector<functions::IFunction*> members(ifunc.dim());
      // for(unsigned i = 1; i <= ifunc.dim(); ++i) {
      // 	members[i-1] = (dif->grad(i).clone());
      // }
      return new functions::details::FVBase<impl_normal_t>(new impl_normal_t(ifunc));
    }
  }
}
