#include <decomp/syncivtree.hpp>

#include <boost/foreach.hpp>

#ifdef _OPENMP

namespace decomp 
{
  namespace details 
  {
    struct Locker 
    {
      explicit Locker(omp_lock_t *lock, bool dlock)
	:m_lock(lock), m_dlock(dlock)
	{
	  if(m_dlock)
	    omp_set_lock(m_lock);
	}

      ~Locker()
	{
	  if(m_dlock)
	    omp_unset_lock(m_lock);
//	  std::cout << "~Locker" << std::endl;
	}

    private:
      omp_lock_t *m_lock;
      bool m_dlock;
    };
  }

  SyncIVTree::SyncIVTreeNode::SyncIVTreeNode(IIVTreeNode &n, SyncIVTreeNode *parent)
    :IIVTreeNode(n.X()), m_tree(parent->tree()), m_parent(parent), m_node(n), m_dlock(true)
  {
    //std::cout << n.X() << std::endl;
    omp_init_lock(&m_lock);
  }

  SyncIVTree::SyncIVTreeNode::SyncIVTreeNode(IIVTreeNode &n, SyncIVTree &tree)
    :IIVTreeNode(n.X()), m_tree(tree), m_parent(0), m_node(n), m_dlock(true)
  { 
    //std::cout << n.X() << std::endl;
    omp_init_lock(&m_lock);
  }

  const SyncIVTree::SyncIVTreeNode& SyncIVTree::SyncIVTreeNode::operator[](unsigned idx) const
  {
    details::Locker l(&m_lock, m_dlock);
    return m_children[idx];
  }
  
  SyncIVTree::SyncIVTreeNode& SyncIVTree::SyncIVTreeNode::operator[](unsigned idx)
  {
    details::Locker l(&m_lock, m_dlock);
    return m_children[idx];
  }

  const SyncIVTree::SyncIVTreeNode* SyncIVTree::SyncIVTreeNode::parent() const
  {
    return m_parent;
  }

  SyncIVTree::SyncIVTreeNode* SyncIVTree::SyncIVTreeNode::parent()
  {
    return m_parent;
  }

  const SyncIVTree& SyncIVTree::SyncIVTreeNode::tree() const
  {
    return m_tree;
  }

  SyncIVTree& SyncIVTree::SyncIVTreeNode::tree()
  {
    return m_tree;
  }

  SyncIVTree::SyncIVTreeNode::~SyncIVTreeNode()
  {
    join_();
    omp_destroy_lock(&m_lock);
  }

  node_color_t SyncIVTree::SyncIVTreeNode::color_() const
  {
    return m_node.color();
  }
  
  void SyncIVTree::SyncIVTreeNode::join_()
  {
    details::Locker l(&m_lock, true);
    BOOST_FOREACH(SyncIVTreeNode &n, m_children)
      n.join();
    m_children.clear();
  }

  void SyncIVTree::SyncIVTreeNode::split_()
  {
    details::Locker l(&m_lock, true);
    if(m_children.empty()) {
      m_node.split();
      for(unsigned i = 0; i < m_node.children(); i++)
	m_children.push_back(new SyncIVTreeNode(m_node[i], this));
    }
    m_dlock = false;
  }

  unsigned SyncIVTree::SyncIVTreeNode::children_() const
  {
    details::Locker l(&m_lock, m_dlock);
    return m_children.size();
  }

  bool SyncIVTree::SyncIVTreeNode::leaf_() const
  {
    details::Locker l(&m_lock, m_dlock);
    return !m_children.empty();
  }

  unsigned SyncIVTree::SyncIVTreeNode::level_() const
  {
    return m_node.level();
  }

  bool SyncIVTree::SyncIVTreeNode::root_() const
  {
    return !m_parent;
  }

  core::arith::ivector SyncIVTree::SyncIVTreeNode::normal_() const
  {
    return m_node.normal();
  }

  // ****************************************************************
  // SyncIVTree
  // ****************************************************************
  bool SyncIVTree::shadow_() const
  {
    return m_tree.shadow();
  }

  void SyncIVTree::set_shadow_(bool s)
  {
    m_tree.set_shadow(s);
  }

  bool SyncIVTree::standard_() const
  {
    return m_tree.standard();
  }

  SyncIVTree::SyncIVTree(IIVTree &tree)
    :m_tree(tree), m_root(tree.root(), *this)
  { }

  SyncIVTree::~SyncIVTree()
  {
    m_root.join();
  }

  const SyncIVTree::SyncIVTreeNode& SyncIVTree::root() const
  {
    return m_root;
  }

  SyncIVTree::SyncIVTreeNode& SyncIVTree::root()
  {
    return m_root;
  }

  SyncIVTree* SyncIVTree::clone() const
  {
    return new SyncIVTree(m_tree);
  }

  const objects::IGeoObj& SyncIVTree::obj_() const
  {
    return m_tree.obj();
  }

  bool SyncIVTree::normals_() const
  {
    return m_tree.normals();
  }
  
  void SyncIVTree::set_normals_(bool en)
  {
    m_tree.set_normals(en);
  }
}

#endif
