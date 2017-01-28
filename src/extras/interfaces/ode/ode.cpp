#include "ode.hpp"

#include "ivp/iivp.hpp"
#include "ode-eq.hpp"



namespace extras
{
  namespace interfaces
  {
    void solve_ivp_ode(const ivp::IIVP &ivp, const core::arith::mreal &stop_)
    {
      ode::ode_problem_t::set_problem(&ivp);
      
      //double tStart=to_double(&ode::ode_problem_t::problem()->t0());//Start time of integration
      std::cout<<"ODE is active"<<std::endl;
      int IFAIL=1; //Failure flag
      int neq=ode::ode_problem_t::neq();
      double* start;start=new double;*start=to_double(ode::ode_problem_t::problem()->t0());
      double *stop;stop=new double; *stop=to_double(stop_);
      double* WORK; WORK= new double[100+21*neq]; //auxiliary variables
      double ATOL=10e-14;double RTOL=10e-14;//tolerances
      int* IWORK;IWORK=new int[5];//auxiliary variables
      double Y[neq];//state
      for(int i=0;i<neq;i++) Y[i]=to_double(forced_cast<core::arith::mreal>(ode::ode_problem_t::problem()->x0()[i]));
      //for (int i=0;i<neq;i++) std::cout<<"Y["<<i<<"]="<<Y[i]<<std::endl;
      //solver "ODE"
      ode_(&ode::ode_problem_t::problem_f,
		&neq,
		Y,start,stop,&RTOL,&ATOL,
		   &IFAIL,WORK,IWORK);
      switch (IFAIL) {
      case 2:case -2: 
		std::cout<<" integration successfull, stop="<<*stop<<", start="<<*start<<std::endl;
		for (int i=0;i<neq;i++) std::cout<<"Y["<<i<<"]="<<Y[i]<<std::endl;
		break;
      case 3:case -3: std::cout<<" integration failed, error tolerances too small, stop="<<*stop<<", start="<<*start<<std::endl;break;
      case 4:case -4: std::cout<<" integration failed, more than 500 steps are needed, stop="<<*stop<<", start="<<*start<<std::endl;break;
      case 5:case -5: std::cout<<" integration failed, equations appear to be stiff, stop="<<*stop<<", start="<<*start<<std::endl;break;
      case 6:case -6: std::cout<<" invalid input parameters (fatal error), stop="<<*stop<<", start="<<*start<<std::endl; break;
      default: std::cout<<"unknown error, IFAIL="<<IFAIL<<std::endl; break;
      }
    // std::cout<<"IFAIL="<<IFAIL<<std::endl;
    // std::cout<<"stop="<<stop<<std::endl;
     
     delete WORK; delete IWORK;delete start; delete stop;

    }
  }
}
