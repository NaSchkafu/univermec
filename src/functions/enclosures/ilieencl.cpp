#include <functions/enclosures/ilieencl.hpp>

#include "functions/enclosures/details/hyperplane.hpp"
#include "functions/ifunction.hpp"

using namespace core::arith;

namespace functions
{
  namespace encl
  {
    bool ILIEEncl::is_global_() const
    {
      return false;
    }

    IFunction* ILIEEncl::calc_ilie(const avector &affBox, 
				   const aaf &encl,
				   const aaf::error_comb_t &affErr)
    {
#ifndef HAVE_AFFA
      unsigned dim(size(affBox));
      ivector ai(dim);

      interval j(0, 0);
      interval enclNoise(0, 0);

      aaf::error_comb_t comb(encl.comb());

      //unsigned int enclLength = encl.get_length();

      for(aaf::error_comb_t::const_iterator it(comb.begin()); it != comb.end(); ++it) {
        bool isEncl = true;
        for(unsigned l = 0; l < affErr.size(); l++) {
          if(*it == affErr[l]) {
            // if(abs(it->error()) < 10e-35)
            //   return 0;
            interval xi((1.0 / affErr[l].error())*it->error());
            enclNoise += affBox[l].central()*xi;
            // std::cout << "Error " << it->error() << " für " <<
            //   affErr[l].error() << " -> l " << l << std::endl;
            ai[l] = xi;
            isEncl = false;
          }
        }

        if(isEncl) {
          //std::cout << "J: " << it->error() << std::endl;
          j = interval(inf(interval(inf(j))-interval(abs(it->error()))),
                       sup(interval(sup(j)+abs(it->error()))));
          //std::cout << "j: " << j << std::endl;
        }
      }
      //std::cout << "Encl Noise: " << enclNoise << std::endl;
      j -= enclNoise;
      j += encl.central();
      // std::cout << "Box: " << box << "j:" << j << std::endl;
      //std::cout << "ai " << ai << " j: " << j << std::endl;
      return details::hyperplane(ai,-j);
#else
      return 0;
#endif
    }


    IFunction* ILIEEncl::encl_(const ivector &box) const
    {
      // Affine Formen berechnen
      unsigned dim = size(box);
      avector affBox(dim);
      aaf::error_comb_t affErr;
      for(unsigned i = 0; i < dim; i++) {
        affBox[i] = aaf(box[i]);
        const aaf::error_comb_t &ec = affBox[i].comb();
        if(ec.begin() != ec.end() && abs(ec.begin()->error()) > 10e-35)
          affErr.push_back(*ec.begin());
        else // Box ist degeneriert
          return 0;
      }

      // Einschluss mit aaf
      aaf encl(m_F(affBox));
      return calc_ilie(affBox, encl, affErr);
    }

/*
  IFunction* ILIEEncl::encl_(const::core::arith::ivector &box) const
  {
  #ifdef HAVE_AFFA
  // Affine Formen berechnen
  unsigned dim = size(box);
  avector affBox(dim);
  for(unsigned i = 0; i < dim; i++)
  affBox[i] = aaf(box[i]);

  // Einschluss mit aaf
  aaf encl0(m_F(affBox));
  // direkte affine form
  AAF& encl = encl0.get_affa();


  // Erzeugen des Fehlereinschlusses der ILIE
  ivector ai(dim);

  interval j(0, 0);
  interval enclNoise(0, 0);
  unsigned int enclLength = encl.get_length();

  for(unsigned int i = 0; i < enclLength; i++) {
  // Aufsummieren der Fehlersymbole die während des
  // affinen Einschlusses aufgetreten sind
  unsigned int idx = encl.get_index(i);
  bool isEncl = true;
  for(unsigned int l = 0; l < size(affBox) && isEncl; l++) {
  AAF &b = affBox[l].get_affa();
  // Es liegt ein Noisesymbol der Konvertierung IA->AA vor
  if(b.get_index(0) == idx) {
  // Sicherheitshalber mit Intervallen
  if(fabs(b.get_coeff(0)) <= 0.00000000001) {
  // Es fehlt ein Störsymbol
  // d.h. es liegt eine degenierte Box vor, das ILIE kann nicht berechnet werden
  return 0;
  }
  enclNoise += (interval(b.get_center()) / interval(b.get_coeff(0)))*encl.index_coeff(idx);
  // Laut Paper sind hier nur Reals erforderlich
  ai[l] = (1 / b.get_coeff(0))*encl.index_coeff(idx);
  isEncl = false;
  }
  }
  if(isEncl) {
  // Es liegt tatsächlich ein Störsymbole der affinen Funktionsauswertung vor
  // Da wir beide Richtungen getrennt berechnen, gerichtete Rundung statt IV Auswertung
  // eine Richtung reicht aus
  // int mode = fegetround();
//         fesetround(FE_UPWARD);
//         real infj = -(-inf(j) + abs(encl.get_coeff(i)));
//      real supj = sup(j) + abs(encl.get_coeff(i));
//      j = interval(infj, supj);
//         fesetround(mode);
j = interval(inf(interval(inf(j))-interval(fabs(encl.get_coeff(i)))),
sup(interval(sup(j)+fabs(encl.get_coeff(i)))));
}
}
j -= enclNoise;
j += encl.get_center();

// Erzeugen des ILIE
// ILIE L ist nun definiert durch L(x) = J + ai*x (x \in R^affBox.size())
return details::hyperplane(ai,-j);
#endif
assert(!"Not implemented");
return 0;
}
*/


    IFunction* ILIEEncl::encl_() const
    {
      return 0;
    }

    ILIEEncl::ILIEEncl(const IFunction &f)
      :m_F(f)
    { }

    ILIEEncl::~ILIEEncl()
    { }

  }
}
