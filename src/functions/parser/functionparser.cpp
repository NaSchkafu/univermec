#include <functions/parser/functionparser.hpp>

#include <functions/details/fvbase.hpp>
#include <functions/details/fderiv.hpp>
#include <functions/details/ftaylorcoeff.hpp>
#include "parser.inl"

namespace functions
{
  namespace parser 
  {
    IFunction* parse_string(const std::string &fstr, unsigned dim)
    {
      details::FParser f(fstr,dim);
      return new functions::details::FJoin<details::FParser, 
					   functions::details::FDeriv<details::FParser>, functions::details::FTaylorCoeff<details::FParser> >(f);
    }
  }
}
