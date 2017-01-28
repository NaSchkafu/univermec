/**
 * @file   types.hpp
 * @author Stefan Kiel 
 * @date   Mon May 11 14:31:37 2009
 * 
 * @brief  Type definitions
 * 
 * 
 */


#ifndef __TYPES_HPP__
#define __TYPES_HPP__ __TYPES_HPP__

#include <vector>
#include <boost/ref.hpp>
#include <utility>
#include <core/arithcore.hpp>

namespace functions
{
  class IFunction;
}

namespace GlobOpt 
{
  /// a gap
  /**
   * A gap is a pair of the coordinate direction and the gap interval
   */
  typedef std::pair<unsigned int, core::arith::interval> Gap;

  typedef std::vector<boost::reference_wrapper<functions::IFunction> > FLst;



}

#endif
