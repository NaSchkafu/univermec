// -*-c++-*-

#include "ivp/iivp.hpp"
#include "functions/ivfunction.hpp"

namespace vode
{
  /// This structure holds the problem currently being solved by vode.f
  struct vode_problem_t
  {
    inline static const ivp::IIVP* problem()
      {
        return S_PROBLEM;
      }

    inline static void set_problem(const ivp::IIVP *p)
      {
        S_PROBLEM = p;
        auto d(S_PROBLEM->structure());
        NEQ = std::get<0>(d);
        P_DIM = std::get<1>(d);
	U_DIM = std::get<2>(d);
        S_DIM =  U_DIM + NEQ + P_DIM;
      }

    inline static unsigned dim()
      {
        return S_DIM;
      }

    inline static unsigned neq()
      {
        return NEQ;
      }


    inline static core::arith::mreal stop_time()
      {
	return S_STOP_TIME;
      }


    inline static void set_stop_time(const core::arith::mreal &stop_time)
      {
	S_STOP_TIME = stop_time;
      }

   inline static void problem_f(int* n_, double* t, double* y, double* yp, double* rpar, int* ipar)
      {
	//std::cout<<"bin in problem f"<<std::endl;
	int n=dim();	
        assert(*n_==NEQ);
       // std::cout<<"bin in problem f "<<n<<std::endl;
	core::arith::rvector arg(n), out(S_PROBLEM->rhs().cdim());
	for(int i=0;i<neq();i++) arg[i]=y[i];
	int j=0;
	for(int i=neq();i<neq()+P_DIM;i++) arg[i]=to_double(forced_cast<core::arith::mreal>(S_PROBLEM->p()[j++]));
        typename core::arith::rvector t_(1, *t);
	j=0;
	if(U_DIM!=0) for(int i=neq()+P_DIM;i<n;i++) arg[i]=to_double(forced_cast<core::arith::mreal>(S_PROBLEM->u()(t_)[j++])); //std::cout<<"u[0]="<<arg[neq()+P_DIM+3]<<std::endl;
	out=S_PROBLEM->rhs()(arg);
	for(int i=0;i<S_PROBLEM->rhs().cdim();i++) yp[i]=to_double(out[i]);

	
      }
   inline static void problem_jac(double* t, double* y, double* yp)
      {
	
      }
    
    
  private:
    static const ivp::IIVP* S_PROBLEM;
    static unsigned S_DIM;
    static unsigned P_DIM;
    static unsigned U_DIM;
    static unsigned NEQ;
    static core::arith::mreal S_STOP_TIME;
   
  };
}


