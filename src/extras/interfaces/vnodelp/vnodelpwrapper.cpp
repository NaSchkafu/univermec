#include "extras/interfaces/vnodelp/vnodelpwrapper.hpp"

#include "functions/ivfunction.hpp"
#include "ivp/iivp.hpp"


// Profil/BIAS filib?
#undef INTERVAL

#include "allad.h"
#include "vnodeint.h"


namespace extras
{
  namespace interfaces
  {
    // FWD Deklaration
    // Impl. in dfadodewrapper.cpp, adodewrapper.cpp
    vnodelp::AD_ODE* create_ad_ode(const ivp::IIVP &ivp);
    vnodelp::AD_VAR* create_ad_var(const ivp::IIVP &ivp);

    class MyAD : public vnodelp::AD 
    {
    public:
      MyAD(const ivp::IIVP &ivp, int order)
    	:AD(ivp.rhs().dim(), create_ad_ode(ivp), create_ad_var(ivp)), m_order(order)
    	{}

      virtual ~MyAD()
    	{
	  //std::cout << "MYAD Dtor" << std::endl;
    	  //delete tayl_coeff_ode;
    	  //delete tayl_coeff_var; 
    	}

      virtual int getMaxOrder() const
    	{
    	  return m_order;
    	}

    private:
      int m_order;
    };

    inline v_bias::interval to_v_iv(const core::arith::interval &i) 
    {
      return v_bias::interval(to_double(inf(i)), to_double(sup(i)));
    }

    inline core::arith::interval from_v_iv(const v_bias::interval &i) 
    {
      return core::arith::interval(inf(i), sup(i));
    }
    

    core::arith::ivector solve_ivp_vnodelp(const ivp::IIVP &ivp, const core::arith::mreal &stop, 
					   std::vector<core::arith::ivector> &sresults,
					   std::ostream &os, 
					   core::arith::mreal output_step, const ivp_callback_t *cb,
					   bool cparams)
    {
      std::unique_ptr<MyAD> ad = make_unique<MyAD>(ivp, MAXORDER);

      unsigned dim = size(ivp.x0());
      iVector v(dim);
      os <<  to_double(ivp.t0()) <<" ";
      for(unsigned i = 0; i < dim; i++) {
	v[i] = to_v_iv(ivp.x0()[i]);
	os <<  inf(ivp.x0()[i]) << " " << sup(ivp.x0()[i]) << " ";
      }
      os << std::endl;

      v_bias::interval i_start(to_double(ivp.t0()), to_double(ivp.t0()));
      v_bias::interval i_stop(to_double(ivp.t0()+output_step), to_double(ivp.t0()+output_step));
      if(output_step == -1.0) {
	i_stop = v_bias::interval(to_double(stop), to_double(stop));
	output_step = 1.0;
      }
      assert(output_step > 0 && "output_step must be > 0");

       //std::cout << i_start << " " << i_stop << std::endl;

      std::unique_ptr<vnodelp::VNODE> solver = make_unique<vnodelp::VNODE>(ad.get());
      //solver->setHmin(1.0);

       do {
	 solver->integrate(i_start, v, i_stop);
	 os << midpoint(i_start) << " ";
	 core::arith::ivector sres(dim);
	 for(unsigned i = 0; i < dim; i++) {
	   os << inf(v[i]) << " " << sup(v[i]) << " ";
	   sres[i] = core::arith::interval(inf(v[i]), sup(v[i]));
	 }
	 sresults.push_back(sres);
	 
	 os << std::endl;
	 // Callback aufrufen
	 if(cb) {
	   core::arith::ivector v_tmp(dim);
	   for(unsigned i = 0; i < dim; i++)
	     v_tmp[i] = from_v_iv(v[i]);
	   core::arith::interval t(from_v_iv(i_stop));
	   core::arith::interval t_stop(inf(t)+output_step, sup(t)+output_step);
	   (*cb)(v_tmp, t, t_stop);
	   for(unsigned i = 0; i < dim; i++)
	     v[i] = to_v_iv(v_tmp[i]);
	 }
	 i_start = i_stop;
	 i_stop = v_bias::interval(inf(i_stop)+to_double(output_step), sup(i_stop)+to_double(output_step));
	 if(cparams)
	   solver->setFirstEntry();
       } while((sup(i_stop) < stop) && solver->successful());

       // Keine Lsg
       if(!solver->successful())
	 return core::arith::ivector(0);

       // Lsg
       core::arith::ivector r(dim);
       for(unsigned i = 0; i < dim; i++)
	 r[i] = from_v_iv(v[i]);
       return r;
    }
  }
}
