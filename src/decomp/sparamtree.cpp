#include <decomp/sparamtree.hpp>

#include "functions/ifunction.hpp"
#include "objects/iparamsurface.hpp"
#include "utility/utility.hpp"

namespace decomp
{
  // ****************************************************************
  // Tree
  // ****************************************************************
  SParamTree::SParamTree(const objects::IParamSurface &surf, bool normals)
    :m_surf(surf), m_normals(normals), m_root(*this, surf.domain())
  {}

  SParamTree::SParamTree(const objects::IParamSurface &surf, const core::arith::ivector &bounds, bool normals)
    :m_surf(surf), m_normals(normals), m_root(*this, bounds)
  {
    std::cout << "bounds " << bounds << std::endl;
  }

  SParamTree::~SParamTree()
  {}

  const objects::IParamSurface& SParamTree::p_surf_() const
  {
    return m_surf;
  }

  void SParamTree::set_shadow_(bool ) {}
  bool SParamTree::shadow_() const { return false; }
  bool SParamTree::standard_() const { return true; }

  const objects::IGeoObj& SParamTree::obj_() const
  {
    return m_surf;
  }

  SParamTree* SParamTree::clone() const
  {
    return 0;
  }
  
  SParamTree::SParamTreeNode& SParamTree::root() 
  {
    return m_root;
  }

  const SParamTree::SParamTreeNode& SParamTree::root() const
  {
    return m_root;
  }
  
  bool SParamTree::normals_() const
  {
    return m_normals;
  }
    
  void SParamTree::set_normals_(bool en)
  {
    m_normals = en;
  }
  


  // ****************************************************************
  // Node
  // ****************************************************************
  SParamTree::SParamTreeNode::SParamTreeNode(SParamTree::SParamTreeNode *parent, const core::arith::ivector &pd)
    :IParametricTree::IParametricTreeNode(parent->X()), m_tree(parent->m_tree), 
     m_parent(parent), m_color(NOT_WHITE), m_pd(pd)
  {
    init();
  }

  SParamTree::SParamTreeNode::SParamTreeNode(SParamTree& tree, const core::arith::ivector &pd)
    :IParametricTree::IParametricTreeNode(core::arith::ivector(tree.p_surf().p_fun().cdim())), 
     m_tree(tree), m_parent(0), m_color(NOT_WHITE), m_pd(pd)
  {
    init();
  }

  core::arith::ivector SParamTree::SParamTreeNode::normal_() const
  {
    return m_normal;
  }

  const SParamTree& SParamTree::SParamTreeNode::tree() const
  {
    return m_tree;
  }

  SParamTree& SParamTree::SParamTreeNode::tree()
  {
    return m_tree;
  }

  void SParamTree::SParamTreeNode::init()
  {
    if(m_tree.normals()) {
      const functions::IVFunction* nf(m_tree.p_surf().p_normals());
      if(nf) {
	unsigned dim(size(m_pd)+1);
	core::arith::ivector n(nf->operator()(m_pd));
	m_x = n[mtl::irange(0, dim)];
	if(size(n) == 2*dim) {
	  m_normal.change_dim(dim);
	  m_normal = n[mtl::irange(dim, 2*dim)];
	}
	return;
      }
    }
    m_x = m_tree.p_surf().p_fun()(m_pd);
  }

  void SParamTree::SParamTreeNode::split_()
  {
    if(leaf()) {
      unsigned dim = utility::max_dim(m_pd);
      boost::array<core::arith::ivector, 2> boxes(utility::split(m_pd, dim, mid(m_pd[dim])));
      m_left.reset(new SParamTreeNode(this, boxes[0]));
      m_right.reset(new SParamTreeNode(this, boxes[1]));
    }
  }

  void SParamTree::SParamTreeNode::join_()
  {
    m_left.reset();
    m_right.reset();
  }

  SParamTree::SParamTreeNode::~SParamTreeNode()
  {
    if(m_left)
      m_left->join();
    if(m_right)
      m_right->join();
  }

  unsigned SParamTree::SParamTreeNode::level_() const
  {
    if(root())
      return 0;
    return m_parent->level() + 1;
  }

  const SParamTree::SParamTreeNode* SParamTree::SParamTreeNode::parent() const
  {
    return m_parent;
  }

  SParamTree::SParamTreeNode* SParamTree::SParamTreeNode::parent()
  {
    return m_parent;
  }

  bool SParamTree::SParamTreeNode::root_() const
  {
    return !m_parent;
  }

  const SParamTree::SParamTreeNode& SParamTree::SParamTreeNode::operator[](unsigned idx) const
  {
    assert(idx < 2 && "Illegal index requested!");
    if(!idx)
      return *m_left;
    return *m_right;
  }
  SParamTree::SParamTreeNode& SParamTree::SParamTreeNode::operator[](unsigned idx)
  {
    assert(idx < 2 && "Illegal index requested!");
    if(!idx)
      return *m_left;
    return *m_right;
  }

  node_color_t SParamTree::SParamTreeNode::color_() const 
  {
    return m_color;
  }

  bool SParamTree::SParamTreeNode::leaf_() const
  {
    return !m_left;
  }

  unsigned SParamTree::SParamTreeNode::children_() const
  {
    if(m_left)
      return 2;
    return 0;
  }

  core::arith::ivector SParamTree::SParamTreeNode::p_x_() const
  {
    return m_pd;
  }   

}

