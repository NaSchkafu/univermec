#ifndef __IENCL_HPP__
#define __IENCL_HPP__

#include <core/arithcore.hpp>

namespace functions
{
  class IFunction;

  /// Interface for function enclosures
  /**
   * A function enclosure is, basically, a new function representation object
   * which for the underlying mathematical function. It is again represented
   * by an IVFunction instance providing inclusion function for each supported arithmetic.
   * 
   * Refer to @ref functions and Sect. 4.4 in \cite thesis for details.
   */
  class IEncl
  {
  public:
    /** 
     * Checks whether the constructed enclosure is global
     * (i.e. valid for the whole function domain)
     * 
     * @return true if the enclosure is global
     */
    inline bool is_global() const
      {
	return is_global_();
      }

    /** 
     * Computes an enclosure which is, depending on is_globel(), at
     * least valid for the given box. Ownership of the new function is
     * transfered to the caller.
     * 
     * @param box box for which the enclosure should be valid
     *      
     * @return pointer to the enclosure
     */
    inline IFunction* encl(const core::arith::ivector &box) const
      {
	return encl_(box);
      }

    /** 
     * Computes an enclosure that is valid for the whole domain The
     * function returns null if (is_global() == false) holds.
     * Ownership of the new function is transfered to the caller.
     * 
     * @return pointer to the enclosure
     */
    inline IFunction* encl() const
      {
	return encl_();
      }

  private:
    virtual bool is_global_() const = 0;
    virtual IFunction* encl_(const core::arith::ivector &box) const = 0;
    virtual IFunction* encl_() const = 0;
  };
}

#endif /*__IENCL_HPP__*/
