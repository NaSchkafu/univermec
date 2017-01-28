/**
 * @file   johnconstraint.hpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:24:41 2009
 *
 * @brief  Header for Fritz-John-Cond.
 *
 *
 */


#ifndef __JOHNCONSTRAINTS_HPP__
#define __JOHNCONSTRAINTS_HPP__ __JOHNCONSTRAINTS_HPP__

#include "helper.hpp"
#include "simplefunction.hpp"
#include "types.hpp"

namespace GlobOpt
{

/* Aufbau des Inputvektors
 * (x, v) wobei x der Inputvektor der Zielfunktion sowie der Nebenbedingungen ist
 * d.h. die Zielfunktion und Nebenbedingungen m�ssen jeweils die gleichen Parameter entgegennehmen
 * v enth�lt die Lagrange Multiplikatoren die f�r die L�sung der Fritz-John-Bedingungen ben�tigt werden
 * d.h. u0...um wobei m die Anzahl der Ungleichheitsnebedingungen ist
 * Diese Implementierung der Fritz-John-Bedingungen erfasst nur solche Probleme,
 * Gleichheitsnebenbedingungen werden nicht behandelt
 *
 * Die Fritz Johnbedingungen sollten wie in Hansen 14.2.2 aufgebaut sein
 * D.h. die erste Funktion im Funktionenvektor Phi sollte die Normalisierunsbedigungen sein
 * darauffolgend die Funktionen die sich aus der JohnConstraint Klasse Konstruieren lassen
 * (entsprechend der Anzahl, der Eintr�ge im Eingabevektor der Zielfuktion)
 * Als letztes kommen die m Eintr�ge die jeweils mit Hilfe der Klasse
 * JohnInEqConstraint konstruiert werden.
 *
 * Der Einfachheit halber wird mit 1 basierten Indizies
 * gearbeitet. D.h. Lb((x,v)) = 1 und Ub((x,v)) = Ub(x) + m + 1
 */






// class JohnInEqConstraint : public C1Function {
// public:

//   /// Modes for constructing the equation
//   enum MODE { JOHN, KKT };

//   /*!

//   */
//   JohnInEqConstraint(unsigned int cNo,
//                      unsigned int dim, const C1Function& C, MODE mode = JOHN);
//   virtual ~JohnInEqConstraint() { }

//   // Function members
//   virtual inline unsigned int dim() const { return _dim; }

//   virtual std::string text() const;

// protected:
//   virtual inline core::arith::interval calc(const core::arith::ivector &args) const { return evalC(args); }
//   virtual inline core::arith::interval calc(const core::arith::rvector &args) const { return evalC(args); }

// private:
//   template<class T>
//   core::arith::interval evalC(const T &args) const
//     {
//       T x(_sizeX);
//       for(unsigned i = 1; i <= _sizeX; i++)
//         x[i] = args[i];
//       int idx = m_mode ? 1 : 0;
//       return args[_sizeX + idx + _cNo]*_C(x);
//     }

//   // C1 members
//   const Function& getGradF(unsigned int i) const { return _gradStore[i]; }

//   class MyGradStore {
//   public:
//     MyGradStore(unsigned int cNo, unsigned int dim,
//                 const C1Function& C, bool mode);
//     const Function& operator[](unsigned int i) const;
//     virtual ~MyGradStore();

//   private:
//     const unsigned int _sizeX;
//     const unsigned int _cNo;
//     const unsigned int _dim;
//     const SimpleFunction<__ZeroFunction> _zero;
//     std::vector<SimpleFunction<__Multiplier>* > _gradWrappers;
//     const SimpleFunction<__ArgumentWrapper> _c;
//     bool m_mode;
//   };


// };

  /// Mode
  enum MODE { JOHN, KKT };

  /// normaliztation condition for the John-Condition
  /*!
  ** The normalization condition for the John-Condition
  ** Basically this is the function u0 + u1 + ... + um - 1
  **
  */
  functions::IFunction* norm_const(unsigned offset, unsigned constraints);

  /*!
  ** Wrapper for the Fritz-John Condition (inequality constrained)
  ** u0*grad f(x) + u1*grad p1(x) + ... + um*grad pm(x)
  **
  ** @param cNo number of gradient member to be used (1 <= cNo <= sizeX)
  ** @param F objective function
  ** @param constraints inequality constraints
  ** @param mode used for creating the KKT-Conditions instead of the Fritz-John-Conditions
  **
  */
  functions::IFunction* john_constraint(unsigned int cNo, const functions::IFunction& F,
                                        const FLst& constraints, MODE mode = JOHN);
   /*!
   ** Wrapper for a inequality constraint in the John-Condition
   ** Adds the lagrange multipliere to a constraint for use in
   ** the Fritz-John Condition
   ** (Complementary slackness condition)
   ** 
   ** @param sizeX number of arguments of C
   ** @param cNo number of the constraint
   ** @param dim dimension of the function (input vector)
   ** @param C constraint function  
   * 
   */
  functions::IFunction* john_ineq(unsigned int cNo, unsigned int dim,
				  const functions::IFunction& C, MODE mode = JOHN);

}


#endif
