#ifndef __SPARAMTREE_HPP__
#define __SPARAMTREE_HPP__


#include "iivtree.hpp"

namespace objects
{
  class IParamSurface;
}

namespace decomp 
{
  class IParametricTree 
  {
  public:
    class IParametricTreeNode : public IIVTreeNode
    {
    public:
      inline core::arith::ivector p_x() const
	{
	  return p_x_();
	}

    protected:
      IParametricTreeNode(const core::arith::ivector &x)
        :IIVTreeNode(x)  {  }

    private:
      virtual core::arith::ivector p_x_() const = 0;
    };

    inline const objects::IParamSurface& p_surf() const
      {
	return p_surf_();
      }

  private:
    virtual const objects::IParamSurface& p_surf_() const = 0;
  };

  class SParamTree : public IIVTree, public IParametricTree
  {
  public:
    class SParamTreeNode : public IParametricTree::IParametricTreeNode
    {
    public:
      SParamTreeNode(SParamTree &tree, const core::arith::ivector &pd);
      virtual ~SParamTreeNode();

      // Funktionen sind hier direkt virtual deklariert
      // um implementierenden Klassen covariante Rückgaben zu erlauben
      virtual const SParamTreeNode& operator[](unsigned idx) const;
      virtual SParamTreeNode& operator[](unsigned idx);
      virtual const SParamTreeNode* parent() const;
      virtual SParamTreeNode* parent();
      virtual const SParamTree& tree() const;
      virtual SParamTree& tree();

    private:
      virtual node_color_t color_() const;
      virtual void join_();
      virtual void split_();
      virtual unsigned children_() const;
      virtual bool leaf_() const;
      virtual unsigned level_() const;
      virtual bool root_() const;
      virtual core::arith::ivector p_x_() const;
      virtual core::arith::ivector normal_() const;
      void init();
      SParamTreeNode(SParamTreeNode *parent, const core::arith::ivector &pd);

      SParamTreeNode(const SParamTreeNode&);
      SParamTreeNode& operator=(const SParamTreeNode&);

    private:
      SParamTree &m_tree;
      SParamTreeNode *m_parent;
      node_color_t m_color;
      std::unique_ptr<SParamTreeNode> m_left, m_right;
      core::arith::ivector m_pd;
      core::arith::ivector m_normal; 
    };
    
    SParamTree(const objects::IParamSurface &surf, bool normals);
    SParamTree(const objects::IParamSurface &surf, const core::arith::ivector &bounds, bool normals);
    virtual ~SParamTree();

    virtual const SParamTreeNode& root() const;
    virtual SParamTreeNode& root();
    virtual SParamTree* clone() const;

  private:
    virtual void set_shadow_(bool shadow);
    virtual bool shadow_() const;
    virtual bool standard_() const;
    virtual const objects::IParamSurface& p_surf_() const;
    virtual const objects::IGeoObj& obj_() const;
    virtual bool normals_() const;
    virtual void set_normals_(bool en);

  private:
    const objects::IParamSurface& m_surf;
    bool m_normals;
    SParamTreeNode m_root;
  };

}

#endif /*__SPARAMTREE_HPP__*/
