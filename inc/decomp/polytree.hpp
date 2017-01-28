#ifndef __POLYIVTREE_HPP__
#define __POLYIVTREE_HPP__

#include "iivtree.hpp"
// TODO C11
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

namespace objects
{
  class IGeoObj;
}

namespace functions
{
  class IFunction;
}

namespace decomp
{

  class PolyIVTree : public IIVTree
  {
  public:
    class PolyIVTreeNode : public IIVTreeNode
    {
    public:
      enum face_info_t { NONE, SKIP, INTERSECTS, GRAY_DISTANCE };
      PolyIVTreeNode(PolyIVTree& tree, const core::arith::ivector &x);
      virtual ~PolyIVTreeNode();

      const core::arith::rvector* get_ray_vector() const;
      void set_ray_vector(const core::arith::rvector *vect);
      std::vector<face_info_t>* get_cache();
      std::vector<face_info_t>* reset_cache(size_t size);
      std::vector<const core::arith::rvector*> *init_vertex_list(const std::vector<core::arith::rvector> &vertex_list);
      std::vector<const core::arith::rvector*> *get_vertex_list();
      static PolyIVTreeNode *active_node;

      virtual const IIVTreeNode& operator[](unsigned idx) const;
      virtual IIVTreeNode& operator[](unsigned idx);
      virtual const IIVTreeNode* parent() const;
      virtual IIVTreeNode* parent();
      virtual PolyIVTree& tree();
      virtual const PolyIVTree& tree() const;

    private:
      virtual void split_();
      virtual unsigned level_() const;
      virtual node_color_t color_() const;
      virtual void join_();
      virtual bool leaf_() const;
      virtual bool root_() const;
      virtual unsigned children_() const;
      virtual core::arith::ivector normal_() const
	{
	  return core::arith::ivector(0);
	}
      void init();
      PolyIVTreeNode(PolyIVTreeNode *parent, const::core::arith::ivector &X);

      node_color_t m_color;
      PolyIVTreeNode* m_parent;
      PolyIVTree& m_tree;
      std::vector<face_info_t> *m_cache;
      std::vector<const core::arith::rvector*> *m_vertex_list;
      boost::shared_ptr<core::arith::rvector> m_ray_vector;

      boost::scoped_ptr<PolyIVTreeNode> m_left;
      boost::scoped_ptr<PolyIVTreeNode> m_right;

    private:
      PolyIVTreeNode(const PolyIVTreeNode&);
      PolyIVTreeNode& operator=(const PolyIVTreeNode&);
    };


    PolyIVTree(const objects::IGeoObj &F,
               const core::arith::ivector &X, unsigned skip_split = 0, unsigned octa_split = 0);

    virtual ~PolyIVTree();

    typedef PolyIVTreeNode node_t;

    virtual const node_t& root() const;
    virtual node_t& root();
    virtual PolyIVTree* clone() const;

    void set_split_params(unsigned skip_split, unsigned octa_split);
    void get_split_params(unsigned *skip_split, unsigned *octa_split);

  private:
    void init();
    virtual bool shadow_() const;
    virtual void set_shadow_(bool);
    virtual bool standard_() const;
    virtual const objects::IGeoObj& obj_() const;
    virtual bool normals_() const
      {
	return false;
      }
    virtual void set_normals_(bool) {  }
    
	

  private:
    const objects::IGeoObj& m_obj;
    unsigned m_skip_split, m_octa_split;
    PolyIVTreeNode m_root;
  };

}

#endif
