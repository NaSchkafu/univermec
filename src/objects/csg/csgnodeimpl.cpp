#include <objects/csg/csgnodeimpl.hpp>

#include "functions/ifunction.hpp"

namespace objects
{
  namespace details
  {

    unsigned CSGNodeImpl::children_() const
    {
      return m_children.size();
    }

    bool CSGNodeImpl::leaf_() const
    {
      return m_children.empty();
    }

    const ICSGNode* CSGNodeImpl::parent_() const
    {
      return m_parent;
    }

    ICSGNode* CSGNodeImpl::parent_()
    {
      return m_parent;
    }

    const ICSGNode& CSGNodeImpl::child_(unsigned i) const
    {
      assert(i < m_children.size() && "Child does not exist!");
      // TODO: exception
      return m_children[i];
    }

    ICSGNode& CSGNodeImpl::child_(unsigned i)
    {
      assert(i < m_children.size() && "Child does not exist!");
      // TODO: exception
      return m_children[i];
    }

    bool CSGNodeImpl::add_(ICSGNode *child)
    {
      if(m_children.size() < m_max_ch) {
        m_children.push_back(child);
        return true;
      }
      return false;
    }

    bool CSGNodeImpl::remove_(unsigned i)
    {
      if(i >= m_children.size() || m_children.size() <= m_min_ch)
        return false;
      m_children.erase(m_children.begin() + i);
      return true;
    }

    unsigned CSGNodeImpl::max_children_() const
    {
      return m_max_ch;
    }

    unsigned CSGNodeImpl::min_children_() const
    {
      return m_min_ch;
    }

    CSGNodeImpl::~CSGNodeImpl()
    {
    }

    CSGNodeImpl::CSGNodeImpl(unsigned min_ch, unsigned max_ch, CSG_NODE_TYPE type, ICSGNode* parent)
      :m_min_ch(min_ch), m_max_ch(max_ch), m_parent(parent), m_type(type)
    {
    }

    // void CSGNodeImpl::set_cf(functions::IFunction *cf) 
    // {
    //   m_cf = std::auto_ptr<functions::IFunction>(cf);
    // }

    ICSGNode::iterator CSGNodeImpl::begin_()
    {
      return iterator(this);
    }
    
    ICSGNode::iterator CSGNodeImpl::end_()
    {
      return iterator();
    }

    ICSGNode::const_iterator CSGNodeImpl::begin_() const
    {
      return const_iterator(this);
    }
    
    ICSGNode::const_iterator CSGNodeImpl::end_() const
    {
      return const_iterator();
    }

    void CSGNodeImpl::set_parent(ICSGNode *parent)
    {
      // TODO: Logging!
      if(m_type == CSG_TRANSFORMATION) {
	std::cout << "Warning: Transformation inside the CSG tree. This possibly results in massive overestimation..."
		  << std::endl;
      }
      m_parent = parent;
    }

    // const functions::IFunction& CSGNodeImpl::cf_() const 
    // {
    //   return *m_cf;
    // }
    
    ICSGNode* CSGNodeImpl::clone_() const 
    {
      // CSGNodeImpl *c = new CSGNodeImpl(m_min_ch, m_max_ch, m_type, m_parent);
      // c->set_cf(m_cf.get()->clone());
      // BOOST_FOREACH(const ICSGNode &child, m_children) {
      // 	ICSGNode *childn = dynamic_cast<ICSGNode*>(child.clone());
      // 	if(!childn || !c->add_(childn))
      // 	  // TODO: Exception
      // 	  assert(!"Could not add cloned child. Invariants violated.");
      // }
      // return c;
      assert(!"Impl me");
      return 0;
    }
    
    // unsigned CSGNodeImpl::dim_() const 
    // {
    //   return m_cf.get()->dim();
    // }

    CSG_NODE_TYPE CSGNodeImpl::type_() const 
    {
      return m_type;
    }

    // ********************************************************************************
    // CSGLeafImpl
    // ********************************************************************************
    CSGLeafImpl::CSGLeafImpl(const objects::IGeoObj &obj, ICSGNode *parent)
      :CSGNodeImpl(0, 0, CSG_UNKNOWN, parent), m_obj(obj)
    { }

    const functions::IFunction& CSGLeafImpl::cf_() const 
    {
      return m_obj.cf();
    }
    
    unsigned CSGLeafImpl::dim_() const 
    {
      return m_obj.dim();
    }

    const IGeoObj& CSGLeafImpl::obj_() const 
    {
      return m_obj;
    }

    const functions::IVFunction* CSGLeafImpl::normals_() const
    {
      return m_obj.normals();
    }

  }
}
