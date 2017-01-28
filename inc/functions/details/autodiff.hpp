/**
 * @file   autodiff.hpp
 * @author Stefan Kiel 
 * @date   Fri Aug  6 11:20:30 2010
 * 
 * @brief  Some helper's for creating automatic derivatives using the forward mode of fadbad
 * 
 * 
 */

#ifndef __AUTODIFF_HPP__
#define __AUTODIFF_HPP__

#include "isscalarfunctor.hpp"
#include "utility/fadbad_helper.hpp"

namespace functions
{
  namespace details
  {
    template<typename A, int N>
    struct DFPrep
    {
      static void prepare(A &c, unsigned i, const unsigned *dfdirs, unsigned d)
        {
          if(dfdirs[N-1] == i)
            c.diff(i,d);
          DFPrep<typename A::UnderlyingType, N-1>::prepare(c.x(), i, dfdirs, d);
        }
    };

    template<typename A>
    struct DFPrep<A, 0>
    {
      static void prepare(A&, unsigned, const unsigned*, unsigned) { }
    };

    template<typename A, int N>
    struct DFEval
    {
      static typename utility::details::strip_fadbad<A>::real_t eval_ft(A& ft, const unsigned *dfdirs)
        {
          return DFEval<typename A::UnderlyingType, N-1>::eval_ft(ft.d(dfdirs[N-1]), dfdirs);
        }
    };

    template<typename A>
    struct DFEval<A, 0>
    {
      static A eval_ft(A& ft, const unsigned*)
        {
          return ft;
        }
    };

    template <typename T, typename T1>
    typename boost::disable_if_c<is_scalar_functor_t<T>::is_scalar, typename T1::value_type>::type df_eval_functor(const T& func, const T1& args, unsigned member)
    {
      return func(args)[member];
    }

    template <typename T, typename T1>
    typename boost::enable_if_c<is_scalar_functor_t<T>::is_scalar, typename T1::value_type>::type df_eval_functor(const T& func, const T1& args, unsigned )
    {
      return func(args);
    }


    template<typename A, typename X, int N>
    struct DF
    {
      typedef typename utility::details::fadbad_type<typename A::value_type, N>::fadbad_t fadbad_t;

      static typename utility::details::strip_fadbad<fadbad_t>::real_t
      eval_functor(const A& v, const X& functor, const unsigned *dirs, unsigned member)
        {
          //assert(N == sz && "size of dfdirs does not match the derivative's order!");

          unsigned d = size(v);
          mtl::dense_vector<fadbad_t> dv(v);

          for(unsigned i = 0; i < d; i++)
            DFPrep<fadbad_t, N>::prepare(dv[i], i, dirs, d);

	  fadbad_t feval(df_eval_functor(functor, dv, member));
          return DFEval<fadbad_t, N>::eval_ft(feval, dirs);
        }
    };
    
  }
}
#endif /*__AUTODIFF_HPP__*/
