#include "stable.h"
#ifndef __ARITHCONCEPT_HPP__
#define __ARITHCONCEPT_HPP__

#include "boost/concept_check.hpp"
//#include "arithtraits.hpp"

template<class X>
struct ArithConcept : boost::Assignable<X>, boost::EqualityComparable<X>,
  boost::DefaultConstructible<X>, boost::CopyConstructible<X>
{
private:
  // Traits sicherstellen
  // typedef typename ArithTraits<X>::VectorT vector_type;
//   typedef typename ArithTraits<X>::MatrixT matrix_type;
  
public:
  BOOST_CONCEPT_USAGE(ArithConcept)
    {
      // Wir gehen davon aus dass die Klassen gutartig sind
      // insbesondere dass die const correctness eingehalten wird (keine wichtigen member 
      // als mutable etc)

      // X - scalar
      X x;
      const core::arith::mreal d(core::arith::ArithTraits<core::arith::mreal>::zero());
      x += d;
      x -= d;
      x *= d;
      x /= d;
      // Kopier ctor
      const X y(x);
      // TODO:
      // y == x sollte nun gelten
      // wichtig: beide Seiten sind nun const
      x = x + d;
      x = x - d;
      x = x * d;
      x = x / d;
      x = d + x;
      x = d - x;
      x = d * x;
      x = d / x;

      // X - X
      // nur y ist const
      x += y;
      x -= y;
      x *= y;
      x /= y;
      // beide seiten const
      const X z(y);
      x = z + y;
      x = z - y;
      x = z * y;
      x = z / y;

      // assignment
      x = y;

      // unary ops
      x = +y;
      x = -y;

      // Square / Square Root / cos / sin / e^real / x^n / ln(x) / x^real
      x = sqr(y);
      x = sqrt(y);
      x = cos(y);
      x = sin(y);
      x = exp(y);
      x = pow(y, 3);
      x = ln(y);
      x = pow(y, 3.1);
    }
};
#endif /*__ARITHCONCEPT_HPP__*/
