#ifndef __IIVTREE_HPP__
#define __IIVTREE_HPP__

#include <core/arithcore.hpp>
#include "itree.hpp"
#include "core/box.hpp"

namespace decomp
{
  class IIVTree : public ITree
  {
  public:
    /// interface for interval trees
    /**
     * An interval tree is a spatial space decomposition scheme
     * where every node can be exaclty(!) described by a finite number
     * of interval vectors (boxes).
     */
    class IIVTreeNode : public ITreeNode, public core::Box
    {
    public:
      virtual ~IIVTreeNode()  { }


      // Funktionen sind hier direkt virtual deklariert
      // um implementierenden Klassen covariante Rückgaben zu erlauben
      virtual const IIVTreeNode& operator[](unsigned idx) const = 0;
      virtual IIVTreeNode& operator[](unsigned idx) = 0;
      virtual const IIVTreeNode* parent() const = 0;
      virtual IIVTreeNode* parent() = 0;
      inline core::arith::ivector normal()
	{
	  return normal_();
	}

    protected:
      IIVTreeNode(const core::arith::ivector &x)
        :Box(x)  {  }


    private:
      IIVTreeNode(const IIVTreeNode&);
      IIVTreeNode& operator=(const IIVTreeNode&);
      virtual core::arith::ivector normal_() const = 0;
    };

    virtual const IIVTreeNode& root() const = 0;
    virtual IIVTreeNode& root() = 0;

    /**
     * Checks if the tree is a standard tree
     * A standard tree only uses the node colors BLACK, WHITE and GRAY
     *
     * @return true if a standard tree
     */
    inline bool standard() const
      {
        return standard_();
      }

    /**
     * Checks whether the shadow mode for non standard nodes is active
     * A non standard node is automatically mapped to a set of standard nodes
     * through the set inversion algorithm
     *
     * @return true if shadow mode is active
     */
    inline bool shadow() const
      {
        return shadow_();
      }

    /**
     * Sets the shadow mode
     * If standard() is true, this method has no effect
     *
     * @param shadow new shadow mode
     */
    inline void set_shadow(bool shadow)
      {
        set_shadow_(shadow);
      }

    inline bool normals() const 
      {
	return normals_();
      }

    inline void set_normals(bool en)
      {
	set_normals_(en);
      }

  private:
    virtual void set_shadow_(bool shadow) = 0;
    virtual bool shadow_() const = 0;
    virtual bool standard_() const = 0;
    virtual bool normals_() const = 0;
    virtual void set_normals_(bool en) = 0;

  };

  typedef IIVTree::IIVTreeNode IIVTreeNode;
}

#endif /*__IIVTREE_HPP__*/
