#include "extras/interfaces/vnodelp/dfadodewrapper.hpp"

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


    DFADODEWrapper::DFADODEWrapper(const ivp::IIVP &ivp)
      :m_ivp(ivp), m_k(0), m_size(ivp.rhs().cdim())
    {
      m_itcoeff = ivp.rhs().features().taylor_coeff();
      assert(m_itcoeff && "DFADODEWrapper called on function without ITaylorCoeff");

      // Aufbau: 1. Vektor -> Gleichungen
      // 2. Vektor: Ableitungen
      // 3. iVector: Taylorkoeffizienten
      m_tcoeffs.resize(m_size);
      for(unsigned i = 0; i < m_size; i++)
        m_tcoeffs[i].resize(m_size);

    }

    DFADODEWrapper::~DFADODEWrapper() { std::cout << "DF DTOR" << std::endl;}

    void DFADODEWrapper::set(const v_bias::interval&t0,const vnodelp::iVector&y0,
                             const v_bias::interval&h,int k)
    {
      if(k != m_k)
        for(unsigned i = 0; i < m_size; i++)
          for(unsigned j = 0; j < m_size; j++)
            m_tcoeffs[i][j].resize(k+1);
      m_t0 = t0;
      m_y0 = y0;
      m_h = h;
      m_k = k;
    }

    void DFADODEWrapper::compTerms()
    {
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
      if(m_ivp.u().dim()) {
	const core::arith::ivector &u = m_ivp.u()(core::arith::ivector(1, from_v_iv(m_t0)));
	//core::arith::ivector u(8, core::arith::interval(0));
	for(unsigned i = p_end; i < p_end + std::get<2>(is); i++)
	  args[i] = u[i-p_end];
      }

      auto tcoeffs(m_itcoeff->taylor_coeff_ode_df(args, m_k));      

      for(unsigned e = 0; e < m_size; e++ ) {
        v_bias::interval h(1.0, 1.0);
        for(unsigned j = 0; j <= m_k; j++) {
          for(unsigned i = 0; i < m_size; i++)
            m_tcoeffs[e][i][j] = h*to_v_iv(tcoeffs[e][i][j]);
          h *= m_h;
        }
      }

      // std::cout << "****************************************************************" << std::endl;
      // std::cout << "t0 " << m_t0 << " y0: ";
      // for(unsigned i = 0; i < m_y0.size(); i++)
      // 	std::cout << m_y0[i] << std::endl;
      // std::cout <<  " h: " << m_h << " k:" << m_k << std::endl;
      // for(unsigned e = 0; e < m_size; e++) {
      // 	std::cout << "Eq " << e << ": ";
      // 	for(unsigned i = 0; i < m_dim; i++) {
      // 	  std::cout << "Richtung: " << i;
      // 	  for(unsigned j = 0; j <= m_k; j++)
      // 	    std::cout << m_tcoeffs[e][i][j] << "; ";
      // 	  std::cout << endl;
      // 	}
      // 	std::cout << std::endl;
      //}
    }
    

    void DFADODEWrapper::sumTerms(vnodelp::iMatrix& Sum, int k)
    {
      core::arith::imatrix Sum2(m_size, m_size);
      for(int row= 0;row<m_size;row++)
        for(int col= 0;col<m_size;col++) {
          interval s= 0.0;

          for(int coeff= k;coeff>=1;coeff--)
	    s += m_tcoeffs[row][col][coeff];
//	    s+= tf_in[row][coeff].d(col);
          Sum[row][col]= s;
	  //Sum2[row][col] = core::arith::interval(v_bias::inf(s), v_bias::sup(s));
        }


      // std::cout << "****************************************************************" << std::endl;
      // std::cout << "t0 " << m_t0 << " y0: ";
      // for(unsigned i = 0; i < m_y0.size(); i++)
      // 	std::cout << m_y0[i] << std::endl;
      // std::cout <<  " h: " << m_h << " k:" << k << std::endl;
      // std::cout << Sum2 << std::endl;

      for(int row= 0;row<m_size;row++)
         Sum[row][row]+= 1.0;
    }

    void DFADODEWrapper::getTerm(vnodelp::iMatrix& Term,int i) const
    {
      for(int row= 0;row<m_size;row++)
        for(int col= 0;col<m_size;col++)
          Term[row][col]= m_tcoeffs[row][col][i];
    }

    void DFADODEWrapper::eval(void*) {}

  }
}
