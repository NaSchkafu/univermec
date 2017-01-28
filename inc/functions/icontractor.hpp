#ifndef __ICONTRACTOR_HPP__
#define __ICONTRACTOR_HPP__

#include "core/box.hpp"
#include "core/gappedbox.hpp"

namespace functions
{

  class IFunction;
  class IEncl;

  /// Represents an interval contractor inside the framework
  /** 
   * Represents an interval contractor inside the framework.
   * Formally, a contractor C is an algorihm taking a function 
   * \f$f: \Rz^n \to \Rz\f$ and 
   * \f$\x \in \IRz^n, \ci \in \IRz\f$ as arguments and
   * returning an interval \f$\x^{(1)}\f$ such that \f$\x^{(1)}
   * \subseteq \x\f$ and \f$\forall x \in \x \setminus \x^{(1)} :
   * \lnot \exists c \in \ci: f(x) = c\f$ holds.  Loosely spoken, it
   * removes parts from \f$\x\f$ which do not solve the equation
   * \f$f(x) =c \f$.
   * 
   * A concrete IContractor instance is always associated to a function 
   * f. Therefore, only the box \f$\x\f$ to prune and the right-hand side \f$\ci\f$
   * are taken as arguments in prune function of the interface. Because IContractor
   * encapsulates the concrete contraction algorithm is an abstract form which can
   * be passed to algorithms inside the framework (strategy pattern in \cite gamma1995) 
   * it can be used to implement a form of contractor programming as described in
   * \cite Chabert2009.
   * 
   * Refer to Sect. 4.3 and 4.4 in \cite thesis for details on the implementation and
   * to \cite Benhamou1996 for details on contractors.
   */
  class IContractor
  {
  public:
    virtual ~IContractor() = default;

     /**
     * Tries to contract the box using the relation f(x) = rhs
     * 
     * @param box box x
     * @param rhs right hand side of equation
     * 
     * @return true if the box is not consistent
     */
    inline bool prune(core::Box &box, const core::arith::interval &rhs) const
      {
        return prune_(box, rhs);
      }

    /** 
     * Tries to contract the box using the relation f(x) = rhs
     * 
     * @param box box x
     * @param rhs right hand side of equation
     * 
     * @return true if the box is not consistent
     */
    inline bool prune(core::GappedBox &box, const core::arith::interval &rhs) const
      {
        return prune_(box, rhs);
      }

    /** 
     * Gets a description of this contractor
     * 
     * 
     * @return textual description
     */
    inline std::string text()
      {
    	return text_();
      }

    /** 
     * Checks how well the contractor is suited for the box
     * The box is not suited for the box if 0.0 is returned
     * 
     * @param box box
     * 
     * @return 0.0 <= return <= 1.0 
     */
    inline float check_box(const core::Box &box) const
      {
    	return check_box_(box);
      }

  private:
    virtual bool prune_(core::Box &box, const core::arith::interval &rhs) const = 0;
    virtual bool prune_(core::GappedBox &box, const core::arith::interval &rhs) const = 0;
    virtual float check_box_(const core::Box &box) const = 0;
    virtual std::string text_() const = 0;
  };

  class IFreeContractor 
  {
  public:
    virtual ~IFreeContractor() = default;

    /** 
     * Tries to contract the box using the relation f(x) = rhs
     * 
     * @param box box x
     * @param rhs right hand side of equation
     * @param f function f
     * @param encl enclosure for f (optional)

     * @return true if the box is not consistent
     */
    inline bool prune(core::Box &box, const core::arith::interval &rhs, 
		      const IFunction &f, const IEncl *encl = 0) const
      {
        return prune_(box, rhs, f, encl);
      }

    /** 
     * Tries to contract the box using the relation f(x) = rhs
     * 
     * @param box box x
     * @param rhs right hand side of equation
     * @param f function f
     * @param encl enclosure for f (optional)
     * 
     * @return true if the box is not consistent
     */
    inline bool prune(core::GappedBox &box, const core::arith::interval &rhs, 
		      const IFunction &f, const IEncl *encl = 0) const
      {
        return prune_(box, rhs, f, encl);
      }

    inline std::string text() const
      {
	return text_();
      }

    /** 
     * Checks how well the contractor is suited for the function
     * The contractor is not suited for the function if 0.0 is returned
     * 
     * @param f function
     * 
     * @return 0.0 <= return <= 1.0 
     */
    inline float check_func(const IFunction &f) const
      { 
	return check_func_(f);
      }

    /** 
     * Checks how well the contractor is suited for the box
     * The box is not suited for the box if 0.0 is returned
     * 
     * @param box box
     * @param f function
     * 
     * @return 0.0 <= return <= 1.0 
     */
    inline float check_box(const core::Box& box, const IFunction &f) const
      {
	return check_box(box, f);
      }


  private:
    virtual bool prune_(core::Box &box, const core::arith::interval &rhs, 
			const IFunction &f, const IEncl* encl) const = 0;
    virtual bool prune_(core::GappedBox &box, const core::arith::interval &rhs, 
			const IFunction &f, const IEncl* encl) const = 0;
    virtual std::string text_() const = 0;
    virtual float check_func_(const IFunction &f) const = 0;
    virtual float check_box_(const IFunction &f) const = 0;
  };

}


#endif /*__ICONTRACTOR_HPP__*/
