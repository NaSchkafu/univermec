#ifndef __CONVERSION_HPP__
#define __CONVERSION_HPP__

#include <boost/mpl/has_xxx.hpp>
#include <cassert>

#include <core/core_exception.hpp>

namespace core 
{
  namespace arith 
  {

    namespace details 
    {
      BOOST_MPL_HAS_XXX_TRAIT_DEF(arith_type_t)

      template<typename X, typename Y, typename T = void>
      struct LosslessCastImpl
      {
	static X cast(const Y& y)
	  {
	    assert(!"Downcast error!");
	    BOOST_THROW_EXCEPTION(conv_exception() << error_details("Downcast requested!"));
	    return X(0.0);
	  }
      };

      /// \todo Sicherheitscheck einbauen falls X, Y tatsächlich Vektor/Matrixtypen sind
      template<typename X, typename Y>
      struct LosslessCastImpl<X, Y, typename boost::enable_if<boost::mpl::or_<boost::is_same<typename ArithTraits<Y>::base_t, mreal>,
									     boost::mpl::and_<boost::is_same<typename ArithTraits<Y>::base_t, interval>,
											      boost::mpl::not_<boost::is_same<typename ArithTraits<X>::base_t, mreal> > > > >::type>
      {
	static X cast(const Y& y)
	  {
	    return core::arith::ArithTraits<X>::convert(y);
	  }
      };

      /// Non specialized implementation of an enclosure cast, issueing an error
      template<typename X, typename Y, typename T = void>
      struct EnclosureCastImpl
      {
	static X cast(const Y& y)
	  {
	    assert(!"Downcast error");
	    BOOST_THROW_EXCEPTION(conv_exception() << error_details("Downcast requested!"));
	    return X(0.0);
	  }
      };
      
      // Fuer Check s.o.
      /// Specialized implementation of an enclosure cast performing the actual cast
      template <typename X, typename Y>
      struct EnclosureCastImpl<X, Y, typename boost::enable_if<boost::mpl::or_<boost::is_same<typename ArithTraits<X>::base_t, typename ArithTraits<Y>::base_t>,
									       boost::mpl::not_<boost::is_same<typename ArithTraits<X>::base_t, mreal> > > >::type >
      {
	static X cast(const Y& y)
	  {
	    return core::arith::ArithTraits<X>::convert(y);
	  }
      };

      template<typename X, typename Y>
      struct ForcedCastImpl
      {
	static X cast(const Y& y)
	  {
	    return core::arith::ArithTraits<X>::convert(y);
	  }
      };

    }

    template <typename X, typename Y>
    X lossless_cast(const Y& y)
    {
      return details::LosslessCastImpl<X,Y>::cast(y);
    }

    template<typename X, typename Y>
    X enclosure_cast(const Y& y)
    {
      return details::EnclosureCastImpl<X,Y>::cast(y);
    }

    template <typename X, typename Y>
    X forced_cast(const Y& y)
    {
      return details::ForcedCastImpl<X,Y>::cast(y);
    }
  }
}

using core::arith::enclosure_cast;
using core::arith::forced_cast;
using core::arith::lossless_cast;

#endif /*__CONVERSION_HPP__*/
