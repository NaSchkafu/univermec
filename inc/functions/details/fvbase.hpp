/*!
**@file   fvbase.hpp
**@author Stefan Kiel
**@date   Thu Sep 10 13:47:30 2009
**
**@brief  Contains the complete template infrastructure for creating a valid framework function
**
**
*/

#ifndef __FBASE_HPP__
#define __FBASE_HPP__

#include "../ifunction.hpp"
#include "../iffeatures.hpp"
//#include "../funcstrucconcept.hpp"

//#include "cdwrap.hpp"
#include "fmaster.hpp"
#include "isscalarfunctor.hpp"


// some helper's for fadbad

// #include "debug_helper.hpp"

namespace functions
{
  class IVFunction;

  /// Implementation of the interfaces in @ref functions and some utility functions.
  namespace details
  {

    // template<typename T1, typename T2>
    // class FDummy
    // {
    // };

    /// base class for creating functions
    /**
     * The FBase template provides the functions defined in the interface IVFunction.
     * The calls are redelegated to a functor of type T.
     *
     * Functor
     *
     * Methoden:
     * operator()(const T& args) const -> T (mit T = {r,i,a,t}vector)
     * -> Funktionsauswertung am Punkt T
     * 
     * dim() const -> unsigned
     * Anzahl der Eingangsvariablen
     *
     * cdim() const -> unsigned
     * Anzahl der Ausgabevariablen
     *
     * text() const -> std::string
     * Textuelle Beschreibung der Funktion
     *
     * Anmerkung:
     * Ist statt operator(const T& args) const -> T eine Funktion mit der
     * Signatur
     * operator()(const T& args) const -> T::value_type vorhanden
     * wird die Funktion als skalare Funktion (d.h. R^n -> R) behandelt.
     * D.h. FVBase erbt dann automatisch von IFunction (und nur indirekt von IVFunction)
     *
     *
     * This class also handles the storage of the functor and provides it through the FMaster<T> interface
     * to its sister classes.
     */
    template<typename T>
    class FVBase : public is_scalar_functor_t<T>::functor_t,
                   public virtual FMaster<T>, public virtual IFFeatures
    {
    public:
      /**
       * Default ctor
       *
       * Creates a new FBase object based on the Functortype T using it's default ctor
       */
      FVBase();

      /**
       * Ctor
       *
       * @param struc Functor instance
       *
       * Creates a new FBase object based on the functortype T. For this it creates a copy of struc
       */
      explicit FVBase(const T& struc);

      /**
       * Ctor
       *
       * @param struc Functor instance
       *
       * Creates a new FBase object based on the functortype T. The ownership of struc is transfered
       * to the newly created object and should not be manipulated any further by the caller.
       */
      explicit FVBase(T* struc);

      // FBase(const CHAIN &chain);

      // FBase(const T& struc, const CHAIN &chain);

      // FBase(T* struc, const CHAIN &chain);


      /**
       * Destructor
       *
       */
      virtual ~FVBase() {  }

    private:
      void init();

      // FMaster
      virtual T& get_f_();
      virtual const T& get_f_() const;
      virtual unsigned f_cdim_() const;

      // IVFunction
      virtual core::arith::ivector calc_v_(const core::arith::ivector &args) const;
      virtual core::arith::avector calc_v_(const core::arith::avector &args) const;
      virtual core::arith::rvector calc_v_(const core::arith::rvector &args) const;
      virtual core::arith::tvector calc_v_(const core::arith::tvector &args) const;

      //virtual const IFunction& fun_(unsigned i) const;
      virtual unsigned dim_() const;
      virtual unsigned cdim_() const;
      virtual typename is_scalar_functor_t<T>::functor_t* clone_() const;
      virtual std::string text_() const;

      virtual const IFFeatures& features_() const;

      // IFeatures
      virtual const IDerivative* derivative_() const;
      virtual const ICustomConsistency* custom_consistency_() const;
      virtual const ICustomEncl* custom_encl_() const;
      virtual const ITaylorCoeff* taylor_coeff_() const;
      virtual IGPUEval* gpu_eval_() const;

      // IFunction
      // Diese Funktionen sind immer vorhanden aber nur aufrufbar
      // falls T ein skalarer Funktor ist und damit FVBase von
      // IFunction erbt
      virtual core::arith::interval calc_(const core::arith::ivector &args) const;
      virtual core::arith::aaf calc_(const core::arith::avector &args) const;
      virtual core::arith::mreal calc_(const core::arith::rvector &args) const;
      virtual core::arith::taylormodel calc_(const core::arith::tvector &args) const;


      FVBase<T>(const FVBase<T>&);
      FVBase<T>& operator=(const FVBase<T>&);

    private:
      std::unique_ptr<T> m_struc;
      //boost::ptr_vector<IFunction> m_members;
      static const bool S_IS_SCALAR = is_scalar_functor_t<T>::is_scalar;
    };


#include "fvbase.inl"
  }
}

#endif /*__FBASE_HPP__*/
