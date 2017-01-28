#ifndef __CONTRACTINGTREE_HPP__
#define __CONTRACTINGTREE_HPP__

#include "iivtree.hpp"
// TODO C11
#include <boost/scoped_ptr.hpp>

namespace functions
{
  class IContractor;
  class IEncl;
}

namespace decomp 
{
  class CIVTree : public IIVTree
  {
  public:
    class CIVTreeNode : public IIVTreeNode 
    {
    public:
      virtual const CIVTreeNode& operator[](unsigned idx) const;
      virtual CIVTreeNode& operator[](unsigned idx);
      virtual const CIVTreeNode* parent() const;
      virtual CIVTreeNode* parent();
      virtual const CIVTree& tree() const;
      virtual CIVTree& tree();
      virtual ~CIVTreeNode();
      CIVTreeNode(CIVTree &tree, const core::arith::ivector &x);

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

      CIVTreeNode(CIVTreeNode* parent, const core::arith::ivector &x,  
		  const core::Box &hcp, node_color_t color);

      decomp::node_color_t get_color(const core::arith::interval &encl) const;

    private:
      struct node_meta_t 
      {
	node_meta_t(const core::arith::ivector &lx, node_color_t lc,
		    const core::arith::ivector &rx, node_color_t lr);

	node_meta_t(const core::arith::ivector &lx, const core::arith::ivector &lhcp, 
		    const core::arith::ivector &rx, const core::arith::ivector &rhcp);

	/// left node area
	core::arith::ivector m_l_x;
	/// left node pruned area
	core::arith::ivector m_l_hcp;
	/// left node color
	node_color_t m_l_c;

	/// left node area
	core::arith::ivector m_r_x;
	/// left node pruned area
	core::arith::ivector m_r_hcp;
	/// left node color
	node_color_t m_r_c;
      };
	

      CIVTree& m_tree;
      boost::scoped_ptr<CIVTreeNode> m_left, m_right;
      CIVTreeNode* m_parent;
      decomp::node_color_t m_color;
      core::Box m_hcp;
      node_meta_t *m_meta;
      core::arith::ivector m_normal;
    };

    CIVTree(const objects::IGeoObj &obj, const core::arith::ivector &bounds, bool use_normals);
    CIVTree(const objects::IGeoObj &obj, bool use_normals);

    virtual ~CIVTree();

    virtual const CIVTreeNode& root() const;
    virtual CIVTreeNode& root();
    virtual CIVTree* clone() const;

    /** 
     * Adds a contractor to this tree
     * 
     * @param contractor contractor to add
     * @param priority priority
     */
    void add_contractor(const functions::IContractor &contractor, unsigned priority);

    /** 
     * Adds an enclosure to this tree
     * 
     * @param enclosure enclosure to add
     * @param priority priority
     * @param use_con use custom contractors provided by enclosure if possible
     */
    void add_enclosure(const functions::IEncl &enclosure, unsigned priority, bool use_con);

  private:
    struct queue_t
    {
      enum queue_type_t { CONTRACTOR = 0, ENCLOSURE = 1, HYBRID = 2 };

      queue_t(const functions::IContractor &con, unsigned prio);
      queue_t(const functions::IEncl &encl, unsigned prio, bool hybrid);

      union {
	const functions::IContractor *m_con;
	const functions::IEncl *m_encl;
      };

      unsigned m_prio;
      queue_type_t m_type;
    };

    void init();
    virtual bool shadow_() const;
    virtual void set_shadow_(bool);
    virtual bool standard_() const;
    //virtual objects::IGeoObj& obj_();
    virtual const objects::IGeoObj& obj_() const;

    bool progress(const core::arith::ivector &ox, const core::arith::ivector &nx);
    void add_queue(const queue_t &qt);
    virtual bool normals_() const;
    virtual void set_normals_(bool en);

  private:
    const objects::IGeoObj &m_obj;
    /// items for the contractor and enclosure queue
    std::vector<queue_t> m_queue;
    boost::scoped_ptr<CIVTreeNode> m_root;
    bool m_normals;

  private:
    // vorerst kein copy ctor oder assigment
    // TODO: impl
    CIVTree(const CIVTree&);
    CIVTree& operator=(const CIVTree&);

  };
}

#endif /*__CONTRACTINGTREE_HPP__*/
