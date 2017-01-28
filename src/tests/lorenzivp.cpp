#include "tests/lorenzivp.hpp"

#include "functions/details/fvbase.hpp"
#include "functions/details/fjoin.hpp"
#include "functions/details/fderiv.hpp"
#include "functions/details/ftaylorcoeff.hpp"
#include "functions/parser/functionparser.hpp"

#include "ivp/details/ivpimpl.hpp"
#include "ivp/details/dummyu.hpp"

namespace t
{
  using namespace core::arith;

  typedef fadbad::T<interval> Tinterval;
  typedef void(*Tfunction)(int n,
                           Tinterval*yp,
                           const Tinterval*y,
                           Tinterval t,
                           void*param);

  class FadbadODE
  {
  public:
    FadbadODE(int n,Tfunction f,void*param= 0)
      {
        size= n;
        y_coeff= new Tinterval[2*n];
        f_coeff= y_coeff+n;
        fcn= f;
        fcn(size,f_coeff,y_coeff,t,param);
      }


    void set(const interval&t0,const ivector&y0,
             const interval&h,int k)
      {
        t[0]= t0;
        t[1]= h;
        stepsize= h;order= k;
        for(int eqn= 0;eqn<size;eqn++)
          y_coeff[eqn][0]= y0[eqn];
      }

    void compTerms()
      {
        for(int eqn= 0;eqn<size;eqn++)
          f_coeff[eqn].reset();
        for(int coeff= 1;coeff<=order;coeff++)
          for(int eqn= 0;eqn<size;eqn++)
          {
            f_coeff[eqn].eval(coeff-1);
            y_coeff[eqn][coeff]= stepsize*f_coeff[eqn][coeff-1]/double(coeff);

          }

        for(unsigned i = 0; i < size; i++) {
          std::cout << "Eq " << i << ": ";
          for(unsigned j = 0; j <= order; j++)
            std::cout << y_coeff[i][j] << "; ";
          std::cout << std::endl;
        }
      }

    void sumTerms(ivector&sum,int m)
      {
        interval s;
        for(int eqn= 0;eqn<size;eqn++)
        {
          s= 0.0;
          for(int coeff= m;coeff>=0;coeff--)
            s+= y_coeff[eqn][coeff];
          sum[eqn]= s;
        }
      }

    void getTerm(ivector&term,int i)const
      {
        for(int eqn= 0;eqn<size;eqn++)
          term[eqn]= y_coeff[eqn][i];
      }

    interval getStepsize()const
      {

      }

    void eval(void*param)
      {
        fcn(size,f_coeff,y_coeff,t,param);
      }

  private:
    Tfunction fcn;
    Tinterval*y_coeff,*f_coeff,t;

    int size;
    int order;
    interval stepsize;
  };


  typedef fadbad::T<fadbad::F<interval> > TFinterval;
  typedef void(*TFfunction)(int n,TFinterval*yp,const TFinterval*y,
			    TFinterval tf,
			    void*param);

  class FadbadVarODE
  {
  public:
    FadbadVarODE(int n,TFfunction f,void*param=0)
      {
	size= n;
	tf_in= new TFinterval[2*n];
	tf_out= tf_in+n;
	fcn= f;
	fcn(size,tf_out,tf_in,tf,param);
      }

    ~FadbadVarODE()
      {
	delete[]tf_in;
      }

    void set(const interval&t0,const ivector&y0,const interval&h,int k)
      {
	stepsize= h;
	order= k;
	tf[0].x()= t0;
	tf[1].x()= h;
	
	for(int eqn= 0;eqn<size;eqn++)
	{
	  tf_in[eqn][0]= y0[eqn];
	}
      }

    void compTerms()
      {
	for(int eqn= 0;eqn<size;eqn++)
	  tf_out[eqn].reset();
	
	for(int eqn= 0;eqn<size;eqn++)
	  tf_in[eqn][0].diff(eqn,size);

	for(int coeff= 0;coeff<order;coeff++)
	{
	  for(int eqn= 0;eqn<size;eqn++)
	  {
	    tf_out[eqn].eval(coeff);
	    tf_in[eqn][coeff+1]= stepsize*(tf_out[eqn][coeff]/double(coeff+1));
	  }
	}

	
      std::cout << "****************************************************************" << std::endl;
      std::cout << "t0 " << tf[0].x() << " y0: ";
      for(unsigned i = 0; i < size; i++)
      	std::cout << tf_in[i][0].x() << std::endl;
      std::cout <<  " h: " << tf[1].x() << " k:" << order << std::endl;
      for(unsigned e = 0; e < size; e++) {
      	std::cout << "Eq " << e << ": ";
	for(unsigned i = 0; i < size; i++) {
	  std::cout << "Richtung: " << i;
	  for(unsigned j = 0; j <= order; j++)
	    std::cout << tf_in[e][j].d(i) << "; ";
	  std::cout << std::endl;
	}
      	std::cout << std::endl;
      }
      }
    

    void sumTerms(imatrix&Sum,int k)
      {
	for(int row= 0;row<size;row++)
	  for(int col= 0;col<size;col++)
	  {
	    interval s(0.0);
	    for(int coeff= k;coeff>=1;coeff--)s+= tf_in[row][coeff].d(col);
	    Sum[row][col]= s;
	  }

	std::cout << Sum << std::endl;

	for(int row= 0;row<size;row++)
	  Sum[row][row]+= 1.0;
      }

    void getTerm(imatrix&Term, int i)const
      {
	for(int row= 0;row<size;row++)
	  for(int col= 0;col<size;col++)
	    Term[row][col]= tf_in[row][i].d(col);
      }

  private:
    TFinterval*tf_in,*tf_out,tf;
    TFfunction fcn;
    int size;
    int order;
    interval stepsize;
  };


  void fun(int n, TFinterval* res, const TFinterval *args, TFinterval, void*)
  {
    static core::arith::interval sigma(10.0);
    static core::arith::interval rho(28.0);
    static core::arith::interval beta(core::arith::interval(8.0)/3.0);

    //T res(3);
    res[0] =  (sigma)*(args[1]-args[0]);
    res[1] = args[0]*((rho) - args[2]) - args[1];
    res[2] = args[0]*args[1] -(beta)*args[2];
  }

  void fun(int n, Tinterval* res, const Tinterval *args, Tinterval, void*)
  {
    static core::arith::interval sigma(10.0);
    static core::arith::interval rho(28.0);
    static core::arith::interval beta(core::arith::interval(8.0)/3.0);

    //T res(3);
    res[0] =  (sigma)*(args[1]-args[0]);
    res[1] = args[0]*((rho) - args[2]) - args[1];
    res[2] = args[0]*args[1] -(beta)*args[2];
  }

  void fun_1(int n, TFinterval *res, const TFinterval *args, TFinterval, void*)
  {
    res[0] = args[0];
  }
}


struct lorenz_t
{
  template <typename T>
  T operator()(const T& args) const
    {
      static core::arith::interval sigma(10.0);
      static core::arith::interval rho(28.0);
      static core::arith::interval beta(core::arith::interval(8.0)/3.0);

      T res(3);
      res[0] =  forced_cast<typename T::value_type>(sigma)*(args[1]-args[0]);
      res[1] = args[0]*(forced_cast<typename T::value_type>(rho) - args[2]) - args[1];
      res[2] = args[0]*args[1] - forced_cast<typename T::value_type>(beta)*args[2];
      return res;
    }

  std::string text() const
    {
      return "Lorenz Equation";
    }

  unsigned cdim() const
    {
      return 3;
    }

  unsigned dim() const
    {
      return 3;
    }
};

namespace vnodelp
{
  class AD_VAR;
}

namespace extras
{
  namespace interfaces
  {
    vnodelp::AD_VAR* create_ad_var(const ivp::IIVP &ivp);
  }
}

ivp::IIVP* create_lorenz_ivp()
{

  // t::FadbadODE ode(3, &t::fun);
  // core::arith::interval t0(1);
  // core::arith::ivector y0(3, core::arith::interval(15));
  // y0[2] = core::arith::interval(36);
  // core::arith::interval h(0.0316903760615897, 0.0316903760615898);
  // ode.set(t0, y0, h, 19);
  // ode.compTerms();

  // t::FadbadVarODE vode(3, &t::fun);
  // core::arith::interval t0_(0);
  // core::arith::ivector y0(3, core::arith::interval(15));
  // y0[2] = core::arith::interval(36);
  // core::arith::interval h(1.0);
  // vode.set(t0_, y0, h, 10);
  // vode.compTerms();
  // //core::arith::imatrix v(3,3);
  // //vode.sumTerms(v, 10);


//  exit(0);

  functions::IVFunction& rhs = *(new functions::details::FJoin<lorenz_t, functions::details::FDeriv<lorenz_t>, functions::details::FTaylorCoeff<lorenz_t > >);
  core::arith::ivector p(0);
  functions::IVFunction& u = *(ivp::details::create_dummyu());
  core::arith::mreal t0(0.0);
  core::arith::ivector x0(3);
  x0[0] = 15.0;
  x0[1] = 15.0;
  x0[2] = 36.0;
  core::arith::ivector u0(0);
//  return new ivp::details::IVPImpl(rhs, p, u, t0, x0, u0);
  //extras::interfaces::create_ad_var(ivp::details::IVPImpl(rhs, p, u, t0, x0, u0));

  return new ivp::details::IVPImpl(rhs, p, u, t0, x0, u0);

  //exit(-1);

  
  

  // functions::IFunction *f1 = functions::parser::parse_string("x0",1);
  // core::arith::mreal veri_t0(0.0);
  // core::arith::ivector veri_x0(1, core::arith::interval(1.0));
  // functions::IVFunction *u = ivp::details::create_dummyu();
  // core::arith::ivector p(0);

  // t::FadbadVarODE vode(1, &t::fun_1);  
  // vode.set(core::arith::interval(veri_t0), veri_x0, core::arith::interval(1,1), 21);
  // vode.compTerms();

  // auto ret(f1->features().taylor_coeff()->taylor_coeff_ode_df(veri_x0, 10));
  // for(unsigned i = 0; i < ret[0].size(); i++){
  //   for(unsigned j = 0; j < size(ret[0][i]); j++)
  //     std::cout << ret[0][i][j] << "; ";
  //   std::cout << std::endl;
  // }

  // auto ret2(f1->features().derivative()->grad(1).features().taylor_coeff()->taylor_coeff_ode(veri_x0, 10));  
  // for(unsigned i = 0; i < ret2.size(); i++){
  //   for(unsigned j = 0; j < size(ret2[i]); j++)
  //     std::cout << ret2[i][j] << "; ";
  //   std::cout << std::endl;
  // }


  //exit(-1);

//  return new ivp::details::IVPImpl(*f1, p, *u, veri_t0, veri_x0, p);


  //core::arith::mreal stop=1.0;
  // functions::IVFunction& rhs = *(new functions::details::> >);
  // core::arith::ivector p(0);
  // functions::IVFunction& u = *(ivp::details::create_dummyu());
  // core::arith::mreal t0(0.0);
  // core::arith::ivector x0(3);
  // x0[0] = 15.0;
  // x0[1] = 15.0;
  // x0[2] = 36.0;
  // core::arith::ivector u0(0);
  // return new ivp::details::IVPImpl(rhs, p, u, t0, x0, u0);
}
