
#include "ivp/details/dummyu.hpp"

#include "functions/details/fjoin.hpp"
#include "functions/details/fderiv.hpp"

namespace ivp
{
  namespace details
  {

    struct dummyu_t 
    {
      template <typename T>
      T operator()(const T& args) const
	{
	  return args;
	}

      unsigned dim() const
	{
	  return 0;
	}

      unsigned cdim() const
	{
	  return 0;
	}
      
      std::string text() const
	{
	  return "DummyU Function";
	}
    };

    functions::IVFunction* create_dummyu()
    {
      return new functions::details::FJoin<dummyu_t, functions::details::FDeriv<dummyu_t> >;
    }

  }
}
