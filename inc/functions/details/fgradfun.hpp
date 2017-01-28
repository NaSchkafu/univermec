#ifndef __FGRADFUN_HPP__
#define __FGRADFUN_HPP__

#include <boost/ptr_container/ptr_vector.hpp>

#include "../iderivative.hpp"
#include "../ivfunction.hpp"
#include "../ifunction.hpp"
#include "../iffeatures.hpp"

namespace functions
{
  class IFunction;
  class IVFunction;
  
  namespace details
  {
    /// This structure is shared between FDeriv and FGradFun
    /**
     * The purpose of this structure is to store the directional
     * derivatives of FDeriv. They can be reused by FGradFun. It
     * should also referred to by a shared_ptr to avoid problems if
     * clone is called on a FDeriv or FGradFun function.
     */
     struct g_mem_store_t 
     {
       /// storage for gradient members
       std::vector<boost::ptr_vector<IFunction> > m_grad;
     };

    /// Functor class for gradient functions
     /** 
      * This functor is used for automatically creating derivatives
      * using fadbad. It constrast to the Deriv functor it creates
      * derivatives of the form R^n \to \R^n, that is, a row of the
      * Jacobian of the original function.
      * It is used by the FGradFun class
      */
    template <typename T>
    struct f_grad_fun_deriv_t
    {
    public:
      f_grad_fun_deriv_t(const T& func, const std::shared_ptr<g_mem_store_t>& m_store, 
			 unsigned member);
      virtual ~f_grad_fun_deriv_t() {}
      
      //f_grad_fun_deriv_t(const f_grad_fun_deriv_t<T> &other);
      unsigned dim() const;
      unsigned cdim() const;
      std::string text() const;
      template <typename T_>
      T_ operator()(const T_& args) const;

      unsigned member() const;
      const g_mem_store_t& store() const;

    private:
      T m_func;
      std::shared_ptr<g_mem_store_t> m_store;
      const unsigned m_member;
    };


    /// A helper for representing the gradient of a function as
    /// function
    /**
     * This function is used internally by the FDeriv class to
     * represent the gradient of a member function R^n \to R^m as a
     * function of type R^n \to R^n. It reuses the already generated
     * directional derivatives of FDeriv where possible.
     */
    template<typename T>
    class FGradFun : public IDerivative, public virtual FMaster<T>
    {
    public:
      // FGradFun(const IDerivative &parent, unsigned member)
      //   :m_parent(parent), m_member(member)
      //   {}
      using FMaster<T>::get_f;
      using FMaster<T>::f_cdim;
      void init();
      virtual ~FGradFun()
	{ }

    private:
      virtual const IFunction& grad_(unsigned dir, unsigned member) const;
      virtual core::arith::ivector grad_(const core::arith::ivector &args, unsigned member) const;
      virtual core::arith::avector grad_(const core::arith::avector &args, unsigned member) const;
      virtual core::arith::tvector grad_(const core::arith::tvector &args, unsigned member) const;
      virtual core::arith::rvector grad_(const core::arith::rvector &args, unsigned member) const;
      virtual core::arith::rmatrix jacobian_(const core::arith::rvector &args) const;
      virtual core::arith::imatrix jacobian_(const core::arith::ivector &args) const;
      virtual core::arith::amatrix jacobian_(const core::arith::avector &args) const;
      virtual core::arith::tmatrix jacobian_(const core::arith::tvector &args) const;
      virtual const IVFunction& grad_fun_(unsigned member = 0) const;
      
      template <typename T_>
      typename core::arith::ArithTraits<typename T_::value_type>::mat_t calc_jacobian(const T_ &args) const;
    };

    #include "fgradfun.inl"

  }
}

#endif /*__FGRADFUN_HPP__*/
