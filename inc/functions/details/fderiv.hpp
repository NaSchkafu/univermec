//#include "fvb.hpp"
#include "fjoin.hpp"
#include "autodiff.hpp"
#include "../iderivative.hpp"

// Suppport fuer Gradientenfunktionen
#include "fgradfun.hpp"

#include <static_config.hpp>

 namespace functions
 {
   namespace details
   {
    /// meta data structure for derivatives
    template<typename T>
    struct CData
    {
      /// ctor
      explicit CData(const T& f);

      /// copy ctor
      /**
       * Copying increases the gradient's directions store by 1 on every copy operation!
       */
      CData(const CData<T> &other, bool truecopy);

      ~CData();

      /// actual functor
      const T & m_f;
      /// size of m_dirs
      unsigned m_size;
      /// array containg the derivative directions
      unsigned *m_dirs;
      /// derivative of what member function)
      unsigned m_member;

    private:
      CData<T>(const CData<T>&);
      CData<T>& operator=(const CData<T>&);

    };

    /// functor class for derivatives
    /**
     * This functor is used for automatically providing directional
     * derivatives using fadbad. The original functor from the type T
     * is wrapped and used for calculating an derivative of order N
     *
     * The directions are determined by the m_data member, which also contains a reference
     * to the actual T object used for calculation.
     * 
     * The functor creates functions of the form R^n \to R, that is,
     * member functions of the Jacobian of the original function
     */
    template<typename T, int N>
    struct Deriv
    {
    public:
      /// ctor
      explicit Deriv(CData<T> *data);

      /// copy ctor
      Deriv(const Deriv<T,N> &other);

      unsigned dim() const;

      std::string text() const;

      const CData<T>& get_data() const;

      template<typename AT>
      typename AT::value_type operator()(const AT& args) const;

    private:
      std::auto_ptr<CData<T> > m_data;
      Deriv<T,N>& operator=(const Deriv<T,N> &other);
    };

    /// calculates automatic derivatives using fadbad
    /**
     * This class is used for automatically providing derivatives for a function created using
     * the FBase template. The derivatives are all calculated using the forward mode.
     * Member functions grad(i) are retrieved with forward mode
     * Whole gradients grad(args) are calculated using backward mode
     * Gradients of derivatives are calculated forward-backward
     * 
     * Furthermore this class implements the calculation of Taylor
     * coefficients using tadiff.
     * 
     * The format for the functor T is the same as specified for
     * FVBase. However, FDeriv calls the operator()(const X& args)
     * function additionally with types like F<X>, B<X>, F<F<X> >,
     * F<B<X> > and so on for automatic differentation. Make sure that
     * this possible (through a template) 
     * 
     * 
     */
    // Template Parameter
    // T: Aktiver Funktor
    // T1: Original Funktor der nicht abgeleiteten Funktion
    // N: Parameter der die template Rekursion abbricht, wenn alle Ableitungen erzeugt wurden
    // M: Maximale Anzahl von Ableitungen
    // Es gilt: FDeriv<T, T1, N, M> enthält die (M - N + 1)ten Ableitungen von T1
    template<typename T, typename T1 = T, int N = config::functions::DEFAULT_DERIV_ORDER, int M = N>
    class FDeriv : public IDerivative, public virtual FMaster<T>
    {
      using FMaster<T>::get_f;
      using FMaster<T>::f_cdim;
      BOOST_STATIC_ASSERT((M >= N) && N >= 0);

    public:
      FDeriv();

      virtual ~FDeriv();

      void init();

    private:
      typedef Deriv<T1, M-N+1> deriv_t;
      typedef f_grad_fun_deriv_t<T> grad_fun_deriv_t;

      template<typename T1_, typename T2_>
      typename boost::enable_if<boost::is_same<T1_, T2_>, const CData<T2_>& >::type get_data() const;

      template<typename T1_, typename T2_>
      typename boost::disable_if<boost::is_same<T1_, T2_>, const CData<T2_>& >::type get_data() const;

      template<typename T1_, typename T2_>
      typename boost::disable_if<boost::is_same<T1_, T2_> >::type clean_data(const CData<T2_> &) const;

      template<typename T1_, typename T2_>
      typename boost::enable_if<boost::is_same<T1_, T2_> >::type clean_data(const CData<T2_> &data) const;

      template<int N_>
      typename boost::enable_if_c<N_-1 != 0>::type create();

      template<int N_>
      typename boost::disable_if_c<N_-1 != 0>::type create();

      // ****************************************************************
      // IDerivative
      // ****************************************************************
      virtual const IFunction& grad_(unsigned dir, unsigned member) const;
      virtual core::arith::ivector grad_(const core::arith::ivector &args, unsigned member) const;
      virtual core::arith::avector grad_(const core::arith::avector &args, unsigned member) const;
      virtual core::arith::tvector grad_(const core::arith::tvector &args, unsigned member) const;
      virtual core::arith::rvector grad_(const core::arith::rvector &args, unsigned member) const;
      virtual core::arith::rmatrix jacobian_(const core::arith::rvector &args) const;
      virtual core::arith::imatrix jacobian_(const core::arith::ivector &args) const;
      virtual core::arith::amatrix jacobian_(const core::arith::avector &args) const;
      virtual core::arith::tmatrix jacobian_(const core::arith::tvector &args) const;
      // ****************************************************************      

      template<typename T_>
      typename boost::enable_if_c<is_scalar_functor_t<T>::is_scalar, T_>::type calc_grad(const T_ &args, unsigned member) const;
      template<typename T_>
      typename boost::disable_if_c<is_scalar_functor_t<T>::is_scalar, T_>::type calc_grad(const T_ &args, unsigned member) const;

      template<typename T_>
      typename boost::enable_if_c<is_scalar_functor_t<T>::is_scalar, typename core::arith::ArithTraits<typename T_::value_type>::mat_t>::type calc_jacobian(const T_ &args) const;
      template<typename T_>
      typename boost::disable_if_c<is_scalar_functor_t<T>::is_scalar, typename core::arith::ArithTraits<typename T_::value_type>::mat_t>::type calc_jacobian(const T_ &args) const;
      virtual const IVFunction& grad_fun_(unsigned member) const;

    private:
      /// storage for gradient functions
      boost::ptr_vector<IVFunction> m_grad_fun;
      /// storage for gradient members
      std::shared_ptr<g_mem_store_t> m_g_store;
    };

#include "fderiv.inl"
   }
 }
