// -*-c++-*-

template<typename A>
SIVTree<A>::SIVTreeNode::SIVTreeNode(SIVTree& tree,
                                     const core::arith::ivector& x)
  :IIVTreeNode(x), m_tree(tree), m_parent(0)
{
  m_color = GRAY;
  init();
}

template<typename A>
SIVTree<A>::SIVTreeNode::SIVTreeNode(SIVTreeNode* parent,
                                     const core::arith::ivector& x)
  :IIVTreeNode(x), m_tree(parent->m_tree), m_parent(parent)
{
  m_color = GRAY;
  init();
}

template <typename A>
core::arith::ivector SIVTree<A>::SIVTreeNode::normal_() const
{
  return m_normal;
}

template <typename A>
SIVTree<A>::SIVTreeNode::~SIVTreeNode()
{
  if(m_left)
    m_left->join();
  if(m_right)
    m_right->join();
}

template<typename A>
void SIVTree<A>::SIVTreeNode::init()
{
  const core::arith::ivector &res = details::sivtree_eval<A>::eval(X(), m_tree.obj(), m_tree.m_normals);
  if(sup(res[0]) <= 0)
    m_color = BLACK;
  else if(inf(res[0]) > 0)
    m_color = WHITE;
  unsigned dim(size(res));
  if(dim > 1) {
    m_normal.change_dim(dim-1);
    m_normal = res[mtl::irange(1,dim)];
  }
}

template<typename A>
void SIVTree<A>::SIVTreeNode::split_()
{
  if(leaf() && color() == GRAY) {
    unsigned dim = utility::max_dim(X());
    boost::array<core::arith::ivector, 2> boxes(utility::split(X(), dim, mid(X()[dim])));
    m_left.reset(new SIVTreeNode(this, boxes[0]));
    m_right.reset(new SIVTreeNode(this, boxes[1]));
  }
}

template<typename A>
node_color_t SIVTree<A>::SIVTreeNode::color_() const
{
  return m_color;
}

template<typename A>
void SIVTree<A>::SIVTreeNode::join_()
{
  if(m_left)
    m_left->join();
  m_left.reset();
  if(m_right)
    m_right->join();
  m_right.reset();
}

template<typename A>
unsigned SIVTree<A>::SIVTreeNode::children_() const
{
  if(m_left)
    return 2;
  return 0;
}

template<typename A>
bool SIVTree<A>::SIVTreeNode::leaf_() const
{
  return !m_left;
}

template<typename A>
bool SIVTree<A>::SIVTreeNode::root_() const
{
  return !m_parent;
}

template <typename A>
const typename SIVTree<A>::SIVTreeNode& SIVTree<A>::SIVTreeNode::operator[](unsigned idx) const
{
  assert(!leaf() && "Child of leaf node requested!");
  switch(idx) {
  case 0:
    return *m_left;
  case 1:
    return *m_right;
  }
  assert(!"Non existing child requested!");
  return *((SIVTreeNode*)0); // Dummy
}

template <typename A>
typename SIVTree<A>::SIVTreeNode& SIVTree<A>::SIVTreeNode::operator[](unsigned idx)
{
  assert(!leaf() && "Child of leaf node requested!");
  switch(idx) {
  case 0:
    return *m_left;
  case 1:
    return *m_right;
  }
  assert(!"Non existing child requested!");
  return *((SIVTreeNode*)0); // Dummy
}

template <typename A>
unsigned SIVTree<A>::SIVTreeNode::level_() const
{
  if(root())
    return 0;
  return m_parent->level() + 1;
}



template <typename A>
const typename SIVTree<A>::SIVTreeNode* SIVTree<A>::SIVTreeNode::parent() const
{
  return m_parent;
}

template <typename A>
typename SIVTree<A>::SIVTreeNode* SIVTree<A>::SIVTreeNode::parent()
{
  return m_parent;
}

template <typename A>
const SIVTree<A>& SIVTree<A>::SIVTreeNode::tree() const
{
  return m_tree;
}

template <typename A>
SIVTree<A>& SIVTree<A>::SIVTreeNode::tree()
{
  return m_tree;
}


template <typename A>
SIVTree<A>::SIVTree(const objects::IGeoObj &obj, const core::arith::ivector &x, bool normals)
  :m_obj(obj), m_normals(normals), m_root(*this, x)
{ }

template <typename A>
SIVTree<A>::SIVTree(const objects::IGeoObj &obj, bool normals)
  :m_obj(obj), m_normals(normals), m_root(*this, utility::unit_box(3))
{

}

template <typename A>
const typename SIVTree<A>::SIVTreeNode& SIVTree<A>::root() const
{
  return m_root;
}

template <typename A>
typename SIVTree<A>::SIVTreeNode& SIVTree<A>::root()
{
  return m_root;
}

template <typename A>
SIVTree<A>* SIVTree<A>::clone() const
{
  return 0;
}

// template <typename A>
// objects::IGeoObj& SIVTree<A>::obj_()
// {
//   return m_obj;
// }

template <typename A>
const objects::IGeoObj& SIVTree<A>::obj_() const
{
  return m_obj;
}

template <typename A>
SIVTree<A>::~SIVTree() {}

template <typename A>
void SIVTree<A>::set_shadow_(bool)
{}

template <typename A>
bool SIVTree<A>::standard_() const
{
  return true;
}

template <typename A>
bool SIVTree<A>::shadow_() const
{
  return false;
}

template <typename A>
bool SIVTree<A>::normals_() const
{
  return m_normals;
}

template <typename A>
void SIVTree<A>::set_normals_(bool en)
{
  m_normals = en;
}

namespace details
{
  template<typename A>
  core::arith::ivector sivtree_eval<A>::eval(const core::arith::ivector &X,
                                             const objects::IGeoObj &obj, bool normal)
  {
    if(normal) {
      const functions::IVFunction *nf = obj.normals();
      if(nf) {
	core::arith::ivector en(enclosure_cast<core::arith::ivector>((*nf)(lossless_cast<typename core::arith::ArithTraits<A>::vec_t>(X))));
	if(size(en))
	  return en;
      }
    }
    A acf(obj.cf()(lossless_cast<typename core::arith::ArithTraits<A>::vec_t>(X)));
    return core::arith::ivector(1,enclosure_cast<core::arith::interval>(acf));
  }
  //template<>
}

