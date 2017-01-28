#ifndef __CSGNODEIMPL_HPP__
#define __CSGNODEIMPL_HPP__

#include <boost/ptr_container/ptr_vector.hpp>

#include "functions/ivfunction.hpp"
#include "functions/details/flinchainrule.hpp"
#include "../icsgnode.hpp"
#include "../details/implnormals.hpp"

namespace functions
{
  class IFunction;
}

namespace objects
{
  namespace details
  {
    /// implementation of ICSGNode Interface
    class CSGNodeImpl : public virtual ICSGNode
    {
    public:
      virtual ~CSGNodeImpl();
      
      // Interface fuer Factory
    protected:
      /*!
      ** Ctor
      **
      ** @param min_ch minimum children of this node
      ** @param max_ch maximum children of this node
      **
      */
      CSGNodeImpl(unsigned min_ch, unsigned max_ch, 
		  CSG_NODE_TYPE type = CSG_UNKNOWN, ICSGNode *parent = 0);

    private:
      /*!
      ** Sets the characteristic function of this node
      **
      ** @param cf new characteristic function
      */
      //void set_cf(functions::IFunction *cf);

      void set_parent(ICSGNode *parent);

      // Factory Klassen sind friends
      friend class StandardCSGNodeFactory;

      // Implementierung ICSGNode
    private:
      virtual unsigned children_() const;
      virtual bool leaf_() const;
      virtual const ICSGNode* parent_() const;
      virtual ICSGNode* parent_();
      virtual const ICSGNode& child_(unsigned i) const;
      virtual ICSGNode& child_(unsigned i) ;
      virtual bool add_(ICSGNode *child);
      virtual bool remove_(unsigned i);
      virtual unsigned max_children_() const;
      virtual unsigned min_children_() const;
      virtual iterator begin_();
      virtual iterator end_();
      virtual const_iterator begin_() const;
      virtual const_iterator end_() const;
      virtual CSG_NODE_TYPE type_() const;
      virtual ICSGNode* clone_() const;

      // Impl. IGeoObj
    private:
//      virtual const functions::IFunction& cf_() const;
//      virtual unsigned dim_() const;

    private:
      /// pointer to characteristic function
      //std::auto_ptr<functions::IFunction> m_cf;
      /// number of minimum children
      unsigned m_min_ch;
      /// number of maximum children
      unsigned m_max_ch;
      /// pointer to children
      boost::ptr_vector<ICSGNode> m_children;
      /// pointer to parent
      ICSGNode *m_parent;
      /// Node Type
      CSG_NODE_TYPE m_type;
    };

    class CSGLeafImpl : public virtual ICSGLeaf, private CSGNodeImpl
    {
    public:
      CSGLeafImpl(const objects::IGeoObj &obj, ICSGNode *parent = 0);

    private:
      // ICSGLeaf
      virtual const objects::IGeoObj& obj_() const;
      // IGeoObj
      virtual const functions::IFunction& cf_() const;
      virtual unsigned dim_() const;
      virtual const functions::IVFunction* normals_() const;

    private:
      const IGeoObj& m_obj;
    };


    template<typename T>
    class CSGTransformImpl : private virtual CSGNodeImpl, public virtual ICSGTransform
    {
    public:
      CSGTransformImpl(unsigned min_ch, unsigned max_ch, 
		       CSG_NODE_TYPE type = CSG_UNKNOWN, ICSGNode *parent = 0);
      
    private:
      // ICSGTransform
      virtual core::arith::imatrix rot_() const;
      virtual core::arith::imatrix i_rot_() const;
      virtual core::arith::ivector trans_() const;
      virtual core::arith::ivector i_trans_() const;

      // IGeoObj
      virtual const functions::IFunction& cf_() const;
      virtual unsigned dim_() const;
      virtual const functions::IVFunction* normals_() const;

      void set_t_s(const typename core::arith::ArithTraits<T>::mat_t &r, 
		   const typename core::arith::ArithTraits<T>::mat_t &ir,
		   const typename core::arith::ArithTraits<T>::vec_t &t,
		   const typename core::arith::ArithTraits<T>::vec_t &it);

    private:
      typedef functions::details::FLinChainRule<T> fun_t;
      /// transform
      std::unique_ptr<fun_t> m_cf;
      /// normals
      std::unique_ptr<functions::IVFunction> m_nf;
      // friends
      friend class StandardCSGNodeFactory;
      
      typename core::arith::ArithTraits<T>::mat_t m_r;
      typename core::arith::ArithTraits<T>::vec_t m_t;
    };

    #include "csgnodeimpl.inl"
  }

}


#endif /*__CSGNODEIMPL_HPP__*/
