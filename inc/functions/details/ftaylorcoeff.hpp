#ifndef __FTAYLORCOEFF_HPP__
#define __FTAYLORCOEFF_HPP__

#include "fjoin.hpp"
#include "../itaylorcoeff.hpp"

namespace functions
{
  namespace details 
  {
    template <typename T>
    class FTaylorCoeff : public ITaylorCoeff, public virtual FMaster<T> 
    {
    public:
      using FMaster<T>::f_cdim;
      using FMaster<T>::get_f;

      FTaylorCoeff();
      virtual ~FTaylorCoeff();
      void init();
      
    private:
      virtual std::vector<core::arith::rvector> taylor_coeff_ode_(const core::arith::rvector &args, unsigned order) const;
      virtual std::vector<core::arith::ivector> taylor_coeff_ode_(const core::arith::ivector &args, unsigned order) const;
      virtual std::vector<core::arith::avector> taylor_coeff_ode_(const core::arith::avector &args, unsigned order) const;
      virtual std::vector<core::arith::tvector> taylor_coeff_ode_(const core::arith::tvector &args, unsigned order) const;

      virtual std::vector<std::vector<core::arith::rvector> > taylor_coeff_ode_df_(const core::arith::rvector &args, unsigned order) const;
      virtual std::vector<std::vector<core::arith::ivector> > taylor_coeff_ode_df_(const core::arith::ivector &args, unsigned order) const;
      virtual std::vector<std::vector<core::arith::avector> > taylor_coeff_ode_df_(const core::arith::avector &args, unsigned order) const;
      virtual std::vector<std::vector<core::arith::tvector> > taylor_coeff_ode_df_(const core::arith::tvector &args, unsigned order) const;

      template <typename T_>
      std::vector<T_> calc_taylor_coeff_ode(const T_& args, unsigned order) const;

      template <typename T_>
      std::vector<std::vector<T_> > calc_taylor_coeff_ode_df(const T_& args, unsigned order) const;
    };

    #include "ftaylorcoeff.inl"
  }
}

#endif /*__FTAYLORCOEFF_HPP__*/
