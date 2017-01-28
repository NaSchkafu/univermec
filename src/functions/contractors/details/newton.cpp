/**
 * @file   newton1d.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:26:44 2009
 *
 * @brief  Impl. of 1D newton operator
 *
 *
 */


#include <functions/contractors/details/newton.hpp>
#include <core/cxsccomp.hpp>

namespace functions {
  namespace contractors
  {
    namespace details
    {

      std::vector<core::arith::interval> stepNwt1D(const core::arith::mreal &xn, const core::arith::interval &fxn,
                                                   const core::arith::interval &dfx, const core::arith::interval &x)
      {
        if(0.0 <= dfx)
          return extStepNwt1D(xn, fxn , dfx, x);
        std::vector<core::arith::interval> result;
        result.push_back(simpleStepNwt1D(xn, fxn, dfx));
        return result;
      }

      core::arith::interval simpleStepNwt1D(const core::arith::mreal &xn, const core::arith::interval &fxn,
                                            const core::arith::interval &dfx)
      {
        if(0.0 <= dfx)
          assert(!"grad(xn) contains 0");
        core::arith::interval r(xn - fxn/dfx);
        return r;
      }

      std::vector<core::arith::interval> extStepNwt1D(const core::arith::mreal &xn, const core::arith::interval &fI,
                                                      const core::arith::interval &fD, const core::arith::interval &x)
      {
        // core::arith::interval fI = F(xn);
        // core::arith::interval fD = F.grad(x);
        std::vector<core::arith::interval> result;

        if((0.0 <= fI) || !(0.0 <= fD)) {
          assert(!"F.grad(x) contains not a zero or F(xn) contains a zero");
        }
        else if(inf(fI) > 0.0){
          core::arith::mreal pn(xn - (inf(fI)/inf(fD)));
          core::arith::mreal qn(xn - (inf(fI)/sup(fD)));
          if(inf(fD) == 0.0) {
            result.push_back(core::arith::interval(-core::arith::infinity(), qn));
            result.push_back(core::arith::interval(core::arith::infinity(), core::arith::infinity()));
          }
          else if(sup(fD) == 0.0) {
            result.push_back(core::arith::interval(pn, core::arith::infinity()));
            result.push_back(core::arith::interval(-core::arith::infinity(), -core::arith::infinity()));
          }
          else if(inf(fD) < 0.0 && sup(fD) > 0.0) {
            result.push_back(core::arith::interval(-core::arith::infinity(), qn));
            result.push_back(core::arith::interval(pn, core::arith::infinity()));
          }
        }
        else if(sup(fI) < 0.0) {
          core::arith::mreal pn = xn - (sup(fI)/inf(fD));
          core::arith::mreal qn = xn - (sup(fI)/sup(fD));
          if(inf(fD) == 0.0) {
            result.push_back(core::arith::interval(qn, core::arith::infinity()));
            result.push_back(core::arith::interval(-core::arith::infinity(), -core::arith::infinity()));
          }
          else if(sup(fD) == 0.0) {
            result.push_back(core::arith::interval(-core::arith::infinity(), pn) & x);
            result.push_back(core::arith::interval(core::arith::infinity(), core::arith::infinity()));
          }
          else if(inf(fD) < 0.0 && sup(fD) > 0.0) {
            result.push_back(core::arith::interval(-core::arith::infinity(), pn));
            result.push_back(core::arith::interval(qn, core::arith::infinity()));
          }
        }
        std::vector<core::arith::interval>::iterator it = result.begin();
        while(it != result.end()) {
          if(core::arith::isEmpty(*it))
            it = result.erase(it);
          else {
            ++it;
          }
        }
        return result;
      }
    }
  }
}
