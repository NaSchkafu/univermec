/**
 * @file   checkjohn.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:09:40 2009
 *
 * @brief  Implementation of test for Fritz-John-Cond
 *
 *
 */


#include <algorithm/opt-lib/checkjohn.hpp>

#include <boost/foreach.hpp>

#include "functions/ifunction.hpp"

#include <algorithm/opt-lib/types.hpp>
#include <algorithm/opt-lib/optbox.hpp>
#include <algorithm/opt-lib/johnconstraint.hpp>
#include <algorithm/opt-lib/newtonstep.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

using namespace std;
using namespace functions;


namespace GlobOpt
{
  

  bool checkJohnCond(OptBox &b, const functions::IFunction &obj)
  {
    // Hilfsliste der aktiven NBs
    FLst constraints;
    OptBox::MetaList::const_iterator mItEnd(b.mConstraints().end());
    for(OptBox::MetaList::const_iterator it(b.mConstraints().begin());
	it != mItEnd; ++it)
      constraints.push_back(boost::reference_wrapper<functions::IFunction>(it->constraint()));

    // keine aktive NB
    // Box kann verworfen werden im Fall PROXIMITY_QUERY
    // diese Information ist aber nicht bekannt -> Box behalten
    if(!constraints.size())
      return false;

    // Hansen Normalisierungsgleichung
    FLst phi;
    IFunction *n = norm_const(obj.dim() + 1, b.mConstraints().size());
    phi.push_back(boost::reference_wrapper<IFunction>(*n));

    // Gleichungen für Minimierung der Lagrange-Funktion
    for(unsigned int i = 1; i <= obj.dim(); i++) {
      IFunction *jConst = john_constraint(i, obj, constraints);
      phi.push_back(boost::reference_wrapper<IFunction>(*jConst));
    }

    // Gleichungen für komplentären Schlupf
    for(FLst::size_type i = 0; i < constraints.size(); i++) {
      IFunction *inEq = john_ineq(i+1, obj.dim() + constraints.size() + 1, constraints[i]);
      phi.push_back(boost::reference_wrapper<IFunction>(*inEq));
    }

    // Box für NLGS
    int ub = constraints.size() + obj.dim() + 1;

    core::arith::ivector T(ub);
    core::arith::rvector nwtExp(ub);

    nwtExp[mtl::irange(0,obj.dim())] = mid(b.X());
    nwtExp[obj.dim()+1] = b.lambda0Nwt();

    T[mtl::irange(0,obj.dim())] = b.X();
    T[obj.dim()+1] = b.lambda0();;
    OptBox::MetaList::iterator mIt(b.mConstraints().begin());
    for(int i = obj.dim()+2; i <= ub; i++) {
      T[i] = mIt->lagMult();
      nwtExp[i] = (*mIt++).nwtExp();
    }

    // Newtonschritt
    bool empty = false;
    size_t oldGap = b.gaps().size();
    T = newtonStep(T, nwtExp, phi, empty, b.gaps());

    // Gaps in den Lagrange Multiplikatoren verwerfen
    // Boost lambda expression erzeugt eine unklare Warnung unter dem GCC 4.3
    assert(false && "FIX ME");
    // Box::GapList::iterator gapEnd(remove_if(b.gaps().begin(), b.gaps().end(),
    // 					    bind(&Gap::first, _1) > obj.dim() || bind(&Gap::first, _1) < 1));
    // if(gapEnd != b.gaps().end())
    //   b.gaps().erase(gapEnd, b.gaps().end());x

    // Cleanup
    BOOST_FOREACH(IFunction &f, phi) {
      delete &f;
    }

    // Box kann gelöscht werden
    if(empty) {
      return true;
    }

    // Box updaten
    b.update(T[mtl::irange(0, obj.dim())]);

    // Lagrange Multiplikatoren updaten
    b.setLambda0(T[obj.dim()+1]);
    b.setLambda0Nwt(mid(T[obj.dim()+1]));
    mIt = b.mConstraints().begin();
    for(int i = obj.dim() + 2; i <= ub; i++) {
      mIt->lagMult() = T[i];
      (*mIt++).nwtExp() = mid(T[i]);
    }

    // fertig, Box kann nicht verworfen werden
    return false;
  }

}
