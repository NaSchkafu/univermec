#ifndef __GAPPEDBOX_HPP__
#define __GAPPEDBOX_HPP__

#include "box.hpp"

namespace core 
{
  /// A gap is an interval and a coordinate direction
  typedef std::pair<arith::interval, unsigned short> Gap;

  class GappedBox : public Box 
  {
  public:
    GappedBox(const arith::ivector &x)
      :Box(x)
      {}

    GappedBox()
      {}

    typedef std::deque<Gap>::iterator gap_iterator_t;
    typedef std::deque<Gap>::const_iterator const_gap_iterator_t;

    virtual ~GappedBox()
      {}

    bool add_gap_(const Gap& g) 
      {
	m_gaps.push_back(g);
	return true;
	//push_heap(g.begin(), g.end());
      }

    gap_iterator_t begin()
      {
	return m_gaps.begin();
      }

    gap_iterator_t end()
      {
	return m_gaps.end();
      }
    
    const_gap_iterator_t begin() const
      {
	return m_gaps.begin();
      }

    const_gap_iterator_t end() const
      {
	return m_gaps.end();
      }

    core::size_t gaps() const
      {
	return m_gaps.size();
      }

    Gap gap(core::size_t i)
      {
	return m_gaps.at(i);
      }

  private:
    std::deque<Gap> m_gaps;
  };
}

#endif /*__GAPPEDBOX_HPP__*/
