dnl Adapter file for creating ArithTraits specializations
dnl automatically for all registered arithmetics.
#ifndef __ARITHTRAITS_HPP__
#define __ARITHTRAITS_HPP__

#include "core/arithcore.hpp"

/* Die Traitsklassen erlauben es Funktionen zu definieren die mit allen
 * zur Verfügung stehenden Arithmetikkonzepten funktionieren.
 *
 * Eine wichtige Funktion ist die Konvertierung unterschiedlicher Arithmetiktypen ineinander.
 * Zu diesem Zweck wird die template Klasse ArithTraits für alle Arithmetiktypen spezialisiert.
 * Jede Spezialisierung stellt eine Implementirung und Überladungen der convert-Funktion für alle
 * Arithmetikkonzepte zur Verfügung. 
 *
 * Bsp.: ArithTraits<interval>::convert(double) ---> ok (konvertiert double in ein Intervall)
 *       ArithTraits<interval>::convert(interval) ---> ok
 *       ArithTraits<interval>::convert(aaf) ---> Informationsverlust aber ok
 *
 * Zusätzlich stehen die Traits Funktionen auch für die fadbad Typen und mtl-Vektoren bzw. mögliche
 * derselben zur Verfügung. Die Generierung erfolgt hier automatisch über templates auf Basis der
 * zulässigen Konvertierungen der zugrundeliegenden Basistypen (double, interval, etc).
 *
 * Um einen Informationsverlust zu vermeiden sollte nicht auf die ArithTraits<T>::convert() Funktionen direkt
 * zugegriffen, sondern auf die darauf basierenden cast Operatoren lossless_cast<>, enclosure_cast<>, forced_cast<>
 *
 */

#include "utility/fadbad_helper.hpp"
dnl
divert(-1)

dnl Helper macros for creating the standard B_PERFORM_CAST_XXX_YYYY macros
dnl User defined macros are not overriden by the standard ones, so you can customize the casting behaviour
dnl in your adapter files.
dnl interval -> mreal (midpoint)
ifdef(`B_PERFORM_CAST_interval_mreal',`',`pushdef(`B_PERFORM_CAST_interval_mreal',`return mid(other);')')
dnl mreal -> interval
ifdef(`B_PERFORM_CAST_mreal_interval',`',`pushdef(`B_PERFORM_CAST_mreal_interval',`return interval(other);')')

dnl XXX -> mreal (encl midpoint)
define(`CREATE_MREAL_CASTS',`ifelse($#,0,`CREATE_MREAL_CASTS(CLASS_LIST)',`
ifdef(`B_PERFORM_CAST_$1_mreal',`',`pushdef(`B_PERFORM_CAST_$1_mreal',`return mid(ArithTraits<interval>::convert(other));')')
ifelse($#,1,`',`CREATE_MREAL_CASTS(shift($@))')')')
CREATE_MREAL_CASTS

dnl other -> ???
define(`CREATE_OTHER_CASTS',`ifelse($#,1,`CREATE_OTHER_CASTS($1,CLASS_LIST)',`
ifdef(`B_PERFORM_CAST_$2_$1',`',`pushdef(`B_PERFORM_CAST_$2_$1',`return $1(other`'dnl
ifelse(eval(ifelse($2,`mreal',`1',`0')+ifelse($2,`interval',`1',`0')),1,`',`.encl()'));')')
ifelse($#,2,`',`CREATE_OTHER_CASTS($1,shiftn(2,$@))')')')
define(`CREATE_ALL_OTHER_CASTS',`ifelse($#,0,`CREATE_ALL_OTHER_CASTS(CLASS_LIST)',`CREATE_OTHER_CASTS($1)
ifelse($#,1,`',`CREATE_ALL_OTHER_CASTS(shift($@))')')')
CREATE_ALL_OTHER_CASTS

dnl Macro for creating a concrete cast
define(`CREATE_CASTS',`ifelse($#,0,`CREATE_CASTS(CLASS_LIST)', `
inline static CLASS_NAME convert(const $1`'& other)
{
ifelse(CLASS_NAME,`$1',`return other;',`indir(B_PERFORM_CAST_$1_`'CLASS_NAME)')
}
ifelse($#,1,`',`CREATE_CASTS(shift($@))')dnl
')')

dnl Creates a Trat for the current CLASS_NAME
define(`CREATE_TRAITS',`
    /// Arithmetic traits specialization for CLASS_NAME
    template<>
    class ArithTraits<CLASS_NAME, void>
    {
    public:
      typedef mtl::dense_vector<CLASS_NAME> vec_t;
      typedef mtl::dense2D<CLASS_NAME> mat_t;
      typedef CLASS_NAME base_t;
      typedef void arith_type_t;

      inline static base_t zero()
        {
          return base_t(0.0);
        }

      inline static base_t one()
        {
          return base_t(1.0);
        }

      CREATE_CASTS      

      // fadbad Typen
      template<typename AD>
      inline static typename boost::enable_if<utility::details::has_underlying_type<AD>, base_t>::type convert(const AD& ad)
      {
	return convert(ad.val());
      }
    private:
      ArithTraits();
    };
')

define(`CREATE_ALL_TRAITS',`ifelse($#,0,`CREATE_ALL_TRAITS(CLASS_LIST)',`
pushdef(`CLASS_NAME',`$1')dnl
CREATE_TRAITS
popdef(`CLASS_NAME')dnl
ifelse($#,1,`',`CREATE_ALL_TRAITS(shift($@))')')')

divert(0)dnl
dnl
namespace core
{
  namespace arith
  {

    namespace details
    {
      void downcast_error();
      BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_value_type, value_type, true)
    }

    template<typename T, typename T1 = void>
    class ArithTraits
    {
    public:

    private:
      ArithTraits();
    };

    dnl Reorders the list, so that interval is first followed by mreal,
    dnl as the standard implementation of the mreal convert() function
    dnl uses the interval traits
    define(`CLASS_LIST_TMP',`shiftn(2,dequote(defn(`CLASS_LIST')))')dnl
    pushdef(`CLASS_LIST',quote2(`interval',`mreal',CLASS_LIST_TMP))dnl
    CREATE_ALL_TRAITS`'dnl
    popdef(`CLASS_LIST')dnl
    undefine(`CLASS_LIST_TMP')dnl

    // fadbad typen Forward
    template<typename T>
    class ArithTraits<T, typename boost::enable_if<boost::mpl::and_<utility::details::is_forward<utility::details::fadbad_detector_t<T> >,
        utility::details::has_underlying_type<T> > >::type>
    {                                                           
    private:
      typedef T traits_t;

    public:
      typedef typename utility::details::strip_fadbad<T>::real_t base_t;
      typedef void arith_type_t;      

      inline static traits_t convert(const traits_t &t)
      {
        return t;
      }
      
      template<typename AT>
      static traits_t convert(const AT &t)
      {	
        return traits_t(ArithTraits<base_t>::convert(utility::details::fadbad_val<AT>::val(t)));
      }

      typedef mtl::dense_vector<traits_t> vec_t;
      typedef mtl::dense2D<traits_t> mat_t;
    };

    // fadbad typen Taylor
    template<typename T>
    class ArithTraits<T, typename boost::enable_if<boost::mpl::and_<utility::details::is_taylor<utility::details::fadbad_detector_t<T> >,
        utility::details::has_underlying_type<T> > >::type>
    {                                                           
    private:
      typedef T traits_t;

    public:
      typedef typename utility::details::strip_fadbad<T>::real_t::UnderlyingType base_t;
      typedef void arith_type_t;      

      inline static traits_t convert(const traits_t &t)
      {
        return t;
      }
      
      template<typename AT>
      static traits_t convert(const AT &t)
      {	
        return traits_t(ArithTraits<base_t>::convert(utility::details::fadbad_val<AT>::val(t)));
      }

      typedef mtl::dense_vector<traits_t> vec_t;
      typedef mtl::dense2D<traits_t> mat_t;
    };


    // fadbad typen Backward
    template<typename T>
    class ArithTraits<T, typename boost::enable_if<boost::mpl::and_<utility::details::is_backward<utility::details::fadbad_detector_t<T> >,
        utility::details::has_underlying_type<T> > >::type>
    {                                                           
    private:
      typedef T traits_t;

    public:
      typedef typename utility::details::strip_fadbad<T>::real_t::UnderlyingType base_t;
      typedef void arith_type_t;      

      inline static traits_t convert(const traits_t &t)
      {
        return t;
      }
      
      template<typename AT>
      static traits_t convert(const AT &t)
      {	
        return traits_t(ArithTraits<base_t>::convert(utility::details::fadbad_val<AT>::val(t)));
      }

      typedef mtl::dense_vector<traits_t> vec_t;
      typedef mtl::dense2D<traits_t> mat_t;
    };


    // Vektoren und Matrizen
    namespace details 
    {
      BOOST_MPL_HAS_XXX_TRAIT_DEF(orientation)
    }
    // Nur Matrizen
    namespace details 
    {
      BOOST_MPL_HAS_XXX_TRAIT_DEF(sub_matrix_type)
    }

    // TODO: Besserer Trick um Vektoren zu erkennen?
    template<typename T>
    class ArithTraits<T, typename boost::enable_if<boost::mpl::and_<details::has_orientation<T>, 
								    boost::mpl::not_<details::has_sub_matrix_type<T> > > >::type>
    {                
    private:
      typedef T vec_t;

    public:
      typedef typename vec_t::value_type base_t;
      typedef void arith_type_t;

      inline static vec_t convert(const vec_t &v)
      {
        return v;
      }

      template<typename TV>
      static vec_t convert(const TV &tv)
      {
        unsigned dim = size(tv);
        vec_t v(dim);
        for(unsigned i = 0; i < dim; i++)
          v[i] = ArithTraits<base_t>::convert(tv[i]);
        return v;
      }
    };

    // Matrizen
    template<typename T>
    class ArithTraits<T, typename boost::enable_if<details::has_sub_matrix_type<T> >::type>
    {
    private:
      typedef T mat_t;

    public:
      typedef typename mat_t::value_type base_t;
      typedef void arith_type_t;

      inline static mat_t convert(const mat_t &v)
      {
        return v;
      }

      template<typename TV>
      static mat_t convert(const TV &tv)
	{
	  unsigned rows = num_rows(tv);
	  unsigned cols = num_cols(tv);
	  mat_t v(rows, cols);
	  for(unsigned i = 0; i < rows; i++)
	    for(unsigned j = 0; j < cols; j++)
	      v[i][j] = ArithTraits<base_t>::convert(tv[i][j]);
	  return v;
	}
    };
  }
}

  
#endif
