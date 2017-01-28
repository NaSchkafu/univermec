#ifndef __DFADODEWRAPPER_HPP__
#define __DFADODEWRAPPER_HPP__

#include "core/arithcore.hpp"

#undef INTERVAL
#include "ad_var.h"

namespace functions
{
  class ITaylorCoeff;
}

namespace ivp
{
  class IIVP;
}

namespace extras
{
  namespace interfaces
  {
    class DFADODEWrapper : public vnodelp::AD_VAR
    {
    public:
      /**
       * Constructs a new ADODEWrapper object
       *
       * @param ivp reference to the underlying IVP problem
       * Throws an interface_input_exception() if the right-hand side
       * of the problem does not support the ITaylorCoeff interface
       *
       */
      explicit DFADODEWrapper(const ivp::IIVP &ivp);
      virtual ~DFADODEWrapper();

      virtual void set(const v_bias::interval&t0,const vnodelp::iVector&y0,
                       const v_bias::interval&h,int k);
      virtual void compTerms();
      virtual void sumTerms(vnodelp::iMatrix&sum,int m);
      virtual void getTerm(vnodelp::iMatrix&term,int i)const;
      virtual void eval(void*param);

    private:
      /// Reference to the IIVP Problem underlying this ODE object
      const ivp::IIVP& m_ivp;
      /// The ITaylorCoeff interfaces of the equation
      const functions::ITaylorCoeff* m_itcoeff;
      /// Vector contains the Taylor coefficients of the solution
      std::vector<std::vector<vnodelp::iVector> > m_tcoeffs;
      /// Time
      v_bias::interval m_t0;
      /// Expansion point
      vnodelp::iVector m_y0;
      /// Stepsize
      v_bias::interval m_h;
      /// Order of Taylor expansion
      int m_k;
      /// Number of equations
      unsigned m_size;
    };

    vnodelp::AD_VAR* create_ad_var(const ivp::IIVP &ivp)
    {
      auto w(new DFADODEWrapper(ivp));
      // vnodelp::iVector y0(3);
      // y0[0] = v_bias::interval(15);
      // y0[1] = v_bias::interval(15);
      // y0[2] = v_bias::interval(36);
      // v_bias::interval h(1.0);
      // w->set(v_bias::interval(0,0), y0, h, 10);
      // w->compTerms();
      return w;
    }

  }
}

#endif /*__DFADODEWRAPPER_HPP__*/
