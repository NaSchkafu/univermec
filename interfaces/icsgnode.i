%{
#include "stable.h"
#include "igeoobj.hpp"
#include "iterator_helper.hpp"
#include "icsgnode.hpp"
%}

%nodefaultctor objects::ICSGNode;
%nodefaultctor objects::ICSGLeaf;
%nodefaultctor objects::ICSGTransform;

namespace objects 
{
  class ICSGNode;
  template<class VTYPE>
  class INonConvexPolyhedron;
  enum CSG_NODE_TYPE { CSG_LEAF = 1, CSG_INVERSION = 2, CSG_UNION = 3, CSG_INTERSECTION = 4, 
		       CSG_TRANSFORMATION = 5, CSG_UNKNOWN = 0 
  };

  class ICSGNode : public IGeoObj
  {
  public:
    CSG_NODE_TYPE type() const;
    
    unsigned children() const;
    
    bool leaf() const;
    
    const ICSGNode* parent() const;
    
    ICSGNode* parent();
        
    bool add(ICSGNode* child);
    
    bool remove(unsigned i);
        
    unsigned max_children() const;
    
    unsigned min_children() const;
    
    ICSGNode* clone() const;
    
    virtual ~ICSGNode() {  }
  };

  %extend ICSGNode 
  {
    const ICSGNode& at(unsigned i) const 
    {
      return (*$self)[i];
    }

    ICSGNode& at(unsigned i) 
    {
      return (*$self)[i];
    }
  }
  /// interface for CSG leaf nodes
  class ICSGLeaf : public virtual ICSGNode
  {
  public:
    const IGeoObj& obj() const;
    
    virtual ~ICSGLeaf() { }
  };  

  /// interface for CSG transformation nodes
  class ICSGTransform : public virtual ICSGNode
  {
  public:
    core::arith::imatrix rot() const;
    
    core::arith::imatrix i_rot() const;
    
    core::arith::ivector trans() const;
    
    core::arith::ivector i_trans() const;
    
    virtual ~ICSGTransform() {  }
  };
}
