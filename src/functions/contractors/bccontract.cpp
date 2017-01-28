/**
 * @file   boxconsistency.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:07:36 2009
 *
 * @brief  Implementation of box consistency
 *
 *
 */


#include <functions/contractors/bccontract.hpp>

#include <core/cxsccomp.hpp>
#include <functions/ifunction.hpp>
#include <functions/iffeatures.hpp>
#include <functions/iderivative.hpp>
#include <functions/contractors/details/newton.hpp>

namespace functions {

  namespace contractors
  {
    BCContract::BCContract(const functions::IFunction &f)
      :m_F(f)
    {
      //std::cout << f.text() << std::endl;
      m_df = f.features().derivative();
      assert(m_df && "BCContract requested for function without IDerivative!");
    }
    
    BCContract::~BCContract()
    {
    }
    
    bool BCContract::prune_(core::Box &box, const core::arith::interval &rhs) const
    {
      bool empty = false;
      box.ref_X() = apply_bc(box.ref_X(), m_F, empty, rhs);
      return empty;
    }
    
    bool BCContract::prune_(core::GappedBox &box, const core::arith::interval &rhs) const
    {
      return prune_(static_cast<core::Box&>(box), rhs);
    }
    
    float BCContract::check_box_(const core::Box &) const
    {
      return 1.0;
    }
    
    std::string BCContract::text_() const
    {
      return "Box Consistency Contractor";
    }

    /*
     * Verbesserung: die neuen Grenzen von X die in improveBound
     * berechnet werden, koennten direkt an BCC1Function propagiert
     * werden, die wurde insbesondere beim rekursiven Branch in
     * improveBound bessere Ergebnisse bringen.
     */
    core::arith::ivector BCContract::apply_bc(const core::arith::ivector &X, const functions::IFunction &f,
                                              bool &empty, const core::arith::interval &rhs) const
    {
      if(!m_df)
	return X;

      core::arith::ivector new_box(X);
      for(int i = 0; i < Ub(X); i++) {
        if((empty = is_empty(X[i])))
          return X;
        bool ub = false;
        do {
          //std::cout << "newBox: " << newBox << std::endl << std::endl;

          ub = !ub;
          // immer eine neue Funktion mit verbesserten Grenzen verwenden
          // hier gibt es Optimierungspotential, da aber die
          // Nebenbedigungen in unserem Fall relativ "einfach" sind,
          // duerfte die schnellere Konvergenz hier zu bevorzugen sein
          core::arith::interval cInt(new_box[i]);
          switch(improve_bound(cInt, new_box, i, !ub, f, m_df->grad(i+1), rhs))
          {
          case NEW_BOUND:
            new_box[i] &= cInt;
            break;
          case INTERVAL_DELETED: { empty = true; return X; }
          case FINAL_BOUND: { new_box[i] &= cInt; ub = false; break; }
          case INTERNAL_ERROR: { assert(!"Undefined condition"); /*S_THROW("Undefined condition!");*/ }
          }
        } while(ub);
      }
      return new_box;
    }

    /** 
     * Tries to improve the bounds on x using for the equation f(x) = rhs
     * using an iterated one-dimensional newton-step
     * This function is called for a single independet variable of f.
     * 
     * 
     * @param x original bounds to improve
     * @param args original arguments
     * @param exp independet variable to consider
     * @param ub true to improve ub(x) otherwise lb(x) is improved
     * @param f original function
     * @param df derivative of function
     * @param rhs right hand side of function
     * 
     * @return 
     */
    BCContract::IMPROVE_BOUND_RESULT BCContract::improve_bound(core::arith::interval &x,
							       core::arith::ivector args, unsigned exp, 
							       bool ub, const functions::IFunction &f, 
							       const functions::IFunction &df, 
							       const core::arith::interval &rhs) const
    {

      // 1
      float beta = 0.25;
      core::arith::mreal w0(core::arith::wid(x));

      bool cont;
      do {
        cont = false;

        //core::arith::mreal &a = inf(X);
        //core::arith::mreal &b = sup(X);

        // 2
        args[exp] = (ub ? sup(x) : inf(x));
        core::arith::interval evalF(f(args) - rhs);

        if(0.0 <= evalF)
          return NEW_BOUND;

        // 3
        core::arith::mreal w(sup(x) - inf(x));
        core::arith::mreal c(ub ? (sup(x) - beta*w) : (inf(x) + beta*w)); // ge�ndert
        // Fuer LowerBound:
        // Suchintervall Y ist beim beginn ein Unterintervall von X = [a,b]
        // Y = [a, a + 0.25*w(X)], es wird in jedem Schritt des
        // Algorithmus erweitert. Ist das Intervall beim Start zu gro�,
        // ist die Ableitung beim Newtonschritt zu gro�, daher wird dann
        // kein sinnvolles Resultat erzeugt.
        // Ist es zu klein, gibt es keinen sinnvollen Fortschritt. Der
        // Wert von 0.25*weite wird von Hansen empfohlen
        // Fuer UpperBound:
        // Wahl wie bei LowerBound, nur suchen wir hier ausgehend von
        // [b - 0.25*weite, b], wir senken hier bei Iteration die untere
        // Grenze von Y in jedem Schritt
        core::arith::interval Y(ub ? c : inf(x), ub ? sup(x) : c); //ge�ndert
	args[exp] = Y;

        // 4
        // TODO: teilen durch 0!
        std::vector<core::arith::interval> newtonResults(details::stepNwt1D(ub ? sup(x) : inf(x), evalF, df(args), Y));

        core::arith::interval nwt(-core::arith::infinity(), core::arith::infinity());
        if(newtonResults.size() == 2) {
          core::arith::interval resA(newtonResults[0] & Y);
          core::arith::interval resB(newtonResults[1] & Y);
          if(isEmpty(resA) && !isEmpty(resB))
            nwt = newtonResults[1];
          else if(isEmpty(resB) && !isEmpty(resA))
            nwt = newtonResults[0];
          else if(!isEmpty(resA) && !isEmpty(resB))
            nwt = newtonResults[0] | newtonResults[1];
        }
        else if(newtonResults.size() == 1)
          nwt = newtonResults[0];
        else
          assert(!"Undefined execution path in BCContract procedure (newton step failed)");
        //THROW(GlobOptException, "Undefined execution path in BC procedure (newton step failed)");

        core::arith::interval newY(Y & nwt);

        // 5
        if(beta == 1 && isEmpty(newY))
          return INTERVAL_DELETED;

        // FIXME
        // if(beta == 1 && GC::Instance().checkInterval(newY)) {
        //   X = newY;
        //   return FINAL_BOUND;
        // }

        // 7
        if(beta < 1 && isEmpty(newY)) {
          if(ub)
	    x = core::arith::interval(inf(x), c);
          else
	    x = core::arith::interval(c, sup(x));
          beta *= 2;
          cont = true;
        } // goto 2
        // 8
        else if(beta == 1 && core::arith::wid(newY) < 0.5*core::arith::wid(Y)) {
          x = newY;
          cont = true;
        } // goto 2

        // 9
        else if ((!ub && inf(x) < sup(newY) && sup(newY) < c) ||
                 (ub && sup(x) > inf(newY) && inf(newY) > c)) {
          core::arith::interval final(ub ? inf(x) : c, ub ? c : sup(x));
          if(improve_bound(final, args, exp, ub, f, df, rhs) == INTERVAL_DELETED) { // leere box erzeugen
	    final = core::arith::interval(core::arith::infinity(), -core::arith::infinity());
            // final.set_inf(core::arith::infinity());
            // final.set_sup(-core::arith::infinity());
          }
          if(improve_bound(newY, args, exp, ub, f, df, rhs) != INTERVAL_DELETED) {
	    final = core::arith::interval(std::min(inf(nwt), inf(final)), std::max(sup(nwt), sup(final)));
          }

          if(isEmpty(final))
            return INTERVAL_DELETED;
          return NEW_BOUND;
        }
        //    else if(ub &&
        else {
          // 10
          if(ub)
	    x = core::arith::interval(inf(x), sup(newY));
	  else
	    x = core::arith::interval(inf(newY), sup(x));

          // 11
          if(core::arith::wid(x) < 0.5*w0) {
            // step 1
            beta = 0.25;
            w0 = sup(x) - inf(x);
            cont = true; // goto 2
          }
        }
        // 12
        if(!cont) {
          // ??? Warum darf ich das? (Sk 06/04/10)
          // X.set_inf(inf(newY));
          return NEW_BOUND;
        }
      } while(cont);

      return INTERNAL_ERROR;
    }
  }
}



