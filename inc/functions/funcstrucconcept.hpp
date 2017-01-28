#include "stable.h"
#include "boost/concept_check.hpp"
//#include "arithtraits.hpp"
namespace functions
{

  template<class X>
  struct FuncStrucConcept : boost::DefaultConstructible<X>
  {
  public:
    BOOST_CONCEPT_USAGE(FuncStrucConcept)
      {
	const X func;
	unsigned int dim = func.dim(); // dimension
	std::string fasel = func.text(); // text function

	// Arithmetiken
	// TODO: Arithmetiken einfügen

      }
  };

}
