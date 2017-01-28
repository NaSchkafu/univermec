/**
 * @file   simplefunction.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:28:29 2009
 *
 * @brief  Implementation of simple functions for gradients
 *
 *
 */


#include <algorithm/opt-lib/simplefunction.hpp>
#include <algorithm/opt-lib/exception.hpp>

#include "functions/details/fvbase.hpp"

namespace GlobOpt {
  
  // struct ConstantFunction 
  // {
  //   ConstantFunction(const core::arith::interval &c, unsigned dim)
  //     :m_c(c), m_dim(dim)
  //     { }

  //   template<typename T>
  //   typename T::value_type operator()(const T &args) const
  //     {
  //       return lossless_cast<typename T::value_type>(m_c);
  //     }

  //   core::arith::mreal operator()(const core::arith::rvector& args) const
  //     {
  //       return forced_cast<core::arith::mreal>(m_c);
  //     }

  //   unsigned dim() const
  //     {
  //       return m_dim;
  //     }

  //   std::string text() const
  //     {
  //       std::stringstream sstream;
  //       sstream << "f(V) = " << m_c << " (|V| = " << m_dim << ")";
  //       return sstream.str();
  //     }


  // private:
  //   const core::arith::interval m_c;
  //   unsigned m_dim;
  // };

  // struct ArgWrapper
  // {
  //   ArgWrapper(const functions::IFunction &F, unsigned int start, 
  // 	       unsigned int end, unsigned int dim)
  //     :m_F(F), m_start(start), m_end(end), m_dim(dim)
  //     {
  //       assert(m_F.dim() == end - start + 1);
  //     }

  //   std::string text() const
  //     {
  //       std::stringstream sstream;
  //       sstream << "f(v) = g(args) (with args = v(start, end) and g = " 
  // 		<< m_F.text();
  //       return sstream.str();
  //     }

  //   template <typename T>
  //   typename T::value_type operator()(const T& args) const  
  //     {
  // 	return m_F(args[mtl::irange(m_start, m_end)]);
  //     }

  //   unsigned dim() const
  //     {
  // 	return m_dim;
  //     }
    
  // private:
  //   const functions::IFunction &m_F;
  //   unsigned m_start;
  //   unsigned m_end;
  //   unsigned m_dim;
  // };
  
  struct Multiplier {
    Multiplier(unsigned sizeX, unsigned cNo, const functions::IFunction &G, unsigned dim)
      :m_sizeX(sizeX), m_cNo(cNo), m_G(G), m_dim(dim)
      {}

    template <typename T>
    typename T::value_type operator()(const T& args) const
      {
	return args[m_sizeX + 1 +m_cNo]*m_G(MT::copyVec(args,m_sizeX));
      }

    std::string text() const 
      {
	std::stringstream sstr;
	sstr << "Funktion multiplierer: args[" << m_sizeX + 1 + m_cNo << "] * ";
	sstr << m_G.text() << " with arguments " << "args[0..." << m_sizeX << "]"
	     << "Dimension " << m_dim;
	return sstr.str();
      }

    unsigned dim() const 
      {
	return m_dim;
      }

  private:
    unsigned m_sizeX;
    unsigned m_cNo;
    const functions::IFunction& m_G;
    unsigned m_dim;
  };

  functions::IFunction* multi_func(unsigned sizeX, unsigned cNo, 
				   const functions::IFunction &G, unsigned dim)
  {
    Multiplier mf(sizeX, cNo, G, dim);
    return new functions::details::FVBase<Multiplier>(mf);
  }

  // functions::IFunction* constant_func(const core::arith::interval &c, unsigned dim)
  // {
  //   ConstantFunction cf(c, dim);
  //   return new functions::details::FJoin<ConstantFunction,
  //                                        functions::details::FDeriv<ConstantFunction> >(cf);
  // }

  // functions::IFunction* constant_func(const core::arith::mreal &c, unsigned dim)
  // {
  //   ConstantFunction cf(core::arith::interval(c), dim);
  //   return new functions::details::FJoin<ConstantFunction,
  //                                        functions::details::FDeriv<ConstantFunction> >(cf);
  // }

  // functions::IFunction* arg_wrapper(const functions::IFunction &f, 
  // 				    unsigned int start, unsigned int end,
  // 				    unsigned int dim)
  // {
  //   ArgWrapper aw(f, start, end, dim);
  //   return new functions::details::FBase<ArgWrapper>(aw);
  // }


// // ********************************************************************************
// // Impl of ArgWrapper





}

