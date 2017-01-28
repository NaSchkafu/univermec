#ifndef __FJOIN_HPP__
#define __FJOIN_HPP__

#include "../iffeatures.hpp"
#include "../iderivative.hpp"
#include "../icustomconsistency.hpp"
#include "../icustomencl.hpp"
#include "../itaylorcoeff.hpp"
#include "../igpueval.hpp"

#include "fvbase.hpp"

namespace functions
{
  namespace details
  {

    template<int N>
    class FDummy { public: void init() {} };

    template<typename T, typename T1, typename T2 = FDummy<0>, typename T3 = FDummy<1>,
             typename T4 = FDummy<2>, typename T5 = FDummy<3>, typename T6 = FDummy<4> >
    class FJoin : public FVBase<T>, public T1,  public T2, public T3, public T4, public T5, public T6
    {
    public:
      FJoin()
        {
          T1::init();
          T2::init();
          T3::init();
          T4::init();
          T5::init();
          T6::init();
        }

      FJoin(const T& struc)
        :FVBase<T>(struc)
        {
          T1::init();
          T2::init();
          T3::init();
          T4::init();
          T5::init();
          T6::init();
        }

      FJoin(T* struc)
        :FVBase<T>(struc)
        {
          T1::init();
          T2::init();
          T3::init();
          T4::init();
          T5::init();
          T6::init();
        }
      virtual ~FJoin()
        {}

      using FMaster<T>::get_f;

      // private:
      // std::bitset<3> m_features;
    private:
      typedef FJoin<T, T1, T2, T3, T4, T5, T6> my_t;

      typename is_scalar_functor_t<T>::functor_t* clone_() const;
      virtual const IFFeatures& features_() const;

      // Feature erkennung ;-)
      template<typename T_>
      typename boost::enable_if<boost::is_base_of<T_, my_t>,
                                const T_*>::type  detect_feature() const;
      template<typename T_>
      typename boost::disable_if<boost::is_base_of<T_, my_t>,
				 const T_*>::type detect_feature() const;


      // IFFeatures
      virtual const IDerivative* derivative_() const;
      virtual const ICustomConsistency* custom_consistency_() const;
      virtual const ICustomEncl* custom_encl_() const;
      virtual const ITaylorCoeff* taylor_coeff_() const;
      virtual IGPUEval* gpu_eval_() const;
      
      FJoin<T, T1, T2, T3, T4, T5, T6>(const my_t&);
      my_t& operator=(const my_t&);
  };

#include "fjoin.inl"

  }
}
#endif /*__FJOIN_HPP__*/
