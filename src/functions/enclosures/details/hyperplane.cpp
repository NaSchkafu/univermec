/**
 * @file   constraint.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:49:02 2009
 *
 * @brief  Implementation of halfspace constraints
 *
 *
 */

#include "functions/enclosures/details/hyperplanefwd.hpp"
#include "functions/enclosures/details/hyperplane.hpp"

#include "functions/ifunction.hpp"
#include "functions/details/fvbase.hpp"
#include "functions/details/fderiv.hpp"
#include "functions/details/fcustomconst.hpp"

using namespace core::arith;


namespace functions
{
  namespace encl
  {
    namespace details
    {
      Hyperplane_F::Hyperplane_F(const core::arith::ivector &normal,
                                 const core::arith::interval &offset)
        :m_normal(normal), m_offset(offset)
      {

      }

      unsigned Hyperplane_F::dim() const
      {
        return size(m_normal);
      }

      std::string Hyperplane_F::text() const
      {
        std::stringstream sstr;
        sstr << m_normal << "*X" << " - " << m_offset;
        return sstr.str();
      }

      bool Hyperplane_F::prune(core::Box &box, const core::arith::interval &rhs) const
      {
        unsigned it = 0;
        core::arith::ivector &X = box.ref_X();
        // core::arith::ivector X(2, core::arith::interval(0, 1.25));
        //std::cout << "RHS " << rhs << std::endl;
        unsigned dim = size(m_normal);
        for(unsigned i = 0; i < dim; i++) {
          if(0 <= m_normal[i])
            continue;

          interval accu(m_offset);
          // std::cout << it << " Accu " << accu << std::endl;
          for(unsigned j = 0; j < dim; j++)
            if(i != j) {
              accu -= m_normal[j]*X[j];
              // std::cout << it << " m_normal " << m_normal[j] << std::endl;
              // std::cout << it << " X[j] " << X[j] << std::endl;
              // std::cout << it << " m_normal*X[j] " << m_normal[j]*X[j] << std::endl;
            }
          // std::cout << it << " Accu " << accu << std::endl;
          accu += rhs;
          // std::cout << it << " Accu " << accu << std::endl;
          // std::cout << it << " m_normal[i] " << m_normal[i] << std::endl;
          accu /= m_normal[i];
          // std::cout << it << " Accu " << accu << std::endl;
          X[i] &= accu;
          // std::cout << it++ << " X[i] " << X[i] << std::endl;
          if(is_empty(X[i]))
            return true;
        }
        return false;
      }

      core::arith::ivector Hyperplane_F::normal() const
      {
        return m_normal;
      }

      core::arith::interval Hyperplane_F::offset() const
      {
        return m_offset;
      }


      IFunction* hyperplane(const core::arith::ivector &normal, const core::arith::interval &offset)
      {
        return new functions::details::FJoin<Hyperplane_F,
                                             functions::details::FDeriv<Hyperplane_F>, functions::details::FCustomConst<Hyperplane_F> >
          (Hyperplane_F(normal, offset));
      }
    }
  }
}


