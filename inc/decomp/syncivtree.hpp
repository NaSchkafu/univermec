#ifndef __SYNCIVTREE_HPP__
#define __SYNCIVTREE_HPP__

#ifdef _OPENMP
#include "stable.h"
#include <omp.h>

//#include <boost/thread/locks.hpp>
#include "iivtree.hpp"

namespace decomp
{
  /// Provides a special synchronized tree node class for concurrent access
  /**
   * Concurrent access from multiple threads is allowed to the node instances.
   * This is achieved through a mutex. However, no syncronization is done for methods on
   * the tree.
   * 
   */
  class SyncIVTree : public IIVTree
  {
  public:
    /// This class wraps a IIVTreeNode for synchronizing access from different threads
    class SyncIVTreeNode : public IIVTreeNode
    {
    public:
      /// Read-only, exclusive lock
      virtual const SyncIVTreeNode& operator[](unsigned idx) const;
      /// Read-only, exclusive lock
      virtual SyncIVTreeNode& operator[](unsigned idx);
      /// Read-only, no locking
      virtual const SyncIVTreeNode* parent() const;
      /// Read-only, no locking
      virtual SyncIVTreeNode* parent();
      /// Read-only, no locking
      virtual const SyncIVTree& tree() const;
      /// Read-only, no locking
      virtual SyncIVTree& tree();
      virtual ~SyncIVTreeNode();
      SyncIVTreeNode(IIVTreeNode &n, SyncIVTree &tree);

    private:
      SyncIVTreeNode(IIVTreeNode &n, SyncIVTreeNode *parent);
      /// Read-only, no locking
      virtual node_color_t color_() const;
      /// Write, exclusive lock
      /**
       * join_() also automatically aquires exlcusive locks to the children in order
       * to delete them
       */
      virtual void join_();
      /// Write, exclusive lock
      virtual void split_();
      /// Read-only, exclusive lock
      virtual unsigned children_() const;
      /// Read-only, exclusive lock
      virtual bool leaf_() const;
      /// Read-only, no lock
      virtual unsigned level_() const;
      /// Read-only, no lock
      virtual bool root_() const;
      virtual core::arith::ivector normal_() const;
      void init();

    private:
      SyncIVTree& m_tree;
      boost::ptr_vector<SyncIVTreeNode> m_children;
      SyncIVTreeNode* m_parent;
      IIVTreeNode& m_node;
      mutable omp_lock_t m_lock;
      bool m_dlock;
    };

    /** 
     * Ctor
     * Constructs an adapter for an IIVTree object with synchronized methods
     * 
     * @param tree tree to adapt
     * 
     */
    explicit SyncIVTree(IIVTree &tree);

    /** 
     * Dtor
     * 
     */
    virtual ~SyncIVTree();

    virtual const SyncIVTreeNode& root() const;
    virtual SyncIVTreeNode& root();
    virtual SyncIVTree* clone() const;

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
    SyncIVTreeNode m_root;
    IIVTree &m_tree;

  private:
    // vorerst kein copy ctor oder assigment
    // TODO: impl
    SyncIVTree(const SyncIVTree&);
    SyncIVTree& operator=(const SyncIVTree&);
  };


}

#endif

#endif /*__SYNCIIVTREE_HPP__*/
