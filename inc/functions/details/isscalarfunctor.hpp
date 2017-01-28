#ifndef __IS_SCALAR_HPP__
#define __IS_SCALAR_HPP__

namespace functions
{
  namespace details
  {
    template<typename T>
    struct is_scalar_functor_t
    {
      template<typename U, core::arith::interval (U::*)(const core::arith::ivector&) const> struct SFINAE {};
      template<typename U> static char Test(SFINAE<U, &U::operator()>*);
      template<typename U> static int Test(...);
      static const bool is_scalar = sizeof(Test<T>(0)) == sizeof(char);

      typedef typename boost::mpl::if_<boost::mpl::bool_<is_scalar>, functions::IFunction, functions::IVFunction>::type functor_t;

    };

    template <typename T, typename T1>
    typename boost::enable_if_c<is_scalar_functor_t<T>::is_scalar, T1>::type eval_functor_1(const T& func, const T1& args)
    {
      return T1(1, func(args));
    }

    template <typename T, typename T1>
    typename boost::disable_if_c<is_scalar_functor_t<T>::is_scalar, T1>::type eval_functor_1(const T& func, const T1& args)
    {
      return func(args);
    }

    template <typename T, typename T1>
    typename boost::enable_if_c<is_scalar_functor_t<T>::is_scalar, typename T1::value_type>::type eval_functor_member(const T& func, const T1& args, unsigned)
    {
      return func(args);
    }

    template <typename T, typename T1>
    typename boost::disable_if_c<is_scalar_functor_t<T>::is_scalar, typename T1::value_type>::type eval_functor_member(const T& func, const T1& args, unsigned member)
    {
      return func(args)[member];
    }

    
  }
}

#endif /*__IS_SCALAR_HPP__*/
