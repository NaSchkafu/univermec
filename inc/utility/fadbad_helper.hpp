/**
 * @file   fadbad_helper.hpp
 * @author Stefan Kiel
 * @date   Mon Aug  9 16:09:11 2010
 *
 * @brief
 *
 *
 */

#ifndef __FADBAD_HELPER_HPP__
#define __FADBAD_HELPER_HPP__

#include <core/arithcore.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace utility
{
  namespace details
  {
    BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_underlying_type, UnderlyingType, true)
    template <typename T>
    struct fadbad_detector_t
    {  };

    template <typename T >
    struct fadbad_detector_t<fadbad::F<T> >
    {
      typedef int fadbad_forward_t;
    };

    template <typename T>
    struct fadbad_detector_t<fadbad::B<T> >
    {
      typedef int fadbad_backward_t;
    };

    template <typename T>
    struct fadbad_detector_t<fadbad::T<T> >
    {
      typedef int fadbad_taylor_t;
    };

    BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(is_forward, fadbad_forward_t, true)
    BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(is_backward, fadbad_backward_t, true)
    BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(is_taylor, fadbad_taylor_t, true)

    template<typename X, int N>
    struct fadbad_type
    {
      typedef fadbad::F<typename fadbad_type<X, N-1>::fadbad_t > fadbad_t;
    };

    template<typename X>
    struct fadbad_type<X, 0>
    {
      typedef X fadbad_t;
    };

    template<typename X, typename Y = void, typename Z = void>
    struct strip_fadbad
    {
      typedef X real_t;
    };

    template<typename X>
    struct strip_fadbad<X, typename boost::enable_if<has_underlying_type<X> >::type,
			typename boost::enable_if<boost::mpl::or_<boost::is_same<fadbad::B<typename X::UnderlyingType>, X>,
								  boost::is_same<fadbad::T<typename X::UnderlyingType>, X> > >::type >
    {
      typedef X real_t;
    };

    template<typename X>
    struct strip_fadbad<X, typename boost::enable_if<has_underlying_type<X> >::type,
    			typename boost::disable_if<boost::mpl::or_<boost::is_same<fadbad::B<typename X::UnderlyingType>, X>,
								   boost::is_same<fadbad::T<typename X::UnderlyingType>, X> > >::type >
    {
      typedef typename strip_fadbad<typename X::UnderlyingType>::real_t real_t;
    };


    template<typename A, typename B = void>
    struct fadbad_val
    {
      static A val(const A& f)
        {
          return f;
        }
    };

    template<typename A>
    struct fadbad_val<A, typename boost::enable_if<has_underlying_type<A> >::type>
    {
      static typename strip_fadbad<A>::real_t val(const A& f)
        {
          return fadbad_val<typename A::UnderlyingType, void>::val(f.val());
        }
    };
  }

}

#endif /*__FADBAD_HELPER_HPP__*/


