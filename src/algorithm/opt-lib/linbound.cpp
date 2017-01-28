/**
 * @file   linbound.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:25:17 2009
 *
 * @brief  Implementation of linear bound on objective function
 *
 *
 */


#include <algorithm/opt-lib/linbound.hpp>

#include "functions/ifunction.hpp"
#include "functions/iffeatures.hpp"
#include "functions/iderivative.hpp"

#include <algorithm/opt-lib/general.hpp>
#include <algorithm/opt-lib/helper.hpp>
#include <core/box.hpp>
#include <core/cxsccomp.hpp>

namespace GlobOpt {

  bool linBound(const core::arith::rvector &x, core::arith::ivector &box, std::vector<Gap> &gaps,
		const functions::IFunction &F, const core::arith::mreal &uBound)
  {
    const functions::IDerivative* dF = F.features().derivative();
    assert(dF && "linBound requires first order informations!");

    for(unsigned int j = 0; j < F.dim(); j++) {
      core::arith::interval U(F(x));
      for(unsigned int i = 0; i < F.dim(); i++)
        if(i != j)
          U += (box[i] - x[i])*(dF->grad(i+1)(createGradArg(x, box, i)));
      U -= uBound;
      core::arith::interval V(dF->grad(j+1)(createGradArg(x, box, j)));
      // gelöst wird U + V*t <=  mit t = yj - xj
      // y ist ein Punkt in der Ursprungsbox
      const std::vector<core::arith::interval> &result = MT::solveInEq(U, V);

      if(result.size() == 1)
        box[j] &= (result[0] + x[j]);
      else {
	core::arith::interval r1((result[0] + x[j]) & box[j]);
	core::arith::interval r2((result[1] + x[j]) & box[j]);
	if(!(core::arith::isEmpty(r1) || !core::arith::isEmpty(r2))) {
	  if(inf(r1) > sup(r2))
            gaps.push_back(std::make_pair(j, core::arith::interval(inf(r2), sup(r1))));
          else
            gaps.push_back(std::make_pair(j, core::arith::interval(sup(r1), inf(r2))));
	}
        box[j] &= (x[j] + (result[0] | result[1]));
      }

      if(core::arith::isEmpty(box[j]))
        return true;
    }
    return false;
  }
}
