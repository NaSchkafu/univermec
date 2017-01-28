/**
 * @file   constraint.hpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:49:15 2009
 *
 * @brief  Header for Halfspace constraint
 *
 *
 */


#ifndef __HYPERPLANE_HPP__
#define __HYPERPLANE_HPP__

#include <core/arithcore.hpp>

namespace functions
{
  class IFunction;
  namespace encl
  {
    namespace details
    {
      /*!
      ** Creates a new hyperplane. The hyperplane splits the space in
      * two partions. So this function can be used as in/out function depending on the
      * orientation of the normal vector
      **
      ** @param normal normal vector of the plane
      ** @param offset offset to origin
      **
      ** @return pointer to hyperplane function
      */
      functions::IFunction* hyperplane(const core::arith::ivector &normal,
                                       const core::arith::interval &offset);
    }
  }
}

#endif
