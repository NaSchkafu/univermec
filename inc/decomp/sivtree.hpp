#ifndef __SIVTREE_HPP__
#define __SIVTREE_HPP__

// C11
#include <boost/scoped_ptr.hpp>

#include "functions/ifunction.hpp"
#include "functions/ivfunction.hpp"
#include "objects/igeoobj.hpp"
#include "decomp/iivtree.hpp"
#include "utility/utility.hpp"

namespace decomp
{

  namespace details
  {
    template<typename A>
    struct sivtree_eval
    {
      static core::arith::ivector eval(const core::arith::ivector &X,
				       const objects::IGeoObj &obj,
				       bool normal);
    };

    template<>
    struct sivtree_eval<core::arith::taylormodel> 
    {
      static core::arith::ivector eval(const core::arith::ivector &X,
				       const objects::IGeoObj &obj,
				       bool normal);
    };
      

    // template<typename A>
    // struct sivtree_tm_eval
    // {
    //   static core::arith::interval eval(const core::arith::interval &X,
    //                                  const functions::IFunction &f)
    //  {
    //    A acf(f.cf()(core::arith::ArithTraits
    //                 <typename core::arith::ArithTraits<A>::vec_t>::convert(X())));
    //    return core::arith::ArithTraits<core::arith::interval>::force_convert(acf);
    //  }
    // };
  }

  /// Standard decomposition template for all supported arithmetics
  /*!
  ** Uniform spatial space subdivision technique based on a binary tree.
  ** Nodes are interval vectors
  ** Template parameter determines the arithmetic used for inclusion
  **
  */
  template<typename A>
  class SIVTree : public IIVTree
  {
  public:
    class SIVTreeNode : public IIVTreeNode
    {
    public:
      virtual const SIVTreeNode& operator[](unsigned idx) const;
      virtual SIVTreeNode& operator[](unsigned idx);
      virtual const SIVTreeNode* parent() const;
      virtual SIVTreeNode* parent();
      virtual const SIVTree& tree() const;
      virtual SIVTree& tree();
      virtual ~SIVTreeNode();
      SIVTreeNode(SIVTree &tree, const core::arith::ivector &x);

    private:
      virtual node_color_t color_() const;
      virtual void join_();
      virtual void split_();
      virtual unsigned children_() const;
      virtual bool leaf_() const;
      virtual unsigned level_() const;
      virtual bool root_() const;
      virtual core::arith::ivector normal_() const;
      void init();
      SIVTreeNode(SIVTreeNode *parent, const core::arith::ivector &x);

    private:
      SIVTree& m_tree;
      boost::scoped_ptr<SIVTreeNode> m_left, m_right;
      SIVTreeNode* m_parent;
      decomp::node_color_t m_color;
      core::arith::ivector m_normal;
    };

    SIVTree(const objects::IGeoObj &F,
            const core::arith::ivector &X,
	    bool normals);
    SIVTree(const objects::IGeoObj &obj, bool normals);
    
    virtual ~SIVTree();

    virtual const SIVTreeNode& root() const;
    virtual SIVTreeNode& root();
    virtual SIVTree* clone() const;

  private:
    void init();
    virtual bool shadow_() const;
    virtual void set_shadow_(bool);
    virtual bool standard_() const;
    //virtual objects::IGeoObj& obj_();
    virtual const objects::IGeoObj& obj_() const;
    virtual bool normals_() const;
    virtual void set_normals_(bool en);

  private:
    const objects::IGeoObj &m_obj;
    bool m_normals;
    SIVTreeNode m_root;

  private:
    // vorerst kein copy ctor oder assigment
    // TODO: impl
    SIVTree(const SIVTree<A>&);
    SIVTree& operator=(const SIVTree<A>&);
  };



#include "sivtree.inl"

  /// standard binary tree with iv eval
  typedef SIVTree<core::arith::interval> BinIVTree;
  /// standard binary tree with affine eval
  typedef SIVTree<core::arith::aaf> BinAAFTree;
  /// standard binary tree with tm eval
  typedef SIVTree<core::arith::taylormodel> BinTMTree;

}

#endif

