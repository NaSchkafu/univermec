#include "vode.hpp"

#include "ivp/iivp.hpp"
#include "vode-eq.hpp"



namespace extras
{
  namespace interfaces
  {
    void solve_ivp_vode(const ivp::IIVP &ivp, const core::arith::mreal &stop_, std::ostream &os, const core::arith::mreal &output_step)
    {
      vode::vode_problem_t::set_problem(&ivp);
      
      std::cout<<"DVODE is active"<<std::endl;
      int neq=vode::vode_problem_t::neq();
      double* start;start=new double;*start=to_double(vode::vode_problem_t::problem()->t0());
      double *stop;stop=new double; *stop=to_double(stop_);
      double Y[neq];//state
      for(int i=0;i<neq;i++) {Y[i]=to_double(forced_cast<core::arith::mreal>(vode::vode_problem_t::problem()->x0()[i]));std::cout<<"Y[i]="<<Y[i]<<std::endl;}

      //vode settings
      int ISTATE=1;
      int ITOL=1;
      double ATOL=10e-14;double RTOL=10e-14;//tolerances
      int MF=22;//stiff, automatically generated Jacobian
      int ITASK=1;
      int IOPT=0;
      int LRW=22 +  9*neq + 2*neq*neq;
      double* WORK; WORK= new double[LRW];
      int LIW=30 + neq;
      int* IWORK;IWORK=new int[LIW];//auxiliary variables
      //for (int i=0;i<neq;i++) std::cout<<"Y["<<i<<"]="<<Y[i]<<std::endl;
      //solver "vode"
      double stepsize=to_double(output_step);
      int nsteps=*stop/stepsize+1;
      double* tout;tout=new double; *tout=stepsize;    
      os<<*start<<"   ";for (int i=0;i<neq;i++) os<<Y[i]<<"   "; os<<std::endl;
      for(int i=1;i<=nsteps;i++){
    	      dvode_(&vode::vode_problem_t::problem_f,
			&neq,
			Y,start,tout,&ITOL,&RTOL,&ATOL,&ITASK, &ISTATE,&IOPT,WORK,&LRW,IWORK,&LIW,
	           	   &vode::vode_problem_t::problem_jac,&MF,NULL,NULL);     	
      		switch (ISTATE) {
      		case 2: 
			//std::cout<<" integration successfull, stop="<<*stop<<", start="<<*tout<<std::endl;
			os<<*tout<<"   ";
			for (int i=0;i<neq;i++) os<<Y[i]<<"   "; os<<std::endl;
		break;
      		case -1: std::cout<<" integration failed, excess work done on this call, stop="<<*stop<<", start="<<*start<<std::endl;return;;
      		case -2: std::cout<<" integration failed, excess accuracy requested, stop="<<*stop<<", start="<<*start<<std::endl;return;;
      		case -3: std::cout<<" integration failed, illegal input detected, stop="<<*stop<<", start="<<*start<<std::endl;return;;
      		case -4: std::cout<<" integration failed, repeated error test failures, stop="<<*stop<<", start="<<*start<<std::endl;return;;
      		case -5: std::cout<<" integration failed, repeated convergence failures, stop="<<*stop<<", start="<<*start<<std::endl;return;;
      		case -6: std::cout<<" integration failed, error weight became zero during problem, stop="<<*stop<<", start="<<*start<<std::endl; return;;
      		default: std::cout<<"unknown error, ISTATE="<<ISTATE<<std::endl; return;
      }
	*start=*tout;*tout+=stepsize;
      }
      
    if(ISTATE==2) {std::cout<<"integration successful"<<std::endl;std::cout<<*tout<<"   ";for (int i=0;i<neq;i++) std::cout<<Y[i]<<"   "<<std::endl;}
    // std::cout<<"stop="<<stop<<std::endl;
     
     delete[] WORK; 
     delete[] IWORK;
     delete start; 
     delete stop; 
     delete tout;
    }
  }
}
