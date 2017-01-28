#ifndef __PARAMNORMALS_HPP__
#define __PARAMNORMALS_HPP__


namespace functions
{
  class IVFunction;
}

namespace objects 
{
  namespace details
  {
    /** 
     * Constructs a normal function (N(x,y,z) = (f(x,y,z), N_x, N_y, N_z)) for the parametric
     * object represented by if.
     * The returned function is valid as long as ifunc is valid. Ownership is transfered
     * to the caller.
     * 
     * @param ifunc function representing the parametric object with differentiable member functions
     * 
     * @return pointer to newly created created normal function
     */
    functions::IVFunction* p_normal_fun(const functions::IVFunction &ifunc);
  }
}


#endif /*__PARAMNORMALS_HPP__*/
