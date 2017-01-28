#ifndef __SUPERQUADRIC_HPP__
#define __SUPERQUADRIC_HPP__ 

#include <core/arithcore.hpp>

namespace functions 
{
  class IFunction;
  class IVFunction;
}

namespace shapes 
{
  /*! 
  ** Creates an implicit representation of a superquadric
  ** Only ellipsoids!
  **
  ** 
  ** @param rx x scaling
  ** @param ry y scaling
  ** @param rz z scaling
  ** @param eps1 east-west roundness 
  ** @param eps2 north-south roundness
  ** @param imp_aaf improved aaf handling (squares the input forms as intervals for ensuring a positive range) 
  * 
  ** @return pointer to implicit function
  */
  functions::IFunction* sq(core::arith::mreal rx, core::arith::mreal ry, core::arith::mreal rz, int eps1_p, unsigned eps1_q, 
			   int eps2_p, unsigned eps2_q);

  /** 
   * Creates a parametric representation of a superquadric's surface
   * 
   * @param rx 
   * @param ry 
   * @param rz 
   * @param eps1_p 
   * @param eps1_q 
   * @param eps2_p 
   * @param eps2_q 
   * 
   * @return pointer to parametric functions
   */
  functions::IVFunction* param_sq(core::arith::mreal rx, core::arith::mreal ry, core::arith::mreal rz, int eps1_p, unsigned eps1_q,
                                  int eps2_p, unsigned eps2_q);

  functions::IVFunction* param_sq_normals(const functions::IVFunction &sq);

  /** 
   ** Creates an implicit representation of a bended superquadric
   ** Only ellipsoids!
   **
   ** @param rx x scaling
   ** @param ry y scaling
   ** @param rz z scaling
   ** @param eps1 east-west roundness 
   ** @param eps2 north-south roundness
   ** @param kbend 
   ** @param alpha 
   ** @param imp_aaf improved aaf handling (squares the input forms as intervals for ensuring a positive range) 
   ** 
   ** @return pointer to implicit function
   */
  functions::IFunction* sq_bended(core::arith::mreal rx, core::arith::mreal ry, core::arith::mreal rz, int eps1_p, unsigned eps1_q,
				  int eps2_p, unsigned eps2_q, core::arith::mreal kbend, core::arith::mreal alpha);

  bool get_sq_params(const functions::IFunction &sq, core::arith::rvector &scale, 
		     int &eps1_p,unsigned &eps1_q, int &eps2_p, unsigned &eps2_q);

}


#endif /*__SUPERQUADRIC_HPP__*/
