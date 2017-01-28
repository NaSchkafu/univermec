#include "functions/enclosures/nencl.hpp"

#include <functions/ifunction.hpp>
#include "functions/details/fvbase.hpp"
#include "functions/details/fderiv.hpp"
#include "functions/details/fcustomconst.hpp"

// TODO C11
#include <boost/scoped_ptr.hpp>

using namespace core::arith;


namespace functions
{
  namespace encl
  {
    namespace details
    {
      struct tm_eval_t
      {
        tm_eval_t(const functions::IFunction &f)
          :m_f(f.clone())
          {assert(m_f && "clone() operation in tm_eval_t failed!");}

        tm_eval_t(const tm_eval_t &other)
          :m_f(other.m_f->clone())
          {   }

        template <typename T>
        typename T::value_type operator()(const T& args) const
          {
            char name[9];
            unsigned dim = size(args);
            assert(dim <= 10 && "Maximum 10 Variables supported. Increase array size for more!");
            core::arith::tvector tx(dim);
	    core::arith::ivector iargs(enclosure_cast<core::arith::ivector>(args));
            for(unsigned i = 0; i < dim; i++) {
	      sprintf(name, "tmencl%d",i);
	      tx[i] = mid(iargs[i])+0.5*wid(iargs[i])*core::arith::taylormodel(name);
	      //tx[i] = core::arith::create_tm_var(enclosure_cast<interval>(args[i]), name);
	    }
            return enclosure_cast<typename T::value_type>((*m_f)(tx));
          }

        taylormodel operator()(const tvector &args)
          {
            return (*m_f)(args);
          }

        unsigned dim() const
          {
            return m_f->dim();
          }

        std::string text() const
          {
            std::stringstream sstr;
            sstr << "Natural TaylorModel Enclosure for " << m_f->text();
            return sstr.str();
          }

      private:
        boost::scoped_ptr<functions::IFunction> m_f;
      };

      struct aaf_eval_t
      {
        aaf_eval_t(const functions::IFunction &f)
          :m_f(f.clone())
          {assert(m_f && "clone() operation in aaf_eval_t failed!");}

        aaf_eval_t(const aaf_eval_t &other)
          :m_f(other.m_f->clone())
          {   }

        template <typename T>
        typename T::value_type operator()(const T& args) const
          {
            core::arith::avector ax(enclosure_cast<avector>(args));
            return enclosure_cast<typename T::value_type>((*m_f)(ax));
          }

        aaf operator()(const avector &args)
          {
            return (*m_f)(args);
          }

        unsigned dim() const
          {
            return m_f->dim();
          }

        std::string text() const
          {
            std::stringstream sstr;
            sstr << "Natural Affine Enclosure for " << m_f->text();
            return sstr.str();
          }

      private:
        boost::scoped_ptr<functions::IFunction> m_f;
      };

      struct iv_eval_t
      {
        iv_eval_t(const functions::IFunction &f)
          :m_f(f.clone())
          {assert(m_f && "clone() operation in iv_eval_t failed!");}

        iv_eval_t(const iv_eval_t &other)
          :m_f(other.m_f->clone())
          {   }

        template <typename T>
        typename T::value_type operator()(const T& args) const
          {
            core::arith::ivector ix(enclosure_cast<ivector>(args));
            return enclosure_cast<typename T::value_type>((*m_f)(ix));
          }

        interval operator()(const ivector &args)
          {
            return (*m_f)(args);
          }

        unsigned dim() const
          {
            return m_f->dim();
          }

        std::string text() const
          {
            std::stringstream sstr;
            sstr << "Natural Interval Enclosure for " << m_f->text();
            return sstr.str();
          }

      private:
        boost::scoped_ptr<functions::IFunction> m_f;
      };
    }

    template <int N >
    NEncl<N>::NEncl(const functions::IFunction &f)
      :m_F(f)
    {}

    template <int N >
    bool NEncl<N>::is_global_() const
    {
      return true;
    }

    template <int N >
    IFunction* NEncl<N>::encl_(const ivector&) const
    {
      return encl_();
    }

    template <int N >
    IFunction* NEncl<N>::encl_() const
    {
      switch(N) {
      case 0:
        return new functions::details::FVBase<details::iv_eval_t>(details::iv_eval_t(m_F));
      case 1:
        return new functions::details::FVBase<details::aaf_eval_t>(details::aaf_eval_t(m_F));
      case 2:
        return new functions::details::FVBase<details::tm_eval_t>(details::tm_eval_t(m_F));
      };
      return 0;
    }

    template class NEncl<0>;
    template class NEncl<1>;
    template class NEncl<2>;
  }
}
