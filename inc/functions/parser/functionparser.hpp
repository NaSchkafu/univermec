#ifndef __FUNCTIONPARSER_HPP__
#define __FUNCTIONPARSER_HPP__

#include <string>

namespace functions
{
  class IFunction;

  /// Namespace for the function parser
  /**
   * The function parser is the preferred way to construct an IVFunction representation
   * for a specific scalar-valued mathematical function \f$f: \Rz^n \to \Rz\f$ if this 
   * function can be represented by a closed-form analytical expression.
   * 
   * Example:
   * If we want to represent \f$f(x) := x_0^2 + x_1^2 + x_2^2 -1 \f$ (the
   * implicit function of a unit sphere), we can use the parser by:
   * ~~~~~~~~~~~~~~~~~~~~~
   * IFunction *sphere_fun = parse_string("sqr(x0)+sqr(x1)+sqr(x2)-1");
   * ~~~~~~~~~~~~~~~~~~~~~
   * The returned function supports all inclusion functions specified in IFunction.
   * Additionally, the parser adds automatically support for [derivatives](@ref IDerivative) 
   * to [feature set](@ref IFFeatures) of the function which are obtained by 
   * algorithmic differentiation up to the default order defined in static_config.hpp.
   * 
   * The syntax of the parser is straightforwardly based on standard mathematical notation.
   * Input variables are addressed by \f$x_1, \ldots, x_n\f$. Elementary functions are
   * have the same names as in the @ref core layer which in turn is oriented on
   * the upcoming P1788 interval standard \cite P1788044.
   */
  namespace parser
  {
    /** 
     * Parses a analytical expression for a mathematical function and creates a new IFunction instance
     * representing this function inside the framework.
     * 
     * If the dim parameter is ommitted, the dimension is calculated automatically based on the
     * highest variable index \f$x_1, \ldots, x_n\f$ occuring in fstr.
     * 
     * If fstr could not be parsed a parser_exception is thrown.
     * 
     * @param fstr analytical expression
     * @param dim dimension of the function
     * 
     * @return newly created function
     */
    IFunction* parse_string(const std::string &fstr, unsigned dim = 0);
  }
}

#endif /*__FUNCTIONPARSER_HPP__*/
