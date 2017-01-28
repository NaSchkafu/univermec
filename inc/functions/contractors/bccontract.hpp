#ifndef __BCCONTRACT_HPP__
#define __BCCONTRACT_HPP__

#include <core/arithcore.hpp>
#include "functions/icontractor.hpp"


namespace functions {
  class IFunction;
  class IDerivative;

  namespace contractors
  {


    class BCContract : public IContractor
    {
    public:
      BCContract(const functions::IFunction &m_F);
      virtual ~BCContract();

    private:
      virtual bool prune_(core::Box &box, const core::arith::interval &rhs) const;
      virtual bool prune_(core::GappedBox &box, const core::arith::interval &rhs) const;
      virtual float check_box_(const core::Box &box) const;
      virtual std::string text_() const;

      /*!
      ** Applys box consistency (BC) to a function
      ** BC tries to eliminate subboxes which cannot contain a solution
      ** for F(x1...xn) = 0, where x1...xn are element of X
      **
      ** @param X original box
      ** @param F function to apply
      ** @param empty set to true if the box is completly deleted
      **
      ** @return the new reduced box if empty is not set, otherwise undefined
      */
      core::arith::ivector apply_bc(const core::arith::ivector &X, const functions::IFunction &f,
                                    bool &empty, const core::arith::interval &rhs) const;

      enum IMPROVE_BOUND_RESULT { INTERVAL_DELETED, NEW_BOUND, FINAL_BOUND, INTERNAL_ERROR };

      /*!
      ** Implementation of box consistency according to Hansen (10.2)
      ** For a function f(x1...xn)=0 BC elimantes subboxes which cannot
      ** contain a solution
      **
      ** @param X interval to improve
      ** @param F function
      ** @param ub true for the upper bound, false for lower bound
      **
      ** @return DELETED_BOX: no point in the interval is feasible
      *          NEW_BOUND: a new reduced bound has been generated
      *          FINAL_BOUND: the new generated bound satiesfies the final
      *          bound condition
      *          INTERNAL_ERROR: undefined case occured
      */
      IMPROVE_BOUND_RESULT improve_bound(core::arith::interval &x,
					 core::arith::ivector args, unsigned exp,
					 bool ub, const functions::IFunction &f, 
					 const functions::IFunction &df, const core::arith::interval &rhs) const;
      

    private:
      const functions::IFunction &m_F;
      const functions::IDerivative *m_df;

    };
  }
}


#endif /*__BCCONTRACT_HPP__*/
