/**
 * @file   newtonstep.cpp
 * @author Stefan Kiel <fluxy@lucy>
 * @date   Mon May 11 14:27:22 2009
 *
 * @brief  Impl. of multidim. newton step
 *
 *
 */


#include <algorithm/opt-lib/newtonstep.hpp>
#include <core/cxsccomp.hpp>

#include "functions/ifunction.hpp"

#include <algorithm/opt-lib/gaussjordan.hpp>
#include <algorithm/opt-lib/general.hpp>
//#include "simplefunction.hpp"
#include <algorithm/opt-lib/exception.hpp>
#include <algorithm/opt-lib/helper.hpp>
#include <algorithm/opt-lib/globalcriteria.hpp>
#include <boost/numeric/mtl/mtl.hpp>
// TODO C11
#include <boost/dynamic_bitset.hpp>


// debug
//#include "tests/mupad.hpp"

// Definieren falls C-XSC Multithreadfähig ist
#define CXSC_FIXED

#ifndef CXSC_FIXED
#include "cxscopenmp.hpp"
#endif

// Huellenmethode aktivieren
#define WITH_HULLMETHOD

using namespace core::arith;
using namespace std;

namespace GlobOpt {


 /*!
  ** Gauss-Seidel Einzelschrittverfahren
  ** Solves M*(y-x) = r(x)
  **
  ** @param M matrix m
  ** @param R vector r
  ** @param box current box
  ** @param x vector x
  ** @param empty flag indicates an empty result
  ** @param gaps vecot of gaps in box
  **
  ** @return new enclosure of box
  */
  ivector gaussSeidel(const imatrix &M, const ivector &R,
		      const ivector &box, const rvector &x,
		      bool &empty, vector<Gap> &gaps, ivector *nwt) {
    empty = false;
    ivector newBox(box);

    // iteration fuer xi
    int ub = Ub(box);
    // Anzahl der Elemente ohne 0 in Mii
    int cZero = ub;

    // Erst ohne erweiterte Arithmetik
    // siehe C-XSC Buch
    for(int i = 0; i < ub && !empty; i++) {
      if(0 <= M[i][i]) {
        cZero--;
        continue;
      }
      interval sum1(0,0);
      for(int j = 0; j < i; j++)
        sum1 += M[i][j]*(newBox[j] - x[j]);
      interval sum2(0,0);
      for(int j = i; j < ub; j++)
        sum2 += M[i][j]*(box[j] - x[j]);
      interval test(R[i] - sum1 -sum2);
      test /= M[i][i];
      test += x[i];
      newBox[i] = test;
      if(nwt)
	(*nwt)[i] = newBox[i];
      newBox[i] &= box[i];
      empty = core::arith::isEmpty(newBox[i]);
    }

    // rausspringen, da eine Verbesserung
    // unwahrscheinlich ist (siehe Hansen S.:241)
    if(!cZero)
      return newBox;

    // Elemente die eine 0 in Mii enthalten (erweiterte Arithmetik)
    for(int i = 0; i < ub && !empty; i++) {
      if(!(0 <= M[i][i]))
        continue;

      interval sum1(0,0);
      for(int j = 1; j < i; j++)
        sum1 += M[i][j]*(newBox[j] - x[j]);

      interval sum2(0,0);
      for(int j = i; j < ub; j++)
        sum2 += M[i][j]*(box[j] - x[j]);

      ivector z(ex_division((R[i] - sum1 - sum2), M[i][i]));
      z[0] = (z[0] + x[i]) & box[i];
      if(!isEmpty(z[1]))
	z[1] =  (z[1] + x[i]) & box[i];
      //TODO_C: Im nachfolgenden z[2] auf z[1] und z[1] auf z[0] gesetzt
      if(!core::arith::isEmpty(z[1])) {
          if(inf(z[1]) > sup(z[0]))
            gaps.push_back(make_pair(i, interval(inf(z[1]), sup(z[0]))));
          else
            gaps.push_back(make_pair(i, interval(sup(z[0]), inf(z[1]))));
        newBox[i] =  z[0] | z[1];
      }
      else
        newBox[i] = z[0];
      //TODO_C:BIS HIER
      if(nwt)
	(*nwt)[i] = z[0];
//      ivector z(box[i] & (x[i] + ([0]));
//      ivector z(box[i] & (x[i] + ([0]));
//      ivector z(box[i] & (x[i] + ([0]));
      empty = core::arith::isEmpty(newBox[i]);
    }
    return newBox;
  }

  /**
   * Bliek-Hansen hull method
   *
   * @param M1  Matrix M
   * @param r1  right-hand-side
   * @param box box
   * @param expPoint expansion point in box
   * @param empty flag for empty result
   * @param irr flag for irregular matrix m
   *
   * @return interval hull of LGS
   */
  ivector hullMethod(const imatrix &M1, const ivector &r1, const ivector &box,
		     const rvector &expPoint, bool &empty, bool &irr)
  {
    imatrix M(M1);
    ivector r(r1);

    // Indikator für Erfolg bei Matrixinvertierung
    bool succ;
    empty = false;
    irr = false;

    // anpassen von M dass mid(m) == Id(M) gilt
    for(unsigned i = 1; i <num_rows(M) && !irr; i++) {
      for(unsigned j = 1; j < num_cols(M) && !irr; j++) {
        if(i != j) {
          if(!(sup(M[i][j]) <= -abs(sup(M[i][j]))))
	    M[i][j] = interval(-abs(sup(M[i][j])), sup(M[i][j]));
        }
        else if(i == j) {
	  M[i][i] = interval(min(inf(M[i][i]), inf(interval(2,2) - sup(M[i][i]))),
			     sup(M[i][i]));
	  if(inf(M[i][i]) <= 0)
	    irr = true;
        }
      }
    }
    // TODO:
    // Die entstandene Matrix hat aufgrund von Rundungsfehlern nur "fast" die
    // Identitätsmatrix als Mittelpunkt:
    // Einige nicht diagonalelemente haben Werte 10^-10 etc
    // Diese Fehler können im nächsten Schritt evtl. den Aufruf der Hüllenmethode
    // verhindern
    // Falls hier Bugs auftreten überprüfen

    // TODO:
    // erzeugen von P
    // unklar ob P nicht auch eine Intervallmatrix sein sollte
    // von der Logik her müsste der Round-Mode mind. auf UP gesetzt werden
    // in der Literatur wird nichts erwähnt
    // treten hier Bugs auf diesen Abschnitt überprüfen
    // (Hansen Buch 5.8 S.:99 ff // Hansen 2000 "The hull of preconditioned Interval
    // Linear Systems // Kearfott/Ning "A comparison of some methods for solving
    // linear interval equations")

    const rmatrix &P = invertMatrix(inf(M), succ);
    if(!succ) {
      irr = true;
      return box;
    }

    for(unsigned int i = 1; i <= num_rows(M) && !irr; i++) {
      for(unsigned int j = 1; j <= num_cols(M) && !irr; j++) {
	if(i != j)
	  irr = P[i][j] < 0;
	else
	  irr = P[i][i] < 1;
      }
    }


    if(irr)
      return box;

    // ab hier ist M sicher regulär
    // Vektor r anpassen
    boost::dynamic_bitset<> sign(Ub(r));
    for(int i = 1; i <= Ub(r); i++) {
      if((0 <= r[i] && -inf(r[i]) > sup(r[i])) || sup(r[i]) < 0) {
        r[i] *= -1;
        sign.set(i-1);
      }
      else
	sign.reset(i-1);
      // Sicherheitcheck
      assert(0 <= abs(inf(r[i])) && abs(inf(r[i])) <= sup(r[i]));
    }

#ifdef CXSC_FIXED
    ivector H(P*sup(r));
#else
    ivector H(CXSCComp::multimatvec(P, sup(r)));
#endif

    rvector C(1, Ub(box)), Z(1, Ub(box));
    for(int i = 1; i <= Ub(C); i++) {
      C[i] = 1/(2*P[i][i] - 1);
      Z[i] = (inf(r[i]) + sup(r[i]))*P[i][i] - inf(H[i]);
    }



    ivector result(box);
    for(int i = 1; i <= Ub(H) && !empty; i++) {
      if(Z[i] > 0)
        H[i] = interval(C[i]*Z[i], sup(H[i]));
      else
	H[i] = interval(Z[i], sup(H[i]));
      if(sign[i-1])
        H[i] *= -1;

      result[i] &= H[i] + expPoint[i];

      if(core::arith::isEmpty(result[i]))
        empty = true;
    }

    return result;
  }


  ivector newtonStep(const ivector &box, const rvector &expPoint,
		     const FLst &functions, bool &empty,
		     vector<Gap> &gaps, ivector *nwt)
  {
    empty = false;

    //1
    // zeigt einen Fehler an
    bool err = false;
    // zeigt eine irreguläre Matrix an
    bool irr = false;
    const imatrix &J = jacobian(box, expPoint, functions, true, err);
    if(err)
      return box;
    const rmatrix &JCenter = core::arith::mid(J);
    const rmatrix &B = invertMatrix(JCenter, irr);
#ifdef CXSC_FIXED
    const imatrix &M = static_cast<imatrix>(B*J);
    const ivector &r = static_cast<ivector>(B*(-1)*evalFunctions(functions, expPoint));
#else
    const imatrix &M = CXSCComp::multimatmat(B, J);
    const ivector &r = CXSCComp::multimatvec(-B, evalFunctions(functions, expPoint));
#endif

#ifdef DEBUG_OUTPUT
    cout << "***************************NEWTON********************************" << endl;
    cout << "Box: " << endl << box << endl;
    cout << "J: " << endl << J << endl;
    cout << "JCenter: " << endl << JCenter << endl;
    cout << "B: " << endl << B << endl;
    cout << "M: " << endl << M << endl;
    cout << "r: " << endl << r << endl;
    cout << "****************************************************************" << endl;
#endif

#ifdef WITH_HULLMETHOD
    if(!nwt) {
      // Hüllenmethode testen
      ivector newBox(hullMethod(M, r, box, expPoint, empty, irr));
      if(!irr || empty) {
	return newBox;
      }
    }
#endif

    // ab hier Gauss-Seidel
    ivector refBox, newBox(box);
    do {
      refBox = newBox;
      newBox = gaussSeidel(M, r, refBox, expPoint, empty, gaps, nwt);
    } while (!empty && GC::Instance().hasReduced(newBox, refBox) && !nwt);

    return newBox;
  }

  // ivector newtonStep(const ivector &box, const rvector &expPoint,
  // 		     const C1Lst &functions, bool &empty,
  // 		     vector<Gap> &gaps, ivector *nwt)
  // {
  //   CC1Lst lst;
  //   BOOST_FOREACH(C1Function &f, functions) {
  //     lst.push_back(boost::reference_wrapper<const C1Function>(f));
  //   }
  //   return newtonStep(box, expPoint, lst, empty, gaps, nwt);
  // }

}
