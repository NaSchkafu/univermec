/**
 * @file   globalcriteria.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:16:27 2009
 *
 * @brief  Impl. of criteria
 *
 *
 */


#include <algorithm/opt-lib/globalcriteria.hpp>
#include <algorithm/opt-lib/helper.hpp>
#include <core/cxsccomp.hpp>
#include "functions/ifunction.hpp"

namespace GlobOpt
{


DONOTUSEGlobalCriteria::DONOTUSEGlobalCriteria()
  :/*m_ulp(core::arith::succ(1.0) - 1.0),*/ m_epsilonX(0.1), m_epsilonF(0.01), 
   m_epsilonP(0.01), m_epsZero(0.00001)
{ }

bool DONOTUSEGlobalCriteria::checkInterval(const core::arith::interval &i) const
{
  return core::arith::wid(i) <= m_epsilonX;
}


void DONOTUSEGlobalCriteria::setEpsX(const core::arith::mreal &epsX)
{
  m_epsilonX = epsX;
}

bool DONOTUSEGlobalCriteria::checkFunction(const core::arith::ivector &args,
					   const functions::IFunction& F) const
{
  return core::arith::wid(F(args)) <= m_epsilonF;
}

bool DONOTUSEGlobalCriteria::checkFunction(const core::arith::interval &i) const
{
  return core::arith::wid(i) <=m_epsilonF;
}

void DONOTUSEGlobalCriteria::setEpsF(const core::arith::mreal &epsF)
{
  m_epsilonF = epsF;
}
  
bool DONOTUSEGlobalCriteria::checkBox(const core::arith::ivector &box) const
{
  return MT::w(box) <= m_epsilonX;
}

bool  DONOTUSEGlobalCriteria::checkInEqConstraint(const core::arith::ivector &args,
						  const functions::IFunction &P) const
{
  return sup(P(args)) <= m_epsilonP;
}

bool  DONOTUSEGlobalCriteria::checkInEqConstraint(const core::arith::interval &i) const
{
  return sup(i) <= m_epsilonP;
}

void  DONOTUSEGlobalCriteria::setEpsP(const core::arith::mreal &epsP)
{
  m_epsilonP = epsP;
}

bool DONOTUSEGlobalCriteria::hasReduced(const core::arith::ivector &newBox,
                                              const core::arith::ivector &oldBox)
{
  // Impl von:
  // 0*25*w(oldBox) - max{w(oldBox[i]), w(newBox[i]) } <= 0
  core::arith::mreal maxRed(0.0);
  for(int i = 0; i < Ub(newBox); i++)
    maxRed = std::max(maxRed, core::arith::wid(oldBox[i]) - core::arith::wid(newBox[i]));
  return (0.25*MT::w(oldBox) - maxRed) <= 0;
}

bool DONOTUSEGlobalCriteria::isZero(const core::arith::mreal &n) const
{
  return abs(n) < m_epsZero;
}

bool DONOTUSEGlobalCriteria::isZero(const core::arith::interval &n) const
{
  return (abs(inf(n)) < m_epsZero) && (abs(sup(n)) < m_epsZero) && checkInterval(n);
}

DONOTUSEGlobalCriteria DONOTUSEGlobalCriteria::m_instance = DONOTUSEGlobalCriteria();

}

