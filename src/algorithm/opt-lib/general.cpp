/**
 * @file   general.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:14:56 2009
 *
 * @brief  Impl. of general routines
 *
 *
 */

#include <core/arithcore.hpp>
#include <core/cxsccomp.hpp>

// TODO C11
#include <boost/foreach.hpp>

#include <algorithm/opt-lib/general.hpp>
#include <algorithm/opt-lib/helper.hpp>
#include <algorithm/opt-lib/types.hpp>

#include "functions/ifunction.hpp"
#include "functions/iffeatures.hpp"
#include "functions/iderivative.hpp"

namespace GlobOpt {
  core::arith::ivector createGradArg(const core::arith::rvector &x, const core::arith::ivector &box, int i)
  {
    core::arith::ivector result(Ub(box));
    for(int j=0; j < Ub(box); j++)
      if(j <= i)
        result[j] = box[j];
      else
        result[j] = x[j];
    return result;
  }

  template<class FLST, class VECT>
  core::arith::ivector evalFunctions(const FLST &functions, const VECT &box)
  {
    core::arith::ivector result(functions.size());
    for(typename FLST::size_type i = 0; i < functions.size(); i++) {
      result[i] = functions[i].get()(enclosure_cast<core::arith::ivector>(box));
    }
    return result;
  }

  template<class CXLST>
  core::arith::imatrix jacobian(const core::arith::ivector &box, const core::arith::rvector &x,
                                const CXLST &functions, bool setZero, bool &err)
  {
    core::arith::imatrix J(functions.size(), functions[0].get().dim());
    bool contErr = err;
    err = false;
    for(typename CXLST::size_type i = 0; i < functions.size() && (contErr || !err); i++) {
      const functions::IDerivative *dF = functions[i].get().features().derivative();
      for(typename CXLST::size_type j = 0; j < functions[i].get().dim(); j++) {
        core::arith::ivector tmp(createGradArg(x, box, j));
        J[i][j] = dF->grad(j+1)(tmp);
        if(setZero && GC::Instance().isZero(J[i][j]))
          J[i][j] = core::arith::interval(0,0);
        else if(MT::isSpecialValue(J[i][j]))
          err = true;
      }
    }
    return J;
  }

  template<class FLST, class B>
  bool isFeasible(const B &b, const FLST &constraints)
  {
    BOOST_FOREACH(const functions::IFunction &f, constraints) {
      if(sup(f(enclosure_cast<core::arith::ivector>(b))) > 0)
        return false;
    }
    return true;
  }

  // Templateinstanziierung
  template core::arith::imatrix jacobian(const core::arith::ivector &box, const core::arith::rvector &x,
                                         const GlobOpt::FLst &functions, bool setZero, bool &err);
  template bool isFeasible(const core::arith::rvector &, const FLst &);
  template core::arith::ivector evalFunctions(const FLst &functions, const core::arith::ivector &box);
  template core::arith::ivector evalFunctions(const FLst &functions, const core::arith::rvector &box);
}
