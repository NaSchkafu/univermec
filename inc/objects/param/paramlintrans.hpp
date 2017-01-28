#ifndef __PARAMLINTRANS_HPP__
#define __PARAMLINTRANS_HPP__

#include "../iparamtrans.hpp"

namespace functions
{
  namespace details 
  {
    template <typename T>
    class FLinChainRule;

    template <typename T>
    class FVBase;
  }
}


namespace objects 
{
  namespace details 
  {
    template <typename T, bool>
    struct lin_trans_t;

    /// represents a linear transformation of a parametric surface
    template<typename T>
    class ParamLinTrans : public IParamTrans
    {
    public:
 
      /** 
       * Ctor
       * Constructs a linear transformation for a parametric surface
       * The implicit representation is automatically adjusted using the inverse transformations
       * 
       * @param child parametric surface
       * @param r linear scaling
       * @param ir inverse scaling
       * @param t linear translation
       * @param it inverse translation
       */
      ParamLinTrans(IParamSurface *child, 
		    const typename core::arith::ArithTraits<T>::mat_t &r, 
		    const typename core::arith::ArithTraits<T>::mat_t &ir,
		    const typename core::arith::ArithTraits<T>::vec_t &t,
		    const typename core::arith::ArithTraits<T>::vec_t &it);

      /** 
       * Sets the linear transform of this node
       * 
       * @param r matrix transform
       * @param ir inverse matrix transform
       * @param t addition
       * @param it inverse addition
       */
      void set_t_s(const typename core::arith::ArithTraits<T>::mat_t &r, 
		   const typename core::arith::ArithTraits<T>::mat_t &ir,
		   const typename core::arith::ArithTraits<T>::vec_t &t,
		   const typename core::arith::ArithTraits<T>::vec_t &it);

      virtual ~ParamLinTrans();

    private:
      // IParamSurface
      virtual const IParamSurface& child_() const;
      virtual unsigned d_dim_() const;
      virtual core::arith::ivector domain_() const;
      virtual const functions::IVFunction& p_fun_() const;
      virtual const functions::IVFunction* normals_() const;

      // IGeoObj
      virtual const functions::IFunction& cf_() const;
      virtual IGeoObj* clone_() const;
      virtual unsigned dim_() const;
      virtual const functions::IVFunction* p_normals_() const;

    private:
      // Pointer Child node
      std::unique_ptr<IParamSurface> m_child;
      // Transform IGeoObj
      typedef functions::details::FLinChainRule<T> cf_fun_t;
      std::unique_ptr<cf_fun_t> m_cf;
      // Normals IGeoObj
      std::unique_ptr<functions::IVFunction> m_nf;
      // Transform Parametric
      typedef functions::details::FVBase<lin_trans_t<T, false> > pf_fun_t;
      std::unique_ptr<pf_fun_t> m_pf;
      // Normals Parametric
      typedef functions::details::FVBase<lin_trans_t<T, true> >  pnf_fun_t;
      std::unique_ptr<pnf_fun_t> m_npf;
    };
  }
}

#endif /*__PARAMLINTRANS_HPP__*/
