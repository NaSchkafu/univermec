#include "functions/itaylorcoeff.hpp"
#include "functions/iffeatures.hpp"
#include "functions/iderivative.hpp"
#include "functions/itaylorcoeff.hpp"
#include "functions/ivfunction.hpp"
#include "functions/parser/functionparser.hpp"

#include "functions/details/fvbase.hpp"
#include "functions/details/fjoin.hpp"
#include "functions/details/fderiv.hpp"

class TODE
{
public:
  fadbad::T<double> x;            // Independent variables
  fadbad::T<double> xp;           // Dependent variables
  TODE(){xp=cos(x);}      // record DAG at construction
};

class TDODE
{
public:
  fadbad::T<double> x;            // Independent variables
  fadbad::T<double> xp;           // Dependent variables
  TDODE(){xp=-sin(x);}      // record DAG at construction
};


int fak(int a)
{
  return a > 1 ? a*fak(a-1) : a;
}

int test_taylor()
{
  // functions::IFunction &ft = *functions::parser::parse_string("cos(x0)*sin(x1)");
  // const functions::ITaylorCoeff* taylor = ft.features().taylor_coeff();
  // core::arith::rvector v(2, 1.0);
  // std::cout << v << std::endl;
  // std::cout << taylor->taylor(v, 4) << std::endl;

  // return 0;

  TODE ode;                // Construct ODE:
  ode.x[0]=1;              // Set point of expansion:
  for(int i=0;i<10;i++)
  {
    ode.xp.eval(i); // Evaluate i'th Taylor coefficient
    ode.x[i+1]=ode.xp[i]/double(i+1);// Use dx/dt=ode(x).
  }
  // ode.x[0]...ode.x[10] now contains the Taylor-coefficients
  // of the solution of the ODE.

  // Print out the Taylor coefficients for the solution
  // of the ODE:

  // Test Original
  core::arith::rvector v(1, 1.0);
  // Funktion erstellen
  functions::IFunction &ft = *functions::parser::parse_string("cos(x0)");
  // Taylorkoeffizienten Interface der Funktion holen
  const functions::ITaylorCoeff* taylor = ft.features().taylor_coeff();
  auto vec(taylor->taylor_coeff_ode(v, 10));
  for(unsigned i = 0; i < mtl::size(vec[0]); i++) {
    std::cout << "x[" << i << "]: " << vec[0][i] << std::endl;
    std::cout << "x[" << i << "]: " << ode.x[i] << std::endl;
  }
  
  // Test Ableitung
  TDODE dode;                // Construct ODE:
  dode.x[0]=1;              // Set point of expansion:
  for(int i=0;i<10;i++)
  {
    dode.xp.eval(i); // Evaluate i'th Taylor coefficient
    dode.x[i+1]=dode.xp[i]/double(i+1);// Use dx/dt=ode(x).
  }

  // Richtungsableitung von ft in Richtung x
  const functions::IFunction &dfx = ft.features().derivative()->grad(1);
  // Interface fuer Taylorcoeffs der Richtungsableitung holen
  const functions::ITaylorCoeff* dtaylor = dfx.features().taylor_coeff();
  auto dvec(dtaylor->taylor_coeff_ode(v, 10));
  for(unsigned i = 0; i < mtl::size(dvec[0]); i++) {
    std::cout << "dx[" << i << "]: " << dvec[0][i] << std::endl;
    std::cout << "dx[" << i << "]: " << dode.x[i] << std::endl;
  }


  delete &ft;
  return 0;
 }


 
