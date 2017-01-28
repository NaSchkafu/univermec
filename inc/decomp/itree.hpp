#ifndef __ITREE_HPP__
#define __ITREE_HPP__

#include <core/arithcore.hpp>
#include "itreenode.hpp"

namespace objects 
{
  class IGeoObj;
}

namespace decomp
{

  /// interface for all tree deocmposition structures
  /**
   * A tree is always an object tree, that is, it is associated with a geometric object.
   * The object is decomposed by the tree.
   */
  class ITree
  {
  public:
    virtual ~ITree() {  }
    ITree() { }

    // virtual für kovariante Rückgabe
    /** 
     * Gets a reference to the root node
     * 
     * 
     * @return root node of tree
     */
    virtual const ITreeNode& root() const = 0;
    /** 
     * Gets a reference to the root node
     * 
     * 
     * @return root node of tree
     */
    virtual ITreeNode& root() = 0;

    /** 
     * Returns a deep copy of this object
     * (All nodes are also copied)
     * 
     * @return copy of this object
     */
    virtual ITree* clone() const = 0;

    inline const objects::IGeoObj& obj() const 
      {
	return obj_();
      }
    
    // inline objects::IGeoObj& obj() 
    //   {
    // 	return obj_();
    //   }
    
	

  private:
    ITree(const ITree&);
    ITree& operator=(const ITree&);
    virtual const objects::IGeoObj& obj_() const = 0;
    //virtual objects::IGeoObj& obj_() = 0;
  };

}

#endif /*__ITREE_HPP__*/
