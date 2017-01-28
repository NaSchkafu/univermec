#ifndef __HYPERPLANEFWD_HPP__
#define __HYPERPLANEFWD_HPP__

#include "core/box.hpp"

namespace functions
{
  namespace encl
  {
    namespace details
    {
      // Hyperplane FWD
      struct Hyperplane_F
      {
        Hyperplane_F(const core::arith::ivector &normal,
                     const core::arith::interval &offset);

        template<typename T>
        typename T::value_type operator()(const T& args) const
          {
            T normal(core::arith::ArithTraits<T>::convert(m_normal));
            typename T::value_type offset(core::arith::ArithTraits<typename T::value_type>::convert(m_offset));
            return mtl::dot(normal,args) - offset;
          }

        unsigned dim() const;
        std::string text() const;

        bool prune(core::Box &box, const core::arith::interval &rhs) const;
        core::arith::ivector normal() const;
        core::arith::interval offset() const;

      private:
        core::arith::ivector m_normal;
        core::arith::interval m_offset;
      };
    }
  }
}

#endif /*__HYPERPLANEFWD_HPP__*/
