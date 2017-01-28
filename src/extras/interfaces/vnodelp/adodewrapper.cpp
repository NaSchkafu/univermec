#include "extras/interfaces/vnodelp/adodewrapper.hpp"

// 
#include "core/arithcore.hpp"

#include "functions/ivfunction.hpp"
#include "functions/iffeatures.hpp"
#include "functions/itaylorcoeff.hpp"

#include "ivp/iivp.hpp"

#include "extras/interfaces/interface_exception.hpp"

namespace extras
{
  namespace interfaces
  {
    inline v_bias::interval to_v_iv(const core::arith::interval &i) 
    {
      return v_bias::interval(to_double(inf(i)), to_double(sup(i)));
    }

    inline core::arith::interval from_v_iv(const v_bias::interval &i) 
    {
      return core::arith::interval(inf(i), sup(i));
    }

    ADODEWrapper::ADODEWrapper(const ivp::IIVP &ivp)
      :m_ivp(ivp), m_size(ivp.rhs().cdim()), m_k(0)
    { 
      m_itcoeffs = ivp.rhs().features().taylor_coeff();
      assert(m_itcoeffs && "ADODEWrapper called on IVP with no ITaylorCoeff interface for the rhs.");
      if(!m_itcoeffs)
	BOOST_THROW_EXCEPTION(interface_input_exception() << 
			      core::error_details("ADODEWrapper called on IVP with no ITaylorCoeff interface for the rhs."));

      m_tcoeffs.resize(m_size);
    }

    ADODEWrapper::~ADODEWrapper() {std::cout << "AD Dtor" << std::endl;}

    void ADODEWrapper::set(const v_bias::interval&t0,const vnodelp::iVector&y0,
    		     const v_bias::interval&h,int k)
    {
      if(k != m_k)
	for(unsigned j = 0; j < m_size; j++)
	  m_tcoeffs[j].resize(k+1);

      m_t0 = t0;
      m_y0 = y0;
      m_h = h;
      m_k = k;
    }

    void ADODEWrapper::compTerms()
    {
      v_bias::interval h(1.0, 1.0);

      // IVP structure
      const auto& is = m_ivp.structure();
      unsigned p_end = std::get<0>(is) + std::get<1>(is);
      core::arith::ivector args(p_end + std::get<2>(is), core::arith::interval(0.0));
      // y0
      for(unsigned i = 0; i < std::get<0>(is); i++)
	args[i] = core::arith::interval(inf(m_y0[i]), sup(m_y0[i]));
      // p
      for(unsigned i = std::get<0>(is); i < p_end; i++)
	args[i] = m_ivp.p()[i-std::get<0>(is)];
      //args[mtl::irange(std::get<0>(is), p_end)] = m_ivp.p();
      // u
      if(m_ivp.u().dim()) {
	const core::arith::ivector &u =
	  m_ivp.u()(core::arith::ivector(1, from_v_iv(m_t0)));
	//core::arith::ivector u(8, core::arith::interval(0));
	for(unsigned i = p_end; i < p_end + std::get<2>(is); i++)
	  args[i] = u[i-p_end];
      }

      auto tcoeffs(m_itcoeffs->taylor_coeff_ode(args, m_k));
      
      for(unsigned j = 0; j <= m_k; j++) {
	for(unsigned i = 0; i < m_size; i++)
	  m_tcoeffs[i][j] = h*to_v_iv(tcoeffs[i][j]);
	h *= m_h;
      }

      // std::cout << "t0 " << m_t0 << " y0: ";
      // for(unsigned i = 0; i < m_y0.size(); i++)
      // 	std::cout << m_y0[i] << std::endl;
      // std::cout <<  " h: " << m_h << " k:" << m_k << std::endl;
      // for(unsigned i = 0; i < m_size; i++) {
      // 	std::cout << "Eq " << i << ": ";
      // 	for(unsigned j = 0; j <= m_k; j++)
      // 	  std::cout << m_tcoeffs[i][j] << "; ";
      // 	std::cout << std::endl;
      // }
    }

    void ADODEWrapper::sumTerms(v_blas::iVector&sum,int m)
    {
      v_bias::interval s;
      for(int eqn= 0;eqn<m_size;eqn++)
      {
 	s = 0.0;
     	for(int coeff= m;coeff>=0;coeff--)
     	  s+= m_tcoeffs[eqn][coeff];
	sum[eqn]= s;
      }
    }

    void ADODEWrapper::getTerm(v_blas::iVector&term,int i)const
     {
       for(unsigned j = 0; j < m_size; j++)
	 term[j] = m_tcoeffs[j][i];
     }

    v_bias::interval ADODEWrapper::getStepsize()const
     {
       return m_h;
     }

     void ADODEWrapper::eval(void* )
     {
//       for(unsigned i = 0; i < m_size; i++)
       //m_tcoeffs[i] = m_itcoeffs[i](m_y0, m_k);
     }

}
}
