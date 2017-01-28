#include <decomp/polytree.hpp>
#include "functions/ifunction.hpp"
#include "objects/igeoobj.hpp"
#include "utility/utility.hpp"


using namespace core::arith;

namespace decomp
{
    PolyIVTree::PolyIVTreeNode *PolyIVTree::PolyIVTreeNode::active_node = NULL;

    // PolyIVTree::PolyIVTreeNode::PolyIVTree::PolyIVTreeNode(const objects::IGeoObj &F)
    //     : AIVTree(utility::unit_box(3), GRAY, 0), m_F(F), m_skip_split(0), m_octa_split(0), m_cache(0), m_vertex_list(0), m_ray_vector()
    // {
    //     init();
    // }

    // PolyIVTree::PolyIVTreeNode::PolyIVTree::PolyIVTreeNode(const objects::IGeoObj &F, const ivector &X)
    //     : AIVTree(X, GRAY, 0), m_F(F), m_skip_split(0), m_octa_split(0), m_cache(0), m_vertex_list(0), m_ray_vector()
    // {
    //     init();
    // }

    // PolyIVTree::PolyIVTreeNode::PolyIVTree::PolyIVTreeNode(const objects::IGeoObj &F, const core::arith::ivector &X, unsigned skip_split, unsigned octa_split)
    //     : AIVTree(X, GRAY, 0), m_F(F), m_skip_split(skip_split), m_octa_split(octa_split), m_cache(0), m_vertex_list(0), m_ray_vector()
    // {
    //     init();
    // }

    PolyIVTree::PolyIVTreeNode::PolyIVTreeNode(PolyIVTreeNode *parent, const ivector &X)
      : IIVTreeNode(X), m_color(GRAY), m_parent(parent), m_tree(parent->m_tree), m_cache(0), m_vertex_list(0), 
	m_ray_vector(parent->m_ray_vector), m_left(0), m_right(0)
    {
        if( parent->m_cache )
            this->m_cache = new std::vector<PolyIVTreeNode::face_info_t>(*parent->m_cache);
        if( parent->m_vertex_list )
            this->m_vertex_list = new std::vector<const rvector*>(*parent->m_vertex_list);
        init();
    }

    PolyIVTree::PolyIVTreeNode::PolyIVTreeNode(PolyIVTree &tree, const ivector &X)
      :IIVTreeNode(X), m_color(GRAY), m_parent(0), m_tree(tree),m_cache(0), m_vertex_list(0), 
       m_ray_vector(), m_left(0), m_right(0)
    {
      init();
    }

    PolyIVTree::PolyIVTreeNode::~PolyIVTreeNode()
    {
      if(m_left)
	m_left->join();
      if(m_right)
	m_right->join();
      delete m_cache;
    }

    const rvector* PolyIVTree::PolyIVTreeNode::get_ray_vector() const
    {
        return m_ray_vector.get();
    }

  void PolyIVTree::PolyIVTreeNode::set_ray_vector(const rvector *vect)
  {
        if( vect )
            m_ray_vector = boost::shared_ptr<rvector>(new rvector(*vect));
        else
            m_ray_vector = boost::shared_ptr<rvector>();
    }

  std::vector<PolyIVTree::PolyIVTreeNode::face_info_t>* PolyIVTree::PolyIVTreeNode::get_cache()
    {
        return m_cache;
    }

  std::vector<PolyIVTree::PolyIVTreeNode::face_info_t>* PolyIVTree::PolyIVTreeNode::reset_cache(size_t size)
    {
        delete m_cache;
        m_cache = new std::vector<PolyIVTree::PolyIVTreeNode::face_info_t>(size, NONE);
        return get_cache();
    }

    std::vector<const rvector*> *PolyIVTree::PolyIVTreeNode::init_vertex_list(const std::vector<rvector> &vertex_list)
    {
        m_vertex_list = new std::vector<const rvector*>();
        for(std::vector<rvector>::const_iterator it = vertex_list.begin(); it != vertex_list.end(); ++it)
        {
            m_vertex_list->push_back(&(*it));
        }
        return m_vertex_list;
    }

    std::vector<const rvector*> *PolyIVTree::PolyIVTreeNode::get_vertex_list()
    {
        return m_vertex_list;
    }


    void PolyIVTree::PolyIVTreeNode::split_()
    {
        if(leaf() && color() == GRAY)
        {
            unsigned dim = utility::max_dim(X());
            boost::array<ivector, 2> boxes(utility::split(X(), dim, mid(X()[dim])));
            m_left.reset(new PolyIVTree::PolyIVTreeNode(this, boxes[0]));
	    m_right.reset(new PolyIVTree::PolyIVTreeNode(this, boxes[1]));
        }
    }

    void PolyIVTree::PolyIVTreeNode::init()
    {
        unsigned l = this->level();
        // Schnelles überspringen der ersten m_skip_split halbierungen.
        // Zwei von drei halbierungen werden übersprungen bis level >= m_octa_split (Octtree-artiges Verhalten)
	unsigned skip_split, octa_split;
	m_tree.get_split_params(&skip_split, &octa_split);
        if( (l <= skip_split) || (l < octa_split && (l % 3 != 1)) )
        {
            //std::cout << "Skipping split at level " << l << std::endl;
            m_color = GRAY;
            return;
        }
        else
        {
            PolyIVTreeNode::active_node = this;
            interval cf( m_tree.obj().cf()( X() ) );
            if(sup(cf) <= 0)
              m_color = BLACK;
            else if(inf(cf) > 0)
              m_color = WHITE;
            PolyIVTree::PolyIVTreeNode::active_node = 0;
        }
    }

  const IIVTreeNode& PolyIVTree::PolyIVTreeNode::operator[](unsigned idx) const
  {
    assert(!leaf() && "Child of a leaf node requested!");
    switch(idx){
    case 0:
      return *m_left;
    case 1:
      return *m_right;
    }
    assert(!"Invalid child!");
  }

  IIVTreeNode& PolyIVTree::PolyIVTreeNode::operator[](unsigned idx)
  {
    assert(!leaf() && "Child of a leaf node requested!");
    switch(idx){
    case 0:
      return *m_left;
    case 1:
      return *m_right;
    }
    assert(!"Invalid child!");
  }

  const IIVTreeNode* PolyIVTree::PolyIVTreeNode::parent() const
  {
    return m_parent;
  }

  IIVTreeNode* PolyIVTree::PolyIVTreeNode::parent()
  {
    return m_parent;
  }

  PolyIVTree& PolyIVTree::PolyIVTreeNode::tree()
  {
    return m_tree;
  }

  const PolyIVTree& PolyIVTree::PolyIVTreeNode::tree() const
  {
    return m_tree;
  }

  unsigned PolyIVTree::PolyIVTreeNode::level_() const
  {
    if(root())
      return 0;
    return 1 + m_parent->level();
  }

  node_color_t PolyIVTree::PolyIVTreeNode::color_() const
  {
    return m_color;
  }

  void PolyIVTree::PolyIVTreeNode::join_()
  {
    if(m_left)
      m_left->join();
    if(m_right)
      m_right->join();
    m_left.reset();
    m_right.reset();
  }

  bool PolyIVTree::PolyIVTreeNode::leaf_() const
  {
    return !m_left;
  }
  
  bool PolyIVTree::PolyIVTreeNode::root_() const
  {
    return !m_parent;
  }

  unsigned PolyIVTree::PolyIVTreeNode::children_() const
  {
    if(m_left)
      return 2;
    return 0;
  }

  

  // ****************************************************************
  
  PolyIVTree::~PolyIVTree()
  {
    
  }

  bool PolyIVTree::standard_() const
  {
    return true;
  }

  bool PolyIVTree::shadow_() const
  {
    return false;
  }

  void PolyIVTree::set_shadow_(bool) 
  {
  }

  PolyIVTree* PolyIVTree::clone() const
  {
    //TODO: FIX!
    return 0;
  }

  PolyIVTree::PolyIVTreeNode& PolyIVTree::root() 
  {
    return m_root;
  }

  const PolyIVTree::PolyIVTreeNode& PolyIVTree::root() const
  {
    return m_root;
  }

    void PolyIVTree::set_split_params(unsigned skip_split, unsigned octa_split)
    {
        this->m_skip_split = skip_split;
        this->m_octa_split = octa_split;
    }

    void PolyIVTree::get_split_params(unsigned *skip_split, unsigned *octa_split)
    {
        if( skip_split )
            *skip_split = m_skip_split;
        if( octa_split )
            *octa_split = m_octa_split;
    }


  const objects::IGeoObj& PolyIVTree::obj_() const
  {
    return m_obj;
  }
  
  PolyIVTree::PolyIVTree(const objects::IGeoObj &F,
			 const core::arith::ivector &X, 
			 unsigned skip_split, unsigned octa_split)
    :m_obj(F), m_skip_split(skip_split), m_octa_split(octa_split), m_root(*this, X)
  {    
    
  }


}
