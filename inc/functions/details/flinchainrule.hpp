#ifndef __FLINCHAINRULE_HPP__
#define __FLINCHAINRULE_HPP__

#include <boost/ptr_container/ptr_vector.hpp>

#include "../ifunction.hpp"
#include "../iffeatures.hpp"
#include "../iderivative.hpp"

namespace functions
{
  namespace details
  {

    /// Implementation of the chain rule for f(g(x)) with linear g
    /**
     * This function provides derivatives for composite functions f(g(x)) where
     * f: R^n -> R is a black box and g(x): R^n -> R^n is linear in the form
     * M*x + C.
     *
     * f has to implement the IDerivative interface.
     * FLinChainRule is currently limited to second-order derivatives.
     *
     */
    template <typename T>
    class FLinChainRule : public IFunction, public IFFeatures, public IDerivative
    {
    public:
      /// dtor
      virtual ~FLinChainRule();

      /**
       * Copy Ctor
       *
       * @param other other object
       *
       */
      FLinChainRule(const FLinChainRule<T> &other);

     /**
      * Ctor
      *
      * @param f function f
      * @param m Affine mapping
      * @param c constant for affine mapping
      *
      */
      FLinChainRule(const IFunction &f, const typename core::arith::ArithTraits<T>::mat_t &m,
                    const typename core::arith::ArithTraits<T>::vec_t &c);

      /** 
       * Gets the matrix of the affine mapping
       * 
       * 
       * @return matrix of mapping
       */
      typename core::arith::ArithTraits<T>::mat_t get_m() const;

      /** 
       * Gets the constant part of the mapping
       * 
       * 
       * @return vector of mapping
       */
      typename core::arith::ArithTraits<T>::vec_t get_c() const;

    private:
      // IVFunction
      virtual core::arith::ivector calc_v_(const core::arith::ivector &args) const;
      virtual core::arith::avector calc_v_(const core::arith::avector &args) const;
      virtual core::arith::rvector calc_v_(const core::arith::rvector &args) const;
      virtual core::arith::tvector calc_v_(const core::arith::tvector &args) const;

      virtual unsigned dim_() const;
      virtual std::string text_() const;
      virtual IFunction* clone_() const;
      virtual const IFFeatures& features_() const;
      virtual unsigned cdim_() const;
      virtual const IFunction& fun_(unsigned i) const;
      

      // IFunction
      virtual core::arith::interval calc_(const core::arith::ivector &args) const;
      virtual core::arith::aaf calc_(const core::arith::avector &args) const;
      virtual core::arith::taylormodel calc_(const core::arith::tvector &args) const;
      virtual core::arith::mreal calc_(const core::arith::rvector &args) const;
    

      // IFFeatures
      virtual const IDerivative* derivative_() const;
      virtual const ICustomConsistency* custom_consistency_() const;
      virtual const ICustomEncl* custom_encl_() const;
      virtual const ITaylorCoeff* taylor_coeff_() const;
      virtual IGPUEval* gpu_eval_() const;

      // IDerivative
      virtual const IFunction& grad_(unsigned int i, unsigned m) const;
      virtual core::arith::ivector grad_(const core::arith::ivector &args, unsigned m) const;
      virtual core::arith::avector grad_(const core::arith::avector &args, unsigned m) const;
      virtual core::arith::tvector grad_(const core::arith::tvector &args, unsigned m) const;
      virtual core::arith::rvector grad_(const core::arith::rvector &args, unsigned m) const;
      virtual core::arith::rmatrix jacobian_(const core::arith::rvector &args) const;
      virtual core::arith::imatrix jacobian_(const core::arith::ivector &args) const;
      virtual core::arith::amatrix jacobian_(const core::arith::avector &args) const;
      virtual core::arith::tmatrix jacobian_(const core::arith::tvector &args) const;
      virtual const IVFunction& grad_fun_(unsigned member) const;
      
      // template<typename T_>
      // typename boost::disable_if<boost::is_same<T_, core::arith::mreal>, T_>::type
      // calcr(const core::arith::rvector &args) const;

    private:
      FLinChainRule(const FLinChainRule<T> &other, unsigned short dir);
      FLinChainRule(const FLinChainRule<T> &other, unsigned short dir, unsigned short dir2);
      
      FLinChainRule<T>& operator=(const FLinChainRule<T> &other);
      template<typename T_>
      typename boost::enable_if<boost::is_same<T_, core::arith::mreal>, core::arith::mreal>::type
      calcr(const core::arith::rvector &args) const;

      template<typename T_>
      typename boost::disable_if<boost::is_same<T_, core::arith::mreal>, core::arith::mreal>::type
      calcr(const core::arith::rvector &args) const;

      template <typename T_>
      typename T_::value_type calcdf(const T_& args) const;

      template <typename T_>
      typename boost::enable_if<boost::mpl::or_<boost::is_same<T, core::arith::mreal>,
						 boost::mpl::not_<boost::is_same<typename T_::value_type, core::arith::mreal> > >,T_>::type
      calc_grad(const T_& args) const;
  
      template <typename T_>
      typename boost::enable_if<boost::mpl::and_<boost::mpl::not_<boost::is_same<T, core::arith::mreal> >,
						 boost::is_same<typename T_::value_type, core::arith::mreal> > ,T_>::type
      calc_grad(const T_& args) const;

      template <typename T_>
      typename core::arith::ArithTraits<typename T_::value_type>::mat_t calc_jacobian(const T_& args) const;

    private:
      const IFunction& m_f;
      const typename core::arith::ArithTraits<T>::mat_t m_m;
      const typename core::arith::ArithTraits<T>::vec_t m_c;
      unsigned short m_dir;
      unsigned short m_dir2;
      boost::ptr_vector<FLinChainRule<T> > m_grad;



      // struct lin_dm_t
      // {
      //        std::vector<typename core::arith::ArithTraits<T>::mat_t> m_dm;
      // };

      // unsigned short m_order;
      // unsigned short *m_dirs;

      // lin_dm_t *m_dm;
    };
  }
}

#endif /*__FDERIVWRAPPER_HPP__*/
