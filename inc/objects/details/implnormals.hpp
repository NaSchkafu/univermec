#ifndef __IMPLNORMALS_HPP__
#define __IMPLNORMALS_HPP__

namespace functions
{
  class IFunction;
  class IVFunction;
}

namespace objects 
{
  namespace details
  {
    /** 
     * Constructs a normal function (N(x,y,z) = (f(x,y,z), N_x, N_y, N_z)) for the implicit
     * object represented by ifunc.
     * The returned function is valid as long as ifunc is valid. Ownership is transfered
     * to the caller.
     * 
     * @param ifunc differentiable function representing the implicit object
     * 
     * @return pointer to newly created created normal function
     */
    functions::IVFunction* normal_fun(const functions::IFunction &ifunc);
  }
}

#endif /*__IMPLNORMALS_HPP__*/
