#include "functions/iffeatures.hpp"
#include "functions/iderivative.hpp"
#include "functions/itaylorcoeff.hpp"
#include "functions/ivfunction.hpp"

#include "functions/details/fvbase.hpp"
#include "functions/details/fjoin.hpp"
#include "functions/details/fderiv.hpp"

// Ein solcher Funktor kann zur Beschreibung einer Funktion
// herangezogen werden
struct deriv_test_t
{

  // Funktionsoperator wird fuer die eigentliche Funktionsauswertung
  // verwendet. 
  // Er muss mit {r,i,a,t}vector aufrufbar sein!
  // Sind weitere Eigenschaften (wie z.B. Differenzierbarkeit mittels
  // fadbad gewuenscht) sollte er ein template sein, da dass Framework
  // ihn dann automatisch mit weiteren Typen wie fadbad::F<rvector>
  // etc aufruft.
  template <typename T>
  T operator()(const T& args) const
    {
      T fvec(4);
      fvec[0] = args[1];
      fvec[1] = -4.0*args[0];
      fvec[2] = sqr(args[0])+2*sqr(args[1]);
      fvec[3] = pow(args[0],3)*pow(args[1],2);
      return fvec;
    }

  // Dimension des Eingabevektors (=Anzahl der unabhängigen Variablen)
  unsigned dim() const
    {
      return 2;
    }

  // Dimension der Codomain (=Anzahl der abhängigen Variablen)
  unsigned cdim() const
    {
      return 4;
    }

  // Textuelle Beschreibung (für den Benutzer oder Debugging)
  std::string text() const
    {
      return "IVP Test";
    }
};

void test_deriv()
{
  // Funktion erstellen
  // Im Beispiel wird die Funktion auf Basis des Funktors deriv_test_t
  // erstellt (s.o.), mittels FJoin koennen weitere Eigenschaften an
  // die Funktion angefuegt. In unserem Fall FDeriv, welches
  // automatisch Ableitungen mittels fadbad zur Verfuegung stellt und
  // diese ueber das IDerivative Interface bereitstellt.
  functions::details::FJoin<deriv_test_t, functions::details::FDeriv<deriv_test_t> > t;
 
  core::arith::rvector tv(2);
  tv[0] = 3.0;
  tv[1] = 5.0;

  // Die erstellte Funktion unterstuetzt das generische
  // Funktionsinterface IVFunction, nur auf dessen Basis sollte weiter
  // gearbeitet werden!
  // Somit ist es moeglich dieses Interface in Sonderfaellen auch
  // selbst zu implementieren, fuer erhoehte Flexibilitaet!
  functions::IVFunction &ft = t;

  // 5 0 1
  std::cout << "Grad y1: " << ft.features().derivative()->grad_fun(0)(tv) << std::endl; 
  // -12 -4 0
  std::cout << "Grad y2: " << ft.features().derivative()->grad_fun(1)(tv) << std::endl;
  // 59 6 20
  std::cout << "Grad y3: " << ft.features().derivative()->grad_fun(2)(tv) << std::endl;

  std::cout << "Jac Grad y1" << ft.features().derivative()->grad_fun(0).features().derivative()->jacobian(tv) << std::endl;
  std::cout << "Jac Grad y2" << ft.features().derivative()->grad_fun(1).features().derivative()->jacobian(tv) << std::endl;
  std::cout << "Jac Grad y3" << ft.features().derivative()->grad_fun(2).features().derivative()->jacobian(tv) << std::endl;

  // Wie Grad y3
  std::cout << " Grad 0 von Grad y3 " << ft.features().derivative()->grad_fun(2).features().derivative()->grad_fun(0)(tv) << std::endl;
  // 6 2 0
  std::cout << " Grad 1 von Grad y3 " << ft.features().derivative()->grad_fun(2).features().derivative()->grad_fun(1)(tv) << std::endl;
  // 20 0 4
  std::cout << " Grad 2 von Grad y3 " << ft.features().derivative()->grad_fun(2).features().derivative()->grad_fun(2)(tv) << std::endl;

  // 2
  std::cout << "dx dx y3 " << ft.features().derivative()->grad(1, 2).features().derivative()->grad(1)(tv) << std::endl;
  // 4
  std::cout << "dy dy y3 " << ft.features().derivative()->grad(2, 2).features().derivative()->grad(2)(tv) << std::endl;
  // 0
  std::cout << "dy dx y3 " << ft.features().derivative()->grad(2, 2).features().derivative()->grad(1)(tv) << std::endl;
  // 0
  std::cout << "dx dy y3 " << ft.features().derivative()->grad(1, 2).features().derivative()->grad(2)(tv) << std::endl;

  // 450
  std::cout << "dx dx y4 " << ft.features().derivative()->grad(1, 3).features().derivative()->grad(1)(tv) << std::endl;
  // 270
  std::cout << "dx dy y4 " << ft.features().derivative()->grad(1, 3).features().derivative()->grad(2)(tv) << std::endl;
  // 270
  std::cout << "dy dx y4 " << ft.features().derivative()->grad(2, 3).features().derivative()->grad(1)(tv) << std::endl;

  // 150
  std::cout << "dx dx dx y3 " << ft.features().derivative()->grad(1, 3).features().derivative()->grad(1).features().derivative()->grad(1)(tv) << std::endl;
  // 0
  std::cout << "dy dy dy y3 " << ft.features().derivative()->grad(2, 3).features().derivative()->grad(2).features().derivative()->grad(2)(tv) << std::endl;
  // 180
  std::cout << "dy dx dx y3 " << ft.features().derivative()->grad(2, 3).features().derivative()->grad(1).features().derivative()->grad(1)(tv) << std::endl;
  // 54
  std::cout << "dx dy dy y3 " << ft.features().derivative()->grad(1, 3).features().derivative()->grad(2).features().derivative()->grad(2)(tv) << std::endl;

  // 450 150 180
  std::cout << "Grad Fun(dx dx y3)" << ft.features().derivative()->grad(1, 3).features().derivative()->grad(1).features().derivative()->grad_fun(0)(tv) << std::endl;

  // Nein (falls Standard order auf 3)
  std::cout << "Deriv 4. Ordnung? " << ft.features().derivative()->grad(1, 3).features().derivative()->grad(1).features().derivative()->grad(1).features().derivative() << std::endl;
  std::cout << "Deriv 4. Ordnung GradFun? " << ft.features().derivative()->grad(1, 3).features().derivative()->grad(1).features().derivative()->grad_fun(0).features().derivative() << std::endl;


  // Speicher Test Direktionale Ableitung
  functions::details::FJoin<deriv_test_t, functions::details::FDeriv<deriv_test_t> > *ft_mem = new  
    functions::details::FJoin<deriv_test_t, functions::details::FDeriv<deriv_test_t> >;
  functions::IFunction *ft_grad = ft_mem->features().derivative()->grad(1,3).clone();
   delete ft_mem;
   std::cout << "Dim geklonte Funktion: " << ft_grad->dim() << std::endl;
   std::cout << "Auswertung geklonte Funktion: " << ft_grad->operator()(tv) << std::endl;
   delete ft_grad;

  // // Speicher Test GradFun
  ft_mem = new functions::details::FJoin<deriv_test_t, functions::details::FDeriv<deriv_test_t> >;
  functions::IVFunction *ft_grad_fun = ft_mem->features().derivative()->grad_fun(1).clone();
  delete ft_mem;
  std::cout << "Dim geklonte Funktion: " << ft_grad_fun->dim() << std::endl;
  std::cout << "Auswertung geklonte Funktion: " << ft_grad_fun->operator()(tv) << std::endl;
  delete ft_grad_fun;
}
