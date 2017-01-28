/**
 * @file   linesearch.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:26:04 2009
 *
 * @brief  Impl. of line search procedure
 *
 *
 */


#include <algorithm/opt-lib/linesearch.hpp>

#include "functions/ifunction.hpp"
#include "functions/iffeatures.hpp"
#include "functions/iderivative.hpp"

#include <algorithm/opt-lib/exception.hpp>
#include <algorithm/opt-lib/optbox.hpp>
#include <algorithm/opt-lib/helper.hpp>

using namespace core::arith;

namespace GlobOpt
{
  bool lineSearch(const OptBox &b, const core::arith::ivector &initBox,
		  const functions::IFunction &F, rvector &ubPoint, bool proximityQuery)
  {
    /// FIXME Derivative Handling
    const functions::IDerivative* pdF = F.features().derivative();
    assert(pdF && "Linesearch requires first order informatiosn!");
    const functions::IDerivative& dF = *pdF;


    rvector x(mid(b.X()));

    bool feasible = b.feasible();
    if(!feasible)
      feasible = b.feasible(x);
    // Schritt 2 ist nicht impl derzeit
    if(!feasible)
      return false;


    rvector y, y_, y__;

    // 3
    // Schritt wurde modifiziert und entspricht nicht original Hansen Algorithmus
    // (siehe Diplomarbeit)
    rvector x_;
    if(proximityQuery) {
      mreal c1(infinity()), c2(infinity());
      for(unsigned i = 0; i < F.dim()/2; i++) {
	if(!(0 <= dF.grad(i)(x)))
	  c1 = std::min(c1, wid(b.X()[i]) / 2*abs(dF.grad(i)(x)));
      }
      for(unsigned i = F.dim()/2; i <= F.dim(); i++) {
	if(!(mreal(0) != dF.grad(i)(x))) // Fixed Vergleich (SK 08.04.10)
	  c2 = std::min(c2, wid(b.X()[i]) / 2*abs(dF.grad(i)(x)));
      }

      // Sicherstellen dass die Konstante c existiert
      if(c1 == infinity())
	c1 = c2;
      else if(c2 == infinity())
	c2 = c1;
      if(c1 == infinity())
	return false;

      x_ =  mid(static_cast<ivector>(x - c1*dF.grad(x)));
      for(unsigned int i = F.dim()/2; i <= F.dim(); i++)
	x_[i] = x[i] - c2*dF.grad(i)(x);
    }
    else {
      // Umodifzierte Liniensuche nach Hansen
      mreal c(infinity());
      for(unsigned i = 0; i < F.dim(); i++) {
	if(!(mreal(0) != dF.grad(i+1)(x))) // Fixed Vergleich (SK 08.04.10)
	  c = std::min(c, wid(b.X()[i]) / 2*abs(dF.grad(i+1)(x)));
      }
      if(c == infinity())
	return false;
      x_ =  mid(static_cast<ivector>(x - c*dF.grad(x)[mtl::irange(1,mtl::imax)]));
//       std::cout << "Gradient Box: " << F.grad(x) << std::endl;
//       std::cout << "Mitte: " << mid(F.grad(x)) << std::endl;
//       std::cout << "c*mid(...): " << c*F.grad(x) << std::endl;
//       ivector v(c*F.grad(x));
//       ivector v1(x - v);
//       std::cout << "x - c*...: " << static_cast<ivector>((x - c*F.grad(x))) << std::endl;
//       if(b.isFeasible(x_));
//       std::cout << "-------" << std::endl;
    }

    if(!(MT::inside(x_, initBox)))
      return false;

    // TODO: Fixme (SK)
    // x_ liegt in initBox aber nicht in b.X()
    // Das ist mathematisch gesehen in Ordnung, derzeit kann aber die Zulässigkeit von x_ nicht
    // bewiesen werden, da die aktuelle Box nur Referenzen auf die aktiven NBs der Box enthält.
    // Jedoch könnte x_ eine für die Box inaktive NB verletzen.
    assert(MT::inside(x_, b.X()) && "x_ is inside the initial box but outside the current box");
#ifdef NEBUG
    if(!MT::inside(x_, b.X())) // Sicherheitscheck, s.o.
      return false;
#endif
    

    // 4
    y = x;
    y_ = x_;
    // TODO: Semantik mid geandert (SK 17.11.09)
     if(F(x_) < F(x)) {
      // 5
      if(b.feasible(x_)) {
	y = x_;
	y_ = x;
      }
    } // else goto 6
    int n = 4;

    do {
      // 6
      y__ = 0.5*(y + y_);

      // 7
      //TODO: Nochmal durchgehen 
      if(F(y__) < std::max(F(y), F(y_))) {
	// 8
	if(F(y__) >= F(y)) {
	  y_ = y__;
	} // else goto 10
	else {
	  // 9
	  if(b.feasible(y__)) {
	    y_ = y;
	    y = y__;
	  }
	}
      } // else goto 11
      else {
	break;
      }

    } while(--n);

    //TODO: Sicherheitscheck rausnehmen!
    if(!b.feasible(y)) {
      S_THROW("Fehler bei Liniensuche!");
    }

    // 11
    ubPoint = y;
    return true;
  }
}
