/**
 * @file   johnconstraint.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:24:22 2009
 *
 * @brief  Impl. of Fritz-John-Cond.
 *
 *
 */

#include <algorithm/opt-lib/johnconstraint.hpp>

// TODO C11
#include <boost/foreach.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "functions/ifunction.hpp"
#include "functions/iffeatures.hpp"
#include "functions/iderivative.hpp"
#include "functions/details/fvbase.hpp"
#include "functions/details/fjoin.hpp"
#include "functions/functionhelper.hpp"

#include <algorithm/opt-lib/exception.hpp>
#include <algorithm/opt-lib/types.hpp>

using namespace functions;

namespace GlobOpt
{

  template <typename T>
  class DerivImpl : public IDerivative, public virtual details::FMaster<T>
  {
  public:
    virtual ~DerivImpl() {}

    void init() {}

  private:
    const IFunction& grad_(unsigned dir, unsigned m) const
      {
        return this->get_f().grad(dir, m);
      }

    core::arith::ivector grad_(const core::arith::ivector &args, unsigned m) const
      {
        return this->get_f().grad(args, m);
      }

    core::arith::avector grad_(const core::arith::avector &args, unsigned m) const
      {
        return this->get_f().grad(args, m);
      }

    core::arith::tvector grad_(const core::arith::tvector &args, unsigned m) const
      {
        return this->get_f().grad(args, m);
      }

    core::arith::rvector grad_(const core::arith::rvector &args, unsigned m) const
      {
        return this->get_f().grad(args, m);
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
  };

  /// gradient member function for the JohnConstraint class
  /*!
  ** This class is used for the gradient members 1 <= i <= k, with k =
  ** F.dim() (F ist the objective function of our problem)
  **
  */
  struct JohnConstraintGrad  {
    /*!
    ** Constructor
    **
    ** @param cNo number of the johnconstraint (1 <= cNo <= k)
    ** @param gradDir direction of this gradient member (1 <= gradDir <= k)
    ** @param F objective function
    ** @param constraints list of constraints
    */
    JohnConstraintGrad(unsigned int cNo, unsigned int gradDir, const IFunction &F,
                       const FLst &constraints, bool mode)
      :m_cNo(cNo), m_gradDir(gradDir), m_F(F), m_constraints(constraints), m_mode(mode)
      {
        assert(m_cNo >= 1 && m_cNo <= m_F.dim());
        assert(m_gradDir >= 1 && m_gradDir <= m_F.dim());
      }

    template <typename T>
    typename T::value_type operator()(const T& args) const
      {
        T x(args[mtl::irange(0, m_F.dim())]);
        // kein Lag-Mult u0 falls KKT-Mode aktiv ist
        typename T::value_type mul(1.0);
        if(m_mode)
          mul = args[m_F.dim()];

        // u0 * grad _F (cNo member funktion s.u.)
        typename T::value_type result(mul*m_F.features().derivative()->grad(m_cNo).features().derivative()->grad(m_gradDir)(x));
        for(FLst::size_type i = 0; i < m_constraints.size(); i++) {
          /* ui*grad pi(x) [jeweils die cNo-te member funktion
           * des Gradienten, wir starten bei u1 da u0 au�erhalb der
           Schleife behandelt wird*/
          result += args[m_F.dim() + (m_mode ? 2 : 1) + i]*
            (m_constraints[i].get().features().derivative()->grad(m_cNo).features().derivative()->grad(m_gradDir)(x));
        }
        return result;
      }

    unsigned int dim() const
      {
        return m_F.dim() + m_constraints.size() + (m_mode ? 1 : 0);
      }

    std::string text() const
      {

      }

  private:
    const unsigned int m_cNo;
    const unsigned int m_gradDir;
    const IFunction &m_F;
    const FLst &m_constraints;
    const bool m_mode;
  };


/*!
** Wrapper for the Fritz-John Condition (inequality constrained)
** u0*grad f(x) + u1*grad p1(x) + ... + um*grad pm(x)
**
**
*/
  struct JohnConstraint
  {
    JohnConstraint(unsigned int cNo, const IFunction& F,
                   const FLst& constraints, MODE mode)
      :m_sizeX(F.dim()), m_cNo(cNo), m_F(F), m_constraints(constraints), m_mode(mode == JOHN)
      {
        create_grad();
      }

    JohnConstraint(const JohnConstraint &other)
      :m_sizeX(other.m_F.dim()), m_cNo(other.m_cNo), m_F(other.m_F),
       m_constraints(other.m_constraints), m_mode(other.m_mode)
      {
        create_grad();
      }

    void create_grad()
      {
        assert(m_F.features().derivative() && m_F.features().derivative()->grad(0).features().derivative()
               && "Second order informations required for JohnConstraint!");

        // Konstruktion des Gradienten
        // Fall 1 (d.h. 1 <= i <= k) (Abl in Richtung x1...xk)
        for(unsigned int i = 1; i <= m_sizeX; i++)
	  m_gradMembers.push_back(new details::FVBase<JohnConstraintGrad>(JohnConstraintGrad(m_cNo, i, m_F, m_constraints, m_mode)));

        // Fall 2a (j = k+1), Abl. in Richtung u0
        if(m_mode)
          m_gradMembers.push_back(arg_wrapper(m_F.features().derivative()->grad(m_cNo), 1, m_sizeX, dim()));

        // ableitungen in richtung der lagrange multiplikatoren u1...um
        // (Fall 2b, d.h. j != k+1 und k < j <=n)
        BOOST_FOREACH(const IFunction& func, m_constraints)
        {
          m_gradMembers.push_back(arg_wrapper(func.features().derivative()->grad(m_cNo), 1, m_sizeX, dim()));
        }
      }

    template<class T>
    typename T::value_type operator()(const T &args) const
      {
        T x(m_sizeX);
        x = args[mtl::irange(0, m_sizeX - 1)];
        // core::arith::ivector x(MT::copyVec(args, m_sizeX, 0));
        // u0 * grad _F (cNo member funktion s.u.)a
        typename T::value_type mul(1.0);
        if(m_mode)
          mul = args[m_sizeX];

        typename T::value_type result(mul*m_F.features().derivative()->grad(m_cNo)(x));
        for(FLst::size_type i = 0; i < m_constraints.size(); i++) {
          /* ui*grad pi(x) [jeweils die cNo-te member funktion
           * des Gradienten, wir starten bei u1 da u0 au�erhalb der
           Schleife behandelt wird*/
          result += args[m_sizeX + (m_mode ? 1 : 0) + i]*
            (m_constraints[i].get().features().derivative()->grad(m_cNo)(x));
        }
        return result;
      }

    std::string text() const
      {
        std::stringstream sstream;
        sstream << "f(v) = ";
        sstream << (m_mode ? "u0*(" : "1(")
                << m_F.features().derivative()->grad(m_cNo).text() << ")";
        for(FLst::size_type i = 0; i < m_constraints.size(); i++)
          sstream << "u" << i+1 << "*c" << i << ".grad(" << m_cNo << ")";
        sstream << std::endl << "(with";
        for(FLst::size_type i = 0; i < m_constraints.size(); i++)
          sstream << " c" << i << ".grad(" << m_cNo << ") = "
                  << m_constraints[i].get().features().derivative()->grad(m_cNo).text();
        sstream << std::endl << "(Note: cN are the constraints)";
        return sstream.str();
      }

    unsigned int dim() const
      {
        return m_sizeX + m_constraints.size() + (m_mode ? 1 : 0);
      }

    const IFunction& grad(unsigned i, unsigned member) const
      {
	assert(!member && "Invalid member requested!");
        return m_gradMembers[i-1];
      }

    template<typename T>
    T grad(const T& args, unsigned member) const
      {
	assert(!member && "Invalid member requested!");
        T ret(dim()+1);
        ret[0] = this->operator()(args);
        for(unsigned i = 1; i <= dim(); i++)
          ret[i] = grad(i, member)(args);
        return ret;
      }

  private:
    /// gradient member functions
    boost::ptr_vector<IFunction> m_gradMembers;
    /// size of input vector of _F (1 <= sizeX)
    const unsigned int m_sizeX;
    /// number of this constraint (1 <= cNo <= sizeX)
    const int m_cNo;
    /// objective function
    const IFunction& m_F;
    /// constraints
    const FLst& m_constraints;
    /// indicates if the John-Cond. are modelled
    const bool m_mode;
    // unsinn
    JohnConstraint& operator=(const IFunction&);
  };

  functions::IFunction* john_constraint(unsigned int cNo, const IFunction& F,
                                        const FLst& constraints, MODE mode)
  {
    JohnConstraint *jc = new JohnConstraint(cNo, F, constraints, mode);
    return new details::FJoin<JohnConstraint, DerivImpl<JohnConstraint> >(jc);
  }

  struct NormalizeConstraint
  {
  public:
    NormalizeConstraint(unsigned int offset, unsigned int noConstraints)
      :m_offset(offset), m_noConstraints(noConstraints),
       m_zero(constant_func(0.0, dim())), m_one(constant_func(1.0, dim()))
      {
        if(m_offset == 0)
          THROW(IllegalArguments, "Objective function must take at least one argument");
        if(m_noConstraints == 0)
          THROW(IllegalArguments, "No constraints defined.");
      }

    NormalizeConstraint(const NormalizeConstraint &other)
      :m_offset(other.m_offset), m_noConstraints(other.m_noConstraints),
       m_zero(constant_func(0.0, dim())), m_one(constant_func(1.0, dim()))
      {  }

    // Functions members
    unsigned int dim() const
      {
        return m_offset + m_noConstraints;
      }

    template<class T>
    typename T::value_type operator()(const T& args) const
      {
        typename T::value_type sum(0.0);
        for(unsigned int i = m_offset; i <= m_offset+m_noConstraints; i++) {
          sum += args[i];
        }
        return (sum - 1.0);
      }

    std::string text() const
      {
        std::stringstream sstream;
        sstream << "f(v) = ";
        for(unsigned int i = 0; i < m_noConstraints; i++)
          sstream << "u" << i << (i+1 == m_noConstraints ? " = 0 " : " + ");
        sstream << " (|v| = )" << m_offset + m_noConstraints << ")";
        return sstream.str();
      }

    const IFunction& grad(unsigned i, unsigned ) const
      {
        if(i < m_offset)
          return *m_zero;
        return *m_one;
      }

    template<typename T>
    T grad(const T& args, unsigned) const
      {
        T ret(dim()+1, typename T::value_type(1.0));
        ret[0] = this->operator()(args);
        return ret;
      }

  private:
    /// offset where the lag. multiplieres start in the input vector
    const unsigned int m_offset;
    /// number of constraints in the input vector
    const unsigned int m_noConstraints;
    ///
    std::unique_ptr<IFunction> m_zero;

    std::unique_ptr<IFunction> m_one;
  };

  IFunction* norm_const(unsigned offset, unsigned constraints)
  {
    return new details::FJoin<NormalizeConstraint, DerivImpl<NormalizeConstraint> >(new NormalizeConstraint(offset, constraints));
  }


  struct JohnInEqConstraint
  {
    JohnInEqConstraint(unsigned int cNo, unsigned int dim,
		       const IFunction& C, MODE mode)
      :m_sizeX(C.dim()), m_cNo(cNo), m_dim(dim), m_C(C), m_mode(mode == JOHN),
       m_zero(constant_func(0.0, dim))
      { 
	create_grad();
      }

    JohnInEqConstraint(const JohnInEqConstraint &other)
      :m_sizeX(other.m_C.dim()), m_cNo(other.m_cNo), m_dim(other.m_dim), m_C(other.m_C), 
       m_mode(other.m_mode), m_zero(constant_func(0.0, m_dim))
      {
	create_grad();
      }

    void create_grad()
      {
	for(unsigned int i = 1; i <= m_sizeX; i++)
          m_grad.push_back(multi_func(m_sizeX, m_cNo - (m_mode ? 0 : 1), m_C.features().derivative()->grad(i), m_dim));
      }

    template<class T>
    typename T::value_type operator()(const T &args) const
      {
        T x(m_sizeX);
        for(unsigned i = 1; i <= m_sizeX; i++)
          x[i] = args[i];
        int idx = m_mode ? 1 : 0;
        return args[m_sizeX + idx + m_cNo]*m_C(x);
      }

    unsigned dim() const 
      {
	return m_dim;
      }

    const IFunction& grad(unsigned int i, unsigned member) const
      {
        int idx = m_mode ? 1 : 0;
        //TODO: exception handling
        if(i == m_cNo + idx + m_sizeX) {
          return m_C;
        }
        else if(i > m_sizeX) {
          return *m_zero;
        }
        return m_grad[i-1];
      }

    template <typename T>
    T grad(const T& args, unsigned member) const 
      {
	T ret(dim()+1, typename T::value_type(0.0));
	ret[0] = m_C(args);
	// Ableitung in x1...xdim
	ret[mtl::irange(1, m_sizeX+1)] = m_C.features().derivative()->grad(args);
	for(unsigned int i = 1; i <= m_sizeX; i++)
	  ret[i] *= args[m_sizeX + 1 +m_cNo];
	// Ableitung u0
        int idx = m_mode ? 1 : 0;
	ret[m_cNo + m_sizeX + idx] = ret[0];
	// Funktionswert
	ret[0] *= args[m_sizeX + idx + m_cNo];
	return ret;
      }

    std::string text() const
      {
        std::stringstream sstream;
        sstream << "q(v) = ";
        sstream << "u" << m_cNo << "*" << m_C.text();
        sstream << " (|v| = " << dim() << ")";
        return sstream.str();
      }

  private:
    const unsigned int m_sizeX;
    const unsigned int m_cNo;
    const unsigned int m_dim;
    const IFunction &m_C;
    const bool m_mode;
    boost::ptr_vector<IFunction> m_grad;
    std::unique_ptr<IFunction> m_zero;
  };

  IFunction* john_ineq(unsigned int cNo, unsigned int dim,
		       const IFunction& C, MODE mode)
  {
    return new details::FJoin<JohnInEqConstraint, DerivImpl<JohnInEqConstraint> >(new JohnInEqConstraint(cNo, dim, C, mode));
  }


}
