#ifndef __IPARAMSURFACE_HPP__
#define __IPARAMSURFACE_HPP__


#include "objects/igeoobj.hpp"
#include "functions/ivfunction.hpp"

namespace objects 
{
  class IParamSurface : public IGeoObj
  {
  public:
    /** 
     * Gets the dimension of the parametric domain
     * 
     * 
     * @return dimension of parametric domain
     */
    inline unsigned d_dim() const 
      {
	return d_dim_();
      }
    
    /** 
     * Gets the domain of the parametric function
     * 
     * 
     * @return domain of parametric function
     */
    inline core::arith::ivector domain() const 
      {
	return domain_();
      }

    /** 
     * The vector-valued parametric function
     * 
     * 
     * @return parametric function
     */
    inline const functions::IVFunction& p_fun() const 
      {
	return p_fun_();
      }

    /** 
     * A function representing the normals of the parametric surface
     * Usually equalivalent to the gradient's cross products of p_fun()
     * If no normals are available a nullptr
     * The returned function is 
     * N(u,v) = (p_1(u,v), p_2(u,v), p_3(u,v), N_x, N_y, N_z)
     * in the 3d case.
     * If no normals are available for (u,v) only the first three components
     * are returned
     * 
     * @return nullptr or function representing surface's normals
     */
    inline const functions::IVFunction* p_normals() const 
      {
	return p_normals_();
      }

  private:
    virtual unsigned d_dim_() const = 0;
    virtual core::arith::ivector domain_() const = 0;
    virtual const functions::IVFunction& p_fun_() const = 0;
    virtual const functions::IVFunction* p_normals_() const = 0;
  };
}

#endif /*__IPARAMSURFACE_HPP__*/
