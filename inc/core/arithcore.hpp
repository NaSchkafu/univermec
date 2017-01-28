/*!
**@file   arithcore.hpp
**@author Stefan Kiel
**@date   Thu Sep 10 10:30:39 2009
**
**@brief  Use this file to include the core arithmetic
**
**
*/

#ifndef __ARITHCORE_HPP__
#define __ARITHCORE_HPP__

// MTL
#include <boost/numeric/mtl/mtl.hpp>

// fadbad
// TODO: Refactoring...should not be always included
#include <fadiff.h>
#include <badiff.h>
#include <tadiff.h>

// Adapter
#include "adapter.hpp"
// Real arithmetik
#include "doubleadapt.hpp"
#include "mrealadapt.hpp"
// fadbad Specializations
#include "fadbadadapt.hpp"
// Traits
#include "arithtraits.hpp"
//conversions
#include "conversion.hpp"

#include "interval_util.hpp"

// Konzept FIXME
//#include "arithconcept.hpp"

namespace core 
{
  typedef unsigned int size_t;
  
  namespace arith 
  {
    // Typedefs
    typedef ArithTraits<interval>::vec_t ivector;
    typedef ArithTraits<interval>::mat_t imatrix;
    
    typedef ArithTraits<mreal>::vec_t rvector;
    typedef ArithTraits<mreal>::mat_t rmatrix;    

    typedef ArithTraits<aaf>::vec_t avector;
    typedef ArithTraits<aaf>::mat_t amatrix;

    typedef ArithTraits<taylormodel>::vec_t tvector;
    typedef ArithTraits<taylormodel>::mat_t tmatrix;

    // Konzeptcheck
    // BOOST_CONCEPT_ASSERT((ArithConcept<interval>));
    // //BOOST_CONCEPT_ASSERT((ArithConcept<real>));
    // BOOST_CONCEPT_ASSERT((ArithConcept<aaf>));
    // BOOST_CONCEPT_ASSERT((ArithConcept<taylormodel>));
  }
}


#endif /*__ARITHCORE_HPP__*/
