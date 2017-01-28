// -*-c++-*-
#ifndef __ICSGNODE_HPP__
#define __ICSGNODE_HPP__ 

#include "igeoobj.hpp"
#include "utility/iterator_helper.hpp"

namespace objects 
{
  
  class ICSGNode;
  /// iterator for CSG trees
  /// const iterator for CSG trees
  //typedef utility::tree_iterator<ICSGNode> const_csg_iterator;
  template<class VTYPE>
  class INonConvexPolyhedron;

  /// CSG Node Types
  enum CSG_NODE_TYPE { CSG_LEAF = 1, CSG_INVERSION = 2, CSG_UNION = 3, CSG_INTERSECTION = 4, 
		       CSG_TRANSFORMATION = 5, CSG_UNKNOWN = 0 
  };

  /// a node in a CSG tree
  /*!
  ** Interface for CSG Nodes
  *  Every CSG Node has to inherit this interface. 
  ** 
  */
  class ICSGNode : public IGeoObj
  {
  public:
    /// children iterator
    typedef utility::tree_iterator<ICSGNode> iterator;
    /// const children iterator
    typedef utility::tree_iterator<ICSGNode const> const_iterator;
    
    /*! 
    ** Gets the type of this node
    ** 
    ** 
    ** @return the node type
    */
    inline CSG_NODE_TYPE type() const 
      {
	return type_();
      }

    /*! 
    ** Gets the number of child nodes
    ** 
    ** 
    ** @return number of child node
    */
    inline unsigned children() const 
      {
	return children_();
      }
    

    /*! 
    ** Checks if this node has node children
    ** 
    ** 
    ** @return true if leaf node
    */
    inline bool leaf() const
      {
	return leaf_();
      }
    

    /*! 
    ** Gets a pointer to the parent
    ** 
    ** 
    ** @return pointer to parent or null if root
    */
    inline const ICSGNode* parent() const
      {
	return parent_();
      }
    

    /*! 
    ** Gets a pointer to the parent
    ** 
    ** 
    ** @return pointer to parent or null if root
    */
    inline ICSGNode* parent()
      {
	return parent_();
      }
    


    /*! 
    ** Gets a child of this node
    ** Throws an exception of the child i does not exist 
    **
    ** @param i number of the child (zero based)
    ** 
    ** @return child number i
    */
    inline const ICSGNode& operator[](unsigned i) const
      {
	return child_(i);
      }
    
    
    /*! 
    ** Gets a child of this node
    ** Throws an exception of the child i does not exist 
    **
    ** @param i number of the child (zero based)
    ** 
    ** @return child number i
    */
    inline ICSGNode& operator[](unsigned i)
      {
	return child_(i);
      }
    
    
    /*! 
    ** Adds a new child to this node
    ** The ownership of the child node is transfered to this node! 
    **
    ** @param child pointer to new child node
    ** 
    ** @return true if successful, false if this node is full
    */
    inline bool add(ICSGNode* child)
      {
	return add_(child);
      }
    
    
    /*! 
    ** Removes a node child
    ** 
    ** @param i number of the child
    ** 
    ** @return true if success, false if the minimum number of children is reached
    */
    inline bool remove(unsigned i)
      {
	return remove_(i);
      }
    
    
    /*! 
    ** Gets the maximum number of allowed children
    ** 
    ** 
    ** @return number of max. allowed children
    */
    inline unsigned max_children() const
      {
	return max_children_();
      }
    
    /*! 
    ** Gets the minimum number of allowed children
    ** 
    ** 
    ** @return number of min. allowed children
    */
    inline unsigned min_children() const
      {
	return min_children_();
      }

    inline iterator begin()
      {
	return begin_();
      }

    inline iterator end()
      {
	return end_();
      }

    inline const_iterator begin() const
      {
	return begin_();
      }

    inline const_iterator end() const
      {
	return end_();
      }

    inline ICSGNode* clone() const
      {
	return clone_();
      }

    virtual ~ICSGNode() {  }

  protected:
    ICSGNode()  { }

  private:
    virtual unsigned children_() const = 0;
    virtual bool leaf_() const = 0;
    virtual const ICSGNode* parent_() const = 0;
    virtual ICSGNode* parent_() = 0;
    virtual const ICSGNode& child_(unsigned i) const = 0;
    virtual ICSGNode& child_(unsigned i)  = 0;
    virtual bool add_(ICSGNode *child) = 0;
    virtual bool remove_(unsigned i) = 0;
    virtual unsigned max_children_() const = 0;
    virtual unsigned min_children_() const = 0;
    virtual iterator begin_() = 0;
    virtual iterator end_() = 0;
    virtual const_iterator begin_() const = 0;
    virtual const_iterator end_() const = 0;
    virtual CSG_NODE_TYPE type_() const = 0;
    virtual ICSGNode* clone_() const = 0;

  private:
    ICSGNode(const ICSGNode &);
    ICSGNode& operator=(const ICSGNode &);
  };


  // Nicht umsetzbar, da Funktionen bei uns per Definition nur skalar sind
  // daher extra Interface um auf Transformationen zugreifen
  // /// interface for CSG nodes which can be evaluated independently from the tree
  // class ICSGIndependent : public virtual ICSGNode 
  // {
  // public:
  //   /*! 
  //   ** Characteristic function for this node which does not include
  //   ** child nodes
  //   ** 
  //   ** 
  //   ** @return reference to ch-function
  //   */
  //   inline const functions::IFunction& in_cf() const
  //     {
  // 	return in_cf_();
  //     }

  //   virtual ~ICSGIndependent() {  }

  // private:
  //   virtual const functions::IFunction& in_cf_() const = 0;
  // };
  // 

  /// interface for CSG leaf nodes
  class ICSGLeaf : public virtual ICSGNode
  {
  public:
    inline const IGeoObj& obj() const 
      {
	return obj_();
      }

    virtual ~ICSGLeaf() { }

  private:
    virtual const IGeoObj& obj_() const = 0;
  };  

  /// interface for CSG transformation nodes
  class ICSGTransform : public virtual ICSGNode
  {
  public:
    /*! 
    ** Gets the rotation matrix for this node
    ** 
    ** 
    ** @return rotation matrix
    */
    inline core::arith::imatrix rot() const
      {
	return rot_();
      }
    
    /*! 
    ** Gets the inverse rotation matrix for this node
    ** 
    ** 
    ** @return inverse rotation matrix
    */
    inline core::arith::imatrix i_rot() const 
      {
	return i_rot_();
      }
    
    /*! 
    ** Gets the translation for this node
    ** 
    ** 
    ** @return translation vector
    */
    inline core::arith::ivector trans() const
      {
	return trans_();
      }
    
    /*! 
    ** Gets the inverse translation for this node
    ** 
    ** 
    ** @return inverse translation
    */
    inline core::arith::ivector i_trans() const
      {
	return i_trans_();
      }

    virtual ~ICSGTransform() {  }
    
  private:
    virtual core::arith::imatrix rot_() const = 0;
    virtual core::arith::imatrix i_rot_() const = 0;
    virtual core::arith::ivector trans_() const = 0;
    virtual core::arith::ivector i_trans_() const = 0;
  };
}


#endif /*__ICSGNODE_HPP__*/
