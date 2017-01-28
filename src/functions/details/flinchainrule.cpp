#include <functions/details/flinchainrule.hpp>
#include <functions/functionhelper.hpp>

using namespace core::arith;
using mtl::iall;


namespace functions
{
  namespace  details
  {
    template <typename T>
    FLinChainRule<T>::FLinChainRule(const IFunction &f, const typename ArithTraits<T>::mat_t &m, 
				    const typename ArithTraits<T>::vec_t &c)
      :m_f(f), m_m(m), m_c(c), m_dir(0), m_dir2(0), m_grad(0)
    {
      assert(m_f.features().derivative() && "Construction of derivative wrapper for a non "
	     " differentiable function requested!");
      if(m_f.features().derivative()) {
	// FLinChainRule<T> *tmpStore = (FLinChainRule<T>*)malloc(m_f.dim() * sizeof(FLinChainRule<T>));
	for(unsigned i = 0; i < m_f.dim(); i++) {
	  // std::cout << tmpStore << " " << tmpStore+i*sizeof(FLinChainRule<T>) << std::endl;
	  // new(tmpStore+i*sizeof(FLinChainRule<T>)) FLinChainRule<T>(*this, i+1);
	  m_grad.push_back(new FLinChainRule<T>(*this, i+1));
	}
	// m_grad = (FLinChainRule<T>*)tmpStore;
      }
    }

    template <typename T>
    FLinChainRule<T>::FLinChainRule(const FLinChainRule<T> &other)
      :m_f(other.m_f), m_m(other.m_m), m_c(other.m_c), m_dir(other.m_dir), m_dir2(other.m_dir),
       m_grad(0)
    {
      //if(!other.m_dir) {
	// FLinChainRule<T> *tmpStore = (FLinChainRule*)malloc(m_f.dim() * sizeof(FLinChainRule<T>));
      if(other.m_grad.size())
	for(unsigned i = 0; i < m_f.dim(); i++)
	  m_grad.push_back(new FLinChainRule<T>(other.m_grad[i])) ;
	  // new(tmpStore+i*sizeof(FLinChainRule<T>)) FLinChainRule<T>(other.m_grad[i]);
	// m_grad = (FLinChainRule<T>*)tmpStore;
	//}
    }

    // Ctor 1. Ableitung Members
    template <typename T >
    FLinChainRule<T>::FLinChainRule(const FLinChainRule<T> &other, unsigned short dir)
      :m_f(other.m_f), m_m(other.m_m), m_c(other.m_c), m_dir(dir), m_dir2(0), m_grad(0)
    { 
      if(m_f.features().derivative()->grad(m_dir).features().derivative())
	for(unsigned i = 1; i <= m_f.dim(); i++)
	  m_grad.push_back(new FLinChainRule<T>(other, dir, i));
    }

    // Ctor 2. Ableitung Members (optional)
    template <typename T>
    FLinChainRule<T>::FLinChainRule(const FLinChainRule<T> &other, unsigned short dir, unsigned short dir2)
      :m_f(other.m_f), m_m(other.m_m), m_c(other.m_c), m_dir(dir), m_dir2(dir2), m_grad(0)
    {}

    template <typename T>
    FLinChainRule<T>::~FLinChainRule()
    {
      // if(m_grad) {
      // 	for(unsigned i = 0; i < m_f.dim(); i++)
      // 	  m_grad[i].~FLinChainRule();
      // 	free((void*)m_grad);
      // }
    }

    template<typename T>
    template<typename T_>
    typename T_::value_type FLinChainRule<T>::calcdf(const T_& args) const 
    {
      T_ nargs(m_m*T_(args+m_c));

      if(m_dir == 0)
	return m_f(nargs);
      else if(!m_dir2 && m_dir <= m_f.dim()) {
	T_ df(m_f.features().derivative()->grad(nargs));
	return dot(df[mtl::irange(1, mtl::imax)], m_m[mtl::iall][m_dir-1]);
      }
      else if(m_dir <= m_f.dim() && m_dir2 <= m_f.dim()) {
	auto ddf(hessian(m_f, nargs));
	T_ v(ddf*m_m[mtl::iall][m_dir-1]);
	return dot(v, m_m[mtl::iall][m_dir2-1]);
      }	
      assert(!"Invalid derivative order!");
    }

    // TODO: Provide derivatives with a general implementation of the chain rule
    template <typename T>
    interval FLinChainRule<T>::calc_(const ivector &args) const
    {
      return calcdf(args);
      assert(!"Invalid derivative order!");
    }

    template <typename T>
    aaf FLinChainRule<T>::calc_(const avector &args) const
    {
      return calcdf(args);
    }

    template <typename T>
    taylormodel FLinChainRule<T>::calc_(const tvector &args) const
    {
      return calcdf(args);
    }

    template<typename T>
    mreal FLinChainRule<T>::calc_(const rvector &args) const 
    {
      return calcr<T>(args);
    }

      // IVFunction
    template <typename T>
    core::arith::ivector FLinChainRule<T>::calc_v_(const core::arith::ivector &args) const
    {
      return core::arith::ivector(1, calc_(args));
    }
    
    template <typename T>
    core::arith::avector FLinChainRule<T>::calc_v_(const core::arith::avector &args) const
    {
      return core::arith::avector(1, calc_(args));
    }
    
    template <typename T>
    core::arith::rvector FLinChainRule<T>::calc_v_(const core::arith::rvector &args) const
    {
      return core::arith::rvector(1, calc_(args));
    }
    
    template <typename T>
    core::arith::tvector FLinChainRule<T>::calc_v_(const core::arith::tvector &args) const
    {
      return core::arith::tvector(1, calc_(args));
    }
    
    

    template <typename T>
    template <typename T_>
    typename boost::enable_if<boost::is_same<T_, core::arith::mreal>, core::arith::mreal>::type 
    FLinChainRule<T>::calcr(const core::arith::rvector &args) const
    {
      return calcdf(args);
    }

    template <typename T>
    template <typename T_>
    typename boost::disable_if<boost::is_same<T_, core::arith::mreal>, core::arith::mreal>::type 
    FLinChainRule<T>::calcr(const core::arith::rvector &args) const
    {
      rmatrix m(forced_cast<rmatrix>(m_m));
      rvector nargs(m*rvector(args+forced_cast<rvector>(m_c)));

      if(m_dir == 0)
	return m_f(nargs);
      else if(!m_dir2 && m_dir <= m_f.dim()) {
	rvector df(m_f.features().derivative()->grad(nargs));
	return dot(df[mtl::irange(1, mtl::imax)], m[mtl::iall][m_dir-1]);
      }
      else if(m_dir <= m_f.dim() && m_dir2 <= m_f.dim()) {
	auto ddf(hessian(m_f, nargs));
	rvector v(ddf*m[mtl::iall][m_dir-1]);
	return dot(v, m[mtl::iall][m_dir2-1]);
      }	
      assert(!"Invalid derivative order!");
    }

    template <typename T>
    std::string FLinChainRule<T>::text_() const
    {
      std::stringstream sstr;
      sstr << "FLinChainRule for " << m_f.text();
      return sstr.str();
    }

    template <typename T>
    IFunction* FLinChainRule<T>::clone_() const
    {
      return new FLinChainRule(*this);
    }

    template <typename T>
    const IFFeatures& FLinChainRule<T>::features_() const
    {
      return *this;
    }

    template <typename T>
    const IDerivative* FLinChainRule<T>::derivative_() const
    {
      if(m_grad.size())
	return this;
      return 0;
    }

    template <typename T>
    const ICustomConsistency* FLinChainRule<T>::custom_consistency_() const
    {
      return m_f.features().custom_consistency();
    }

    template <typename T> 
    const ICustomEncl* FLinChainRule<T>::custom_encl_() const
    {
      return m_f.features().custom_encl();
    }

    template <typename T> 
    const ITaylorCoeff* FLinChainRule<T>::taylor_coeff_() const
    {
      return nullptr;
    }

    template <typename T> 
    IGPUEval* FLinChainRule<T>::gpu_eval_() const
    {
      return nullptr;
    }


    template <typename T>
    const IFunction& FLinChainRule<T>::grad_(unsigned int i, unsigned m) const 
    {
      assert(m_grad.size() && i > 0 && i <= m_f.dim() && !m && "Illegal gradient direction requested!");
      return m_grad[i-1];
    }

    template <typename T>
    template <typename T_>
    typename boost::enable_if<boost::mpl::or_<boost::is_same<T, core::arith::mreal>,
					       boost::mpl::not_<boost::is_same<typename T_::value_type, core::arith::mreal> > >,T_>::type
    FLinChainRule<T>::calc_grad(const T_ &args) const
    {
      assert(m_grad.size() && "Non existent gradient requested!");
      T_ nargs(m_m*T_(args+m_c));
      T_ df(m_f.dim()+1);
      if(!m_dir) {
	df = m_f.features().derivative()->grad(nargs);
	T_ tmp(mtl::vec::trans(df[mtl::irange(1, mtl::imax)])*m_m);
	df[mtl::irange(1, mtl::imax)] = tmp;
	return df;
      } 
      else {
	auto ddf(hessian(m_f, nargs));
	T_ v(ddf*m_m[mtl::iall][m_dir-1]);
	for(unsigned i = 0; i < m_f.dim(); i++)
	  df[i+1] = dot(v, m_m[mtl::iall][i]);
	df[0] = calc_(args);
      }
      return df;
    }
    
    template <typename T>
    template <typename T_>
    typename boost::enable_if<boost::mpl::and_<boost::mpl::not_<boost::is_same<T, core::arith::mreal> >,
					       boost::is_same<typename T_::value_type, core::arith::mreal> > ,T_>::type
    FLinChainRule<T>::calc_grad(const T_& args) const
    {
//      assert(!m_dir && m_grad.size() && "Second derivative requested, but support by underlying function");
//      assert(!m_dir2 && "Third derivative requested, but not supported by FLinChainRule<T>!");
      assert(m_grad.size() && "Non existent gradient requested!");      
      rmatrix m(forced_cast<rmatrix>(m_m));
      T_ nargs(m*T_(args + forced_cast<rvector>(m_c)));
      T_ df(m_f.dim()+1);
      if(!m_dir) {
	df = m_f.features().derivative()->grad(nargs);
	T_ tmp(mtl::vec::trans(df[mtl::irange(1, mtl::imax)])*m);
	df[mtl::irange(1, mtl::imax)] = tmp;
	return df;
      } 
      else {
	auto ddf(hessian(m_f, nargs));
	T_ v(ddf*m[mtl::iall][m_dir-1]);
	for(unsigned i = 0; i < m_f.dim(); i++)
	  df[i+1] = dot(v, m[mtl::iall][i]);
	df[0] = calc_(args);
      }
      return df;
    }

    template <typename T>
    template <typename T_>
    typename core::arith::ArithTraits<typename T_::value_type>::mat_t FLinChainRule<T>::calc_jacobian(const T_& args) const
      {
	T_ rv(grad_(args, 0));
	typename core::arith::ArithTraits<typename T_::value_type>::mat_t rm(1, size(rv));
	for(unsigned i = 0; i < size(rv); i++)
	  rm[0][i] = rv[i];
	return rm;
      }

    template <typename T>
    core::arith::rmatrix FLinChainRule<T>::jacobian_(const core::arith::rvector &args) const
      {
	return calc_jacobian(args);
      }

    template <typename T>    
    core::arith::imatrix FLinChainRule<T>::jacobian_(const core::arith::ivector &args) const
      {
	return calc_jacobian(args);
      }

    template <typename T>    
    core::arith::amatrix FLinChainRule<T>::jacobian_(const core::arith::avector &args) const
      {
	return calc_jacobian(args);
      }

    template <typename T>    
    core::arith::tmatrix FLinChainRule<T>::jacobian_(const core::arith::tvector &args) const
      {
	return calc_jacobian(args);
      }


    template <typename T>
    ivector FLinChainRule<T>::grad_(const ivector& args, unsigned m) const
    {
      assert(!m && "Invalid member function requested!");
      return calc_grad(args);
    }

    template <typename T>
    rvector FLinChainRule<T>::grad_(const rvector& args, unsigned m) const
    {
      assert(!m && "Invalid member function requested!");
      return calc_grad(args);
    }

    template <typename T>
    tvector FLinChainRule<T>::grad_(const tvector& args, unsigned m) const
    {
      assert(!m && "Invalid member function requested!");
      return calc_grad(args);
    }

    template <typename T>
    avector FLinChainRule<T>::grad_(const avector& args, unsigned m) const
    {
      assert(!m && "Invalid member function requested!");
      return calc_grad(args);
    }

    template <typename T>
    unsigned FLinChainRule<T>::dim_() const 
    {
      return m_f.dim();
    }

    template <typename T>
    typename ArithTraits<T>::mat_t FLinChainRule<T>::get_m() const
    {
      return m_m;
    }
    
    template <typename T>
    typename ArithTraits<T>::vec_t FLinChainRule<T>::get_c() const
    {
      return m_c;
    }

    template <typename T>
    unsigned FLinChainRule<T>::cdim_() const
    {
      return 1;
    }
      

    template <typename T>
    const IFunction& FLinChainRule<T>::fun_(unsigned i) const
    {
      assert(!i && "Invalid member function requested!");
      return *this;
    } 

    template <typename T>
    const IVFunction& FLinChainRule<T>::grad_fun_(unsigned member) const
      {
	assert(!"Not IMPL!");
	return *((IVFunction*)0);
      }    

    template class FLinChainRule<interval>;
    template class FLinChainRule<mreal>;

  }
}


