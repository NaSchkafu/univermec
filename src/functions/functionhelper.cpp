#include <functions/functionhelper.hpp>

#include <functions/ifunction.hpp>
#include <functions/iffeatures.hpp>
#include <functions/iderivative.hpp>
#include <functions/ivfunction.hpp>
#include <functions/details/fvbase.hpp>
#include <functions/details/fderiv.hpp>

#include <functions/functionexceptions.hpp>

namespace functions
{
  template <typename T>
  typename core::arith::ArithTraits<typename T::value_type>::mat_t
  hessian(const IFunction &f, const T& args)
  {
    unsigned dim = f.dim();
    typename core::arith::ArithTraits<typename T::value_type>::mat_t h(dim, dim);
    const functions::IDerivative *df = f.features().derivative();
    const functions::IDerivative *ddfi = df->grad_fun(0).features().derivative();
    if(ddfi) {
      h[mtl::iall][mtl::iall] = ddfi->jacobian(args)[mtl::irange(1,dim+1)][mtl::irange(1,dim+1)];
      // for(unsigned i = 1; i <= dim; i++) {
      //   const functions::IFunction &dfi = df->grad(i);

      //   if(ddfi) {
      //     T gi(ddfi->grad(args));
      //     for(unsigned j = 0; j < dim; j++)
      //       h[i-1][j] = gi[j+1];
      //   }
      //   else {
      //     BOOST_THROW_EXCEPTION(functions_exception() << core::error_details("hessian is only applicable "
      //                                                                        "for two times differentiable functions!"));
      //   }
      // }
      return h;
    }
    BOOST_THROW_EXCEPTION(functions_exception() << core::error_details("hessian is only applicable "
                                                                       "for two times differentiable functions!"));
  }


  // // TODO 
  template <typename T>
  typename core::arith::ArithTraits<typename T::value_type>::mat_t
  jacobian(const IVFunction &f, const T& args, T* values, bool forward)
  {
    unsigned dim = f.dim();
     unsigned cdim = f.cdim();
    const functions::IDerivative *df = f.features().derivative();

    typename core::arith::ArithTraits<typename T::value_type>::mat_t jac(cdim, dim);
    if(forward && df) {
      for(unsigned i = 0; i < cdim; i++)
	for(unsigned j = 0; j < dim; j++)
	  jac[i][j] = df->grad(j+1, i)(args);
      if(values)
	*values = f(args);
      return jac;
    }
    else if(df) {
      auto tmp(df->jacobian(args));
      jac[mtl::iall][mtl::iall] = tmp[mtl::iall][mtl::irange(1, f.dim())];
      if(values)
	*values = tmp[mtl::iall][0];
      return jac;
    }
    
    BOOST_THROW_EXCEPTION(functions_exception() << core::error_details("jacobian is only applicable"
  								       " for differentiable functions!"));



    // for(unsigned i = 0; i < cdim; i++) {
    //   const functions::IDerivative *df = f[i].features().derivative();
    //   if(df) {
    //     if(!forward) {
    //       T gi(df->grad(args));
    //       for(unsigned j = 0; j < dim; j++)
    //         jac[i][j] = gi[j+1];
    //       if(values)
    //         (*values)[i] = gi[0];
    //     }
    //     else

    //   }
    //   else {
    //     BOOST_THROW_EXCEPTION(functions_exception() << core::error_details("jacobian is only applicable "
    //                                                                        "for differentiable functions!"));
    //   }
    // }
    // if(values && forward)
    //   (*values) = f.eval(args);
    //return jac;
  }


  template core::arith::rmatrix hessian(const IFunction &f, const core::arith::rvector &args);
  template core::arith::imatrix hessian(const IFunction &f, const core::arith::ivector &args);
  template core::arith::amatrix hessian(const IFunction &f, const core::arith::avector &args);
  template core::arith::tmatrix hessian(const IFunction &f, const core::arith::tvector &args);

  template core::arith::rmatrix jacobian(const IVFunction &f, const core::arith::rvector &args, core::arith::rvector*, bool);
  template core::arith::imatrix jacobian(const IVFunction &f, const core::arith::ivector &args, core::arith::ivector*, bool);
  template core::arith::amatrix jacobian(const IVFunction &f, const core::arith::avector &args, core::arith::avector*, bool);
  template core::arith::tmatrix jacobian(const IVFunction &f, const core::arith::tvector &args, core::arith::tvector*, bool);


  namespace details
  {
    struct constant_func_t
    {
      constant_func_t(const core::arith::interval &c, unsigned dim)
        :m_c(c), m_dim(dim)
        { }

      template<typename T>
      typename T::value_type operator()(const T &args) const
        {
          return forced_cast<typename T::value_type>(m_c);
        }

      // core::arith::mreal operator()(const core::arith::rvector& args) const
      //   {
      //     return forced_cast<core::arith::mreal>(m_c);
      //   }

      unsigned dim() const
        {
          return m_dim;
        }

      std::string text() const
        {
          std::stringstream sstream;
          sstream << "f(V) = " << m_c << " (|V| = " << m_dim << ")";
          return sstream.str();
        }


    private:
      const core::arith::interval m_c;
      unsigned m_dim;
    };

  }

  functions::IFunction* constant_func(const core::arith::interval &c, unsigned dim)
  {
    details::constant_func_t cf(c, dim);
    return new functions::details::FJoin<details::constant_func_t,
                                         functions::details::FDeriv<details::constant_func_t> >(cf);
  }

  functions::IFunction* constant_func(const core::arith::mreal &c, unsigned dim)
  {
    details::constant_func_t cf(core::arith::interval(c), dim);
    return new functions::details::FJoin<details::constant_func_t,
                                         functions::details::FDeriv<details::constant_func_t> >(cf);
  }

  namespace details
  {
    class ArgumentWrapperImpl : public IFunction, IFFeatures, IDerivative
    {
    public:
      ArgumentWrapperImpl(const functions::IFunction &f, unsigned int start,
                          unsigned int end, unsigned int dim)
        :m_f(f.clone()), m_df(m_f->features().derivative()), m_start(start), m_end(end), m_dim(dim)
        {
          assert(start < end);
          assert(end <= dim);
          assert(m_f->dim() == end - start);
          if(m_df) {
            for(unsigned i = 0; i < dim; i++) {
              if(i >= start && i < end)
                m_grad.push_back(new ArgumentWrapperImpl(m_df->grad(i - start + 1), start, end, dim));
              else
                m_grad.push_back(constant_func(0.0, m_dim));
            }
          }
        }

      virtual ~ArgumentWrapperImpl() {}

    private:
      // IFunction
      virtual core::arith::interval calc_(const core::arith::ivector &args) const
        {
          return (*m_f)(args[mtl::irange(m_start, m_end)]);
        }

      virtual core::arith::aaf calc_(const core::arith::avector &args) const
        {
          return (*m_f)(args[mtl::irange(m_start, m_end)]);
        }

      virtual core::arith::mreal calc_(const core::arith::rvector &args) const
        {
          //std::cout << core::arith::rvector(args[mtl::irange(m_start, m_end)]) << std::endl;
          return (*m_f)((args[mtl::irange(m_start, m_end)]));
        }

      virtual core::arith::taylormodel calc_(const core::arith::tvector &args) const
        {
          return (*m_f)(args[mtl::irange(m_start, m_end)]);
        }

      virtual core::arith::ivector calc_v_(const core::arith::ivector &args) const
        {
          return core::arith::ivector(1, calc_(args));
        }

      virtual core::arith::avector calc_v_(const core::arith::avector &args) const
        {
          return core::arith::avector(1, calc_(args));
        }


      virtual core::arith::tvector calc_v_(const core::arith::tvector &args) const
        {
          return core::arith::tvector(1, calc_(args));
        }


      virtual core::arith::rvector calc_v_(const core::arith::rvector &args) const
        {
          return core::arith::rvector(1, calc_(args));
        }

      virtual const functions::IFunction& fun_(unsigned m) const
        {
          assert(!m && "Invalid member requested!");
          return *this;
        }

      virtual unsigned cdim_() const
        {
          return 1;
        }

      virtual unsigned dim_() const
        {
          return m_dim;
        }

      virtual std::string text_() const
        {
          std::stringstream sstr;
          sstr << "ArgumentWrapperImpl (dim=" << m_dim << ", start=" << m_start
               << ", end=" << m_end << ", f=" << m_f->text() << ")";
          return sstr.str();
        }

      virtual IFunction* clone_() const
        {
          return new ArgumentWrapperImpl(*m_f, m_start, m_end, m_dim);
        }

      virtual const IFFeatures& features_() const
        {
          return *this;
        }

      virtual const IDerivative* derivative_() const
        {
          if(m_f->features().derivative())
            return this;
          return nullptr;
        }

      virtual const ICustomConsistency* custom_consistency_() const
        {
          // TODO: not supported
          return nullptr;
        }

      virtual const ICustomEncl* custom_encl_() const
        {
          // TODO: not supported
          return nullptr;
        }

      virtual const ITaylorCoeff* taylor_coeff_() const
        {
          // TODO: not supported
          return nullptr;
        }

      virtual IGPUEval* gpu_eval_() const
        {
          // TODO: not supported
          return nullptr;
        }


      virtual const IFunction& grad_(unsigned direction, unsigned m) const
        {
          assert(direction > 0 && direction <= m_dim && "Invalid gradient direction!");
          assert(!m && "Invalid member for gradient direction selected!");
          return m_grad[direction-1];
        }

      virtual core::arith::ivector grad_(const core::arith::ivector &args, unsigned m) const
        {
          assert(!m && "Invalid member for gradient direction selected!");
          core::arith::ivector res(m_dim+1, core::arith::interval(0.0));
          core::arith::ivector g(m_df->grad(args[mtl::irange(m_start, m_end)]));
          res[mtl::irange(m_start+1, m_end+1)] = g[mtl::irange(1, mtl::imax)];
          res[0] = g[0];
          return res;
        }

      virtual core::arith::avector grad_(const core::arith::avector &args, unsigned m) const
        {
          assert(!m && "Invalid member for gradient direction selected!");
          core::arith::avector res(m_dim+1, core::arith::aaf(0.0));
          core::arith::avector g(m_df->grad(args[mtl::irange(m_start, m_end)]));
          res[mtl::irange(m_start+1, m_end+1)] = g[mtl::irange(1, mtl::imax)];
          res[0] = g[0];
          return res;
        }

      virtual core::arith::tvector grad_(const core::arith::tvector &args, unsigned m) const
        {
          assert(!m && "Invalid member for gradient direction selected!");
          core::arith::tvector res(m_dim+1, core::arith::taylormodel(0.0));
          core::arith::tvector g(m_df->grad(args[mtl::irange(m_start, m_end)]));
          res[mtl::irange(m_start+1, m_end+1)] = g[mtl::irange(1, mtl::imax)];
          res[0] = g[0];
          return res;
        }

      virtual core::arith::rvector grad_(const core::arith::rvector &args, unsigned m) const
        {
          assert(!m && "Invalid member for gradient direction selected!");
          core::arith::rvector res(m_dim+1, 0.0);
          core::arith::rvector g(m_df->grad(args[mtl::irange(m_start, m_end)]));
          res[mtl::irange(m_start+1, m_end+1)] = g[mtl::irange(1, mtl::imax)];
          res[0] = g[0];
          return res;
        }

      template <typename T_>
      typename core::arith::ArithTraits<typename T_::value_type>::mat_t calc_jacobian(const T_& args) const
        {
          T_ rv(grad_(args, 0));
          typename core::arith::ArithTraits<typename T_::value_type>::mat_t rm(1, size(rv));
          for(unsigned i = 0; i < size(rv); i++)
            rm[0][i] = rv[i];
          return rm;
        }

      virtual core::arith::rmatrix jacobian_(const core::arith::rvector &args) const
        {
          return calc_jacobian(args);
        }

      virtual core::arith::imatrix jacobian_(const core::arith::ivector &args) const
        {
          return calc_jacobian(args);
        }

      virtual core::arith::amatrix jacobian_(const core::arith::avector &args) const
        {
          return calc_jacobian(args);
        }

      virtual core::arith::tmatrix jacobian_(const core::arith::tvector &args) const
        {
          return calc_jacobian(args);
        }

      virtual const IVFunction& grad_fun_(unsigned member) const
      {
	assert(!"Not IMPL!");
	return *((IVFunction*)0);
      }

    private:
      std::unique_ptr<functions::IFunction> m_f;
      const IDerivative *m_df;
      boost::ptr_vector<functions::IFunction> m_grad;
      unsigned m_start, m_end, m_dim;
    };
  }

  functions::IFunction* arg_wrapper(const functions::IFunction &f, unsigned int start,
                                    unsigned int end, unsigned int dim)
  {
    return new details::ArgumentWrapperImpl(f, start, end, dim);
  }


}

