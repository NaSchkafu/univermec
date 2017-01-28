#ifndef __STANDARDCSGOPS_HPP__
#define __STANDARDCSGOPS_HPP__

#include "../icsgnode.hpp"

namespace objects
{
  namespace details
  {
    // template<typename TB>
    // struct CSGStandardTransform
    // {
    // private:
    //   typedef typename core::arith::ArithTraits<TB>::mat_t MAT_T;
    //   typedef typename core::arith::ArithTraits<TB>::vec_t VEC_T;

    // public:
    //   CSGStandardTransform(const ICSGNode &n, const  MAT_T &m, const MAT_T &im,
    //                        const VEC_T &t, const VEC_T &it);

    //   unsigned dim() const;
      
    //   std::string text() const;
      
    //   template<typename T>
    //   typename T::value_type operator()(const T& args) const;

    //   MAT_T rot() const;
      
    //   MAT_T i_rot() const;
      
    //   VEC_T trans() const;
      
    //   VEC_T i_trans() const;

    // private:
    //   const ICSGNode &m_node;
    //   /// rotation matrix
    //   const MAT_T m_m;
    //   /// inverse rotation
    //   const MAT_T m_im;
    //   /// translation
    //   const VEC_T m_t;
    //   /// inverse translation
    //   const VEC_T m_it;
    // };

    // template<>
    // struct CSGStandardTransform<core::arith::mreal>
    // {
    //   CSGStandardTransform(const ICSGNode &n, const core::arith::rmatrix &m,
    //                        const core::arith::rmatrix &im, const core::arith::rvector &t,
    //                        const core::arith::rvector &it);
      
    //   unsigned dim() const;
      
    //   std::string text() const;
      
    //   template<typename T>
    //   typename T::value_type operator()(const T& args) const;

    //   core::arith::rmatrix rot() const;
      
    //   core::arith::rmatrix i_rot() const;
      
    //   core::arith::rvector trans() const;
      
    //   core::arith::rvector i_trans() const;

    // private:
    //   const ICSGNode &m_node;
    //   /// rotation matrix
    //   const core::arith::rmatrix m_m;
    //   /// rotation transformation
    //   const core::arith::rmatrix m_im;
    //   /// translation
    //   const core::arith::rvector m_t;
    //   /// inverse translation
    //   const core::arith::rvector m_it;
    // };

    
    struct CSGStandardInversion
    {
      CSGStandardInversion(const ICSGNode &node);
      
      unsigned dim() const;
      
      std::string text() const;
      
      template<typename T>
      typename T::value_type operator()(const T& args) const;
      
    private:
      const ICSGNode &m_node;
    };

    struct CSGStandardUnion
    {
      CSGStandardUnion(const ICSGNode &node1, const ICSGNode &node2);
      
      unsigned dim() const;
      
      std::string text() const;

      core::arith::mreal operator()(const core::arith::rvector &) const;      
      
      template<typename T>
      typename T::value_type operator()(const T& args) const;

    private:
      const ICSGNode &m_node1;
      const ICSGNode &m_node2;
    };

    struct CSGStandardIntersection
    {
      CSGStandardIntersection(const ICSGNode &node1, const ICSGNode &node2);
      
      unsigned dim() const;
      
      std::string text() const;
      
      template<typename T>
      typename T::value_type operator()(const T& args) const;

      core::arith::mreal operator()(const core::arith::rvector &) const;
      
      
    private:
      const ICSGNode &m_node1;
      const ICSGNode &m_node2;
    };



    #include "standardcsgops.inl"
  }
}


#endif /*__STANDARDCSGOPS_HPP__*/
