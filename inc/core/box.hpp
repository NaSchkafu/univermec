#ifndef __IBOX_HPP__
#define __IBOX_HPP__

#include <core/arithcore.hpp>

namespace core
{
  /// Lightweight class representing a box
  /**
   * This class represents a (multidimensional). Class like treenodes etc. should derive from this
   * class. In order to reduce overhead this is purposely not an interface.
   * 
   */
  class Box 
  {
  public:
    virtual ~Box();

    explicit Box(const core::arith::ivector& x)
      :m_x(x) { }

    Box()
      {}

    inline const core::arith::ivector& X() const
      {
	return m_x;
      }

    inline core::arith::ivector& ref_X()
      {
	return m_x;
      }

    inline const core::arith::ivector& ref_X() const
      {
	return m_x;
      }

    inline core::arith::ivector c_X()
      {
	return m_x;
      }

    inline unsigned dim() const
      {
	return size(m_x);
      }

    /** 
     * Bisects this box generating at coordinate dir
     * Generating two boxes with X[dir] = [inf(X[dir]), inf(p)] and [sup(p), sup(X[dir])]
     * 
     * @param dir coordinate
     * @param p splitting point
     * 
     * @return second box generated
     */
    virtual Box* bisect(unsigned dir, const arith::interval &p);

  protected:
    core::arith::ivector m_x;
  };
}

#endif /*__IBOX_HPP__*/
