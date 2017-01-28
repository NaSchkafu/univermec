/**
 * @file   boundconstraint.hpp
 * @author Stefan Kiel 
 * @date   Mon May 11 14:06:20 2009
 * 
 * @brief  Header for generating inequalities from bound constraints
 * 
 * 
 */


#ifndef __BOUNDCONSTRAINTS_HPP__
#define __BOUNDCONSTRAINTS_HPP__ __BOUNDCONSTRAINTS_HPP__

//#include "function.hpp"

namespace GlobOpt {

  /**
   * Generates an inequality constraint from a bound constraint
   * x >= rhs ----> rhs - x <= 0
   * x <= rhs ----> x - rhs <= 0
   */
   /// wrapper class for bound constraints
  // class BoundConstraint : public IFunction {
  // public:
  //   /// type of bound constraint
  //   enum BOUND_TYPE { LEQ, GEQ };

  //   /** 
  //    * Constructor
  //    * 
  //    * @param var number of variable
  //    * @param bound bound for variable
  //    * @param boundType type of bound
  //    * @param dim dimension of the generated constraint
  //    */
  //   BoundConstraint(unsigned int var, const core::arith::interval &bound, 
  // 		    BOUND_TYPE boundType, unsigned int dim);

  //   virtual ~BoundConstraint();

  //   virtual unsigned int dim() const { return m_dim; }
    
  // private:
  //   virtual core::arith::interval calc(const core::arith::ivector &args) const;
  //   virtual core::arith::interval calc(const core::arith::rvector &args) const;
  //   virtual const C1Function& getGradF(unsigned int i) const;

  // private:
  //   const unsigned int m_var;
  //   const core::arith::interval m_bound;
  //   const int m_boundType;
  //   const unsigned int m_dim;

  //   functions::IFunction &m_grad;
  //   functions::IFunction &m_zero;
  // };
}

#endif
