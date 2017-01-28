#ifndef __ADODEWRAPPER_HPP__
#define __ADODEWRAPPER_HPP__

#include "core/arithcore.hpp"

#undef INTERVAL
#include "ad_ode.h"

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
    /// Implementation of the VNODE-LP AD_ODE interface
    /**
     * This class implements the AD_ODE interface of VNODE-LP and
     * allows to use functions represented by UniVerMec's IVFunction
     * interfaces in IVP problems to be solved by VNODE-LP
     * See the VNODE-LP documentation for details on the methods.
     */
    class ADODEWrapper : public vnodelp::AD_ODE
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
      explicit ADODEWrapper(const ivp::IIVP &ivp);
      virtual ~ADODEWrapper();

      virtual void set(const v_bias::interval&t0,const vnodelp::iVector&y0,
                       const v_bias::interval&h,int k);
      virtual void compTerms();
      virtual void sumTerms(vnodelp::iVector&sum,int m);
      virtual void getTerm(vnodelp::iVector&term,int i) const;
      virtual v_bias::interval getStepsize() const;
      virtual void eval(void* param);

    private:
      /// Reference to the IIVP Problem underlying this ODE object
      const ivp::IIVP& m_ivp;
      /// The ITaylorCoeff interfaces of the equation
      const functions::ITaylorCoeff* m_itcoeffs;
      /// Vector contains the Taylor coefficients of the solution
      std::vector<vnodelp::iVector> m_tcoeffs;
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

    vnodelp::AD_ODE* create_ad_ode(const ivp::IIVP &ivp)
    {
      return new ADODEWrapper(ivp);
    }

  }
}

#endif /*__ADODEWRAPPER_HPP__*/
