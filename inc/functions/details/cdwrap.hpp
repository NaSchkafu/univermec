#ifndef __CDWRAP_HPP__
#define __CDWRAP_HPP__


namespace functions
{
  class IFunction;
  class IVFunction;

  namespace details
  {
    /** 
     * This function can be used to map a member function of
     * IVFunction to a scalar function
     * 
     * @param iv vector-valued function containing the function to wrap
     * @param member index of member
     * 
     * @return function wrapping iv[member]
     */
    IFunction* cd_wrap(const IVFunction &iv, unsigned member);
  }
}

#endif /*__CDWRAP_HPP__*/
