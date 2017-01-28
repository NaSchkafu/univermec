#include <decomp/contractingtree.hpp>

#include "functions/ifunction.hpp"
#include "functions/ivfunction.hpp"
#include "functions/iffeatures.hpp"
#include "functions/iencl.hpp"
#include "functions/icontractor.hpp"
#include "functions/icustomconsistency.hpp"

#include "objects/igeoobj.hpp"

#include "utility/utility.hpp"

using namespace utility;
using namespace core::arith;


namespace decomp
{
  CIVTree::CIVTreeNode::CIVTreeNode(CIVTree& tree,
                                    const ivector& x)
    :IIVTreeNode(x), m_tree(tree), m_parent(0), m_meta(0), m_hcp(x)
  {
    m_color = GRAY;
    init();
  }


  CIVTree::CIVTreeNode::CIVTreeNode(CIVTreeNode* parent, const core::arith::ivector &x,
                                    const core::Box &hcp, node_color_t color)
    :IIVTreeNode(x), m_tree(parent->m_tree), m_parent(parent), m_hcp(hcp), m_color(color), m_meta(0)
  {
    init();
  }

  CIVTree::CIVTreeNode::~CIVTreeNode()
  {
    if(m_left)
      m_left->join();
    if(m_right)
      m_right->join();
    delete m_meta;
  }

  decomp::node_color_t CIVTree::CIVTreeNode::get_color(const interval &encl) const
  {
    if(inf(encl) > 0)
      return WHITE;
    else if(sup(encl) <= 0)
      return BLACK;
    return GRAY;
  }

  void CIVTree::CIVTreeNode::init()
  {
    Box hcp(m_hcp);
    interval rhs(-infinity(), 0);

    bool use_n = m_tree.normals();
    
    for(std::vector<queue_t>::const_iterator it(m_tree.m_queue.begin());
        it != m_tree.m_queue.end() && m_color == GRAY && !m_meta; ++it) {
      decomp::node_color_t col;
      const functions::IContractor* fcon = 0;
      boost::scoped_ptr<functions::IFunction> fencl;
      // Enclosure liegt vor
      if(it->m_type > 0) {
	// Fieser Hack für Normals
	if(use_n) {
	  bool has_col = false;
	  use_n = false;
	  //std::cout << "N1" << std::endl;
	  const functions::IVFunction *nf = tree().obj().normals();
	  //std::cout << "N2" << std::endl;
	  if(nf) {
	    core::arith::ivector en(nf->operator()(hcp.ref_X()));
	    //std::cout << "N3" << std::endl;
	    if(size(en)) {
	      m_normal.change_dim(3);
	      for(unsigned i = 1; i < 4; i++)
		m_normal[i-1] = en[i];
	      //std::cout << "N4" << std::endl;
	      has_col = true;
	      col = get_color(en[0]);
	      //std::cout << "N4" << std::endl;
	    }
	  }
	  if(!has_col)
	    col = get_color(tree().obj().cf()(hcp.ref_X()));
	} 
	else {  
	  fencl.reset(it->m_encl->encl(hcp.ref_X()));
	  if(!fencl)
	    continue;
	  col = get_color((*fencl)(hcp.ref_X()));
	}
	
        switch(col) {
        case WHITE:
          m_color = WHITE;
          continue;
        case BLACK:
          if(m_hcp.X() == X())
            m_color = BLACK;
          else
            m_meta = new node_meta_t(hcp.X(), WHITE_INV, hcp.X(), BLACK);
          continue;
        }

        if(it->m_type == CIVTree::queue_t::HYBRID)
          fcon = &(fencl->features().custom_consistency()->get());
      }


      if(fcon || it->m_type == 0) {
        const functions::IContractor *con = fcon ? fcon : it->m_con;
        // TODO: Fixme
	if(con->prune(hcp, rhs))
          m_color == WHITE;
        else if(m_tree.progress(X(), hcp.X()))
          m_meta = new node_meta_t(hcp.X(), WHITE_INV, hcp.X(), GRAY);
      }
    }

    // Standard
    if(!m_meta && m_color == GRAY) {
      unsigned dim = max_dim(hcp.X());
      mreal sp(mid(hcp.X()[dim]));
      boost::array<ivector,2> x(utility::split(X(), dim, sp));
      boost::array<ivector,2> x_(utility::split(hcp.X(), dim, sp));
      m_meta = new node_meta_t(x[0], x_[0], x[1], x_[1]);
    }
  }

  core::arith::ivector CIVTree::CIVTreeNode::normal_() const
  {
    return m_normal;
  }

  void CIVTree::CIVTreeNode::split_()
  {
    if(leaf() && color() == GRAY) {
      unsigned dim = utility::max_dim(X());
      boost::array<ivector, 2> boxes(utility::split(X(), dim, mid(X()[dim])));
      // Der Knoten wurde gejoint, Metadaten wiederherstellen!
      if(!m_meta)
	init();
      m_left.reset(new CIVTreeNode(this, m_meta->m_l_x, Box(m_meta->m_l_hcp), m_meta->m_l_c));
      m_right.reset(new CIVTreeNode(this, m_meta->m_r_x, Box(m_meta->m_r_hcp), m_meta->m_r_c));
      delete m_meta;
      m_meta = 0;
    }
  }

  node_color_t CIVTree::CIVTreeNode::color_() const
  {
    return m_color;
  }

  void CIVTree::CIVTreeNode::join_()
  {
    if(m_left)
      m_left->join();
    m_left.reset();
    if(m_right)
      m_right->join();
    m_right.reset();
  }

  unsigned CIVTree::CIVTreeNode::children_() const
  {
    if(m_left)
      return 2;
    return 0;
  }

  bool CIVTree::CIVTreeNode::leaf_() const
  {
    return !m_left;
  }

  bool CIVTree::CIVTreeNode::root_() const
  {
    return !m_parent;
  }

  const CIVTree::CIVTreeNode& CIVTree::CIVTreeNode::operator[](unsigned idx) const
  {
    assert(!leaf() && "Child of leaf node requested!");
    switch(idx) {
    case 0:
      return *m_left;
    case 1:
      return *m_right;
    }
    assert(!"Non existing child requested!");
  }

  CIVTree::CIVTreeNode& CIVTree::CIVTreeNode::operator[](unsigned idx)
  {
    assert(!leaf() && "Child of leaf node requested!");
    switch(idx) {
    case 0:
      return *m_left;
    case 1:
      return *m_right;
    }
    assert(!"Non existing child requested!");
  }

  unsigned CIVTree::CIVTreeNode::level_() const
  {
    if(root())
      return 0;
    return m_parent->level() + 1;
  }

  const CIVTree::CIVTreeNode* CIVTree::CIVTreeNode::parent() const
  {
    return m_parent;
  }


  CIVTree::CIVTreeNode* CIVTree::CIVTreeNode::parent()
  {
    return m_parent;
  }


  const CIVTree& CIVTree::CIVTreeNode::tree() const
  {
    return m_tree;
  }


  CIVTree& CIVTree::CIVTreeNode::tree()
  {
    return m_tree;
  }

  bool CIVTree::progress(const ivector &ox, const ivector &nx)
  {
    mreal s_gamma = 0.25;
    unsigned dim = size(nx);
    mreal t(0.0);
    for(unsigned i = 0; i < dim; i++)
      t = std::max(t, wid(ox[i]) - wid(nx[i]));
    return s_gamma*wid(ox) - t <= 0;
  }

  void CIVTree::add_queue(const queue_t &qt)
  {
    bool found = false;
    for(std::vector<queue_t>::iterator it(m_queue.begin()); it != m_queue.end(); ++it)
      if(it->m_prio > qt.m_prio) {
	m_queue.insert(it, qt);
	found = true;
	break;
      }

    if(!found)
      m_queue.push_back( qt);
  }

  void CIVTree::add_enclosure(const functions::IEncl &enclosure, unsigned priority, bool use_con)
  {
    add_queue(queue_t(enclosure, priority, use_con));
  }

  void CIVTree::add_contractor(const functions::IContractor &contractor, unsigned priority)
  {
    add_queue(queue_t(contractor, priority));
  }

  CIVTree::CIVTree(const objects::IGeoObj &obj, const core::arith::ivector &bounds, bool use_normals)
    :m_obj(obj), m_normals(use_normals)
  {
    m_root.reset(new CIVTreeNode(*this, bounds));
    if(m_normals) {
      std::cerr << "Normalen HACK aktiv! FIX ME" << std::endl;
      add_enclosure(*(const functions::IEncl*)(nullptr), 0, false);
    }
  }

  CIVTree::CIVTree(const objects::IGeoObj &obj, bool use_normals)
    :m_obj(obj), m_normals(use_normals)
  {
    m_root.reset(new CIVTreeNode(*this, utility::unit_box(3)));
  }

  CIVTree::~CIVTree()
  {
  }

  const CIVTree::CIVTreeNode& CIVTree::root() const
  {
    return *m_root;
  }

  CIVTree::CIVTreeNode& CIVTree::root()
  {
    return *m_root;
  }

  CIVTree* CIVTree::clone() const
  {
    // FIX
    return 0;
  }

  const objects::IGeoObj& CIVTree::obj_() const
  {
    return m_obj;
  }

  void CIVTree::set_shadow_(bool)
  {

  }

  bool CIVTree::shadow_() const
  {

  }

  bool CIVTree::standard_() const
  {

  }

  bool CIVTree::normals_() const 
  {
    return m_normals;
  }

  void CIVTree::set_normals_(bool en)
  {
    m_normals = en;
  }

  CIVTree::queue_t::queue_t(const functions::IContractor &con, unsigned prio)
    :m_con(&con), m_prio(prio), m_type(CONTRACTOR)
  { }

  CIVTree::queue_t::queue_t(const functions::IEncl &encl, unsigned prio, bool hybrid)
    :m_encl(&encl), m_prio(prio), m_type(hybrid ? HYBRID : ENCLOSURE)
  { }

  CIVTree::CIVTreeNode::node_meta_t::node_meta_t(const core::arith::ivector &lx, node_color_t lc,
                                                 const core::arith::ivector &rx, node_color_t rc)
    :m_l_x(lx), m_l_hcp(lx), m_l_c(lc), m_r_x(rx), m_r_hcp(rx), m_r_c(rc)
  { }


  CIVTree::CIVTreeNode::node_meta_t::node_meta_t(const core::arith::ivector &lx, const core::arith::ivector &lhcp,
                                                 const core::arith::ivector &rx, const core::arith::ivector &rhcp)
    :m_l_x(lx), m_l_hcp(lhcp), m_l_c(GRAY), m_r_x(rx), m_r_hcp(rhcp), m_r_c(GRAY)
  {}
}
