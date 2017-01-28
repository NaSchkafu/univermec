#ifndef __IDISTOBSERVER_HPP__
#define __IDISTOBSERVER_HPP__

#include <core/arithcore.hpp>

namespace algorithm
{
  /// Observer interface for distance algorithms
  class IDistObserver
  {
  public:
    IDistObserver() { }
    virtual ~IDistObserver() { }

    inline void lb(const core::arith::ivector &b1, const core::arith::ivector &b2, 
		   core::arith::mreal lb) const
      {
	lb_(b1, b2, lb);
      }
    
    inline void ub(const core::arith::ivector &b1, const core::arith::ivector &b2, 
		   core::arith::mreal ub) const 
      {
	ub_(b1, b2, ub);
      }

    /** 
     * Algorithm status
     * 
     * @param it number of iterates
     * @param el elements on working
     * @param fl elements on a final list
     * 
     * @return return true for signaling a user abort to the algorithm
     */
    inline bool status(unsigned it, unsigned el, unsigned fl) const 
      {
	return status_(it, el, fl);
      }

    inline void distance(const core::arith::interval &encl) const
      {
	distance_(encl);
      }

    // FIXME: Die Methoden werden sichtbar gemacht, damit sie von SWIG aus zugreifbar sind
    // und die Klasse von Python aus ableitbar ist.
    virtual void lb_(const core::arith::ivector &b1, const core::arith::ivector &b2, 
			    core::arith::mreal lb) const = 0;
    virtual void ub_(const core::arith::ivector &b1, const core::arith::ivector &b2, 
			    core::arith::mreal ub) const = 0;
    virtual bool status_(unsigned it, unsigned el, unsigned fl) const = 0;
    virtual void distance_(const core::arith::interval &encl) const = 0;


  private:
    IDistObserver(const IDistObserver&);
    IDistObserver& operator=(const IDistObserver&);
// #ifndef SWIG
//     virtual void lb_(const core::arith::ivector &b1, const core::arith::ivector &b2, 
// 			    core::arith::mreal lb) const = 0;
//     virtual void ub_(const core::arith::ivector &b1, const core::arith::ivector &b2, 
// 			    core::arith::mreal ub) const = 0;
//     virtual bool status_(unsigned it, unsigned el, unsigned fl) const = 0;
//     virtual void distance_(const core::arith::interval &encl) const = 0;
// #endif
  };
}

#endif /*__IDISTOBSERVER_HPP__*/
