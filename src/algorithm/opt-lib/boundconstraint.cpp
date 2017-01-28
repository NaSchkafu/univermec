/**
 * @file   boundconstraint.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:06:05 2009
 *
 * @brief  Implementation file for bound constraints
 *
 *
 */


#include <algorithm/opt-lib/boundconstraint.hpp>
#include <algorithm/opt-lib/simplefunction.hpp>

using namespace core::arith;

namespace GlobOpt {
  // BoundConstraint::BoundConstraint(unsigned int var, const interval &bound,
  // 				   BOUND_TYPE boundType, unsigned int dim)
  //   :m_var(var), m_bound(bound), m_boundType((boundType == GEQ) ? 1 : -1), m_dim(dim),
  //    m_grad(*(constant_func(core::arith::interval(-m_boundType, -m_boundType), m_dim))),
  //    m_zero(*(constant_func(core::arith::interval(0.0),m_dim)))
  // {
  //   assert(m_var <= m_dim);
  //   assert(m_dim > 0);
  // }

  // interval BoundConstraint::calc(const ivector &args) const
  // {
  //   return m_boundType*(m_bound - args[m_var]);
  // }

  // interval BoundConstraint::calc(const rvector &args) const
  // {
  //   return m_boundType*(m_bound - args[m_var]);
  // }

  // const C1Function& BoundConstraint::getGradF(unsigned int i) const
  // {
  //   assert(i > 0 && i <= m_dim);
  //   if(i == m_var)
  //     return m_grad;
  //   return m_zero;
  // }

  // BoundConstraint::~BoundConstraint()
  // {
  //   delete &m_grad;
  //   delete &m_zero;
  // }
}

