/**
 * @file   verification.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:32:33 2009
 * 
 * @brief  Impl. of verification procedure
 * 
 * 
 */

#include <algorithm/opt-lib/verification.hpp>

#include <core/cxsccomp.hpp>
#include <functions/ifunction.hpp>

#include <algorithm/opt-lib/newtonstep.hpp>
#include <algorithm/opt-lib/helper.hpp>


using namespace core::arith;
using namespace std;

namespace GlobOpt {

  /**
   * Tries to prove that there is a solution in
   * F(box) = 0 using epsilon-inflation and newton steps
   *
   * @param box the box
   * @param lst list of functions
   * @param b function for epsilon-inflation
   *
   * @return verification result
   */
  VERIFICATION_RESULT newtonVer(const ivector &box,
				const FLst &lst, BLOWFunc b, core::arith::ivector &newBox)
  {
    assert(!"FIX ME. (TODO)");
    // max iterations
    unsigned int maxSteps = 10;
    // current box
    core::arith::ivector x(box);
    // result of newton operator
    core::arith::ivector nwt(lst.front().get().dim());
    // Fehler in Valgrind....
    for(int i = 1; i <= Ub(nwt); i++)
      nwt[i] = core::arith::interval(0,0);
    // gap list
    std::vector<Gap> gaps;
    // initial epsilon
    core::arith::mreal eps(0.25);

    bool empty = false;

    VERIFICATION_RESULT result = VSTEP;

    for(unsigned int i = 0; i < maxSteps && result == VSTEP; i++) {
      newtonStep(x, mid(x), lst, empty, gaps, &nwt);
      if(gaps.size()) {
	result = UNKNOWN_GAP;
      }
      if(MT::IsEmpty(nwt))
	result = UNKNOWN_EMPTY;
      else if(in(nwt, x)) {
	result = NLGS_UNIQUE_ZERO;
        newBox = nwt;
      }
      //TODO: Inklusionstest
      // else if(nwt < x) {
// 	result = NLGS_ZERO;
//         newBox = nwt;
//       }
      x = nwt;

      // eps-inflation
      b(x, eps, lst);
    }

    if(result == VSTEP)
      result = UNKNOWN;
    return result;
  }

// Dieser Abschnitt ist eine erste Implementierung des Miranda-Tests
// Aufgrund fehlender Präkonditionierung ist das Ergebnis allerdings
// relativ nutzlos.
// Um den Code nutzen zu können sind m.E. mind. 2 Dinge erforderlich:
// 1. Finden einer angemessenen Präkonditinierung
// 2. Zerlegen des topologischen Randes in disjunkte Teilfacetten
// Insbesondere Punkt 1 ist kritisch, da die Mittelpunktmatrix des
// KKT-Systems in der Nähe eines KKT-Punktes, meiner Erfahrung nach
// zur Irregularität neigt.
/*
  // topologischer Rand erzeugen
  vector<ivector> topologicalBoundary(const ivector &box)
  {
    vector<ivector> boundary;
    int ub = Ub(box);
    for(int i = 1; i <= ub; i++) {
      ivector x1(box);
      x1[i] = inf(box[i]);
      boundary.push_back(x1);
      x1[i] = sup(box[i]);
      boundary.push_back(x1);
    }
    return boundary;
  }

  VERIFICATION_RESULT mirandaVer(const ivector &box, const C1Lst &lst)
  {
    const vector<ivector> &boundary = topologicalBoundary(box);
    unsigned int size = boundary.size();

    rmatrix cond(1, Ub(box), 1, Ub(box));
    cond = Id(cond);

    bool cancel = false;
    BOOST_FOREACH(const C1Function &f, lst) {
      for(unsigned int i = 0; i < size && !cancel; i+=2) {
	cancel = (sup(cond*f(boundary[i]))) > 0 || (0 > inf(cond*f(boundary[i+1])));
      }
      if(cancel)
	break;
    }

    if(cancel)
      return UNKNOWN;
    else
      return NLGS_ZERO;
  }
*/

  VerficationResult verificationStep(const rvector &point,
				     const FLst &lst, BLOWFunc b)
  {
    core::arith::ivector x(point);
    b(x, 0.25, lst);
    return verificationStep(x, lst, b);
  }


  VerficationResult verificationStep(const ivector &box,
				       const FLst &lst, BLOWFunc b)
  {
    VerficationResult result;
    core::arith::ivector newBox(Lb(box));
    result.newtonResult = newtonVer(box, lst, b, newBox);
    result.newtonBox = newBox;
    //result.mirandaResult = mirandaVer(box, lst);
    return result;
  }

  ostream& operator<<(ostream &os, const VerficationResult &result)
  {
    os << "Verification with Newtoniteration ";
    os << result.newtonResult << endl;
    os << "Verification with Mirandatest ";
    os << result.mirandaResult << endl;
    return os;
  }

  ostream& operator<<(ostream &os, VERIFICATION_RESULT result)
  {
    switch(result) {
    case UNKNOWN_GAP:
      os << "failed. Matrix is singular.";
      break;
    case UNKNOWN_EMPTY:
      os << "failed. Empty interval returned.";
      break;
    case UNKNOWN:
      os << "failed.";
      break;
    case NLGS_UNIQUE_ZERO:
      os << "was successful. Interval contains unique zero.";
      break;
    case NLGS_ZERO:
      os << "was successful. Interval contains at least one zero.";
      break;
    default:
      os << "returned unexspected result.";
    }
    os << endl;
    return os;
  }


}
