#ifndef __ITREENODE_HPP__
#define __ITREENODE_HPP__

#include <core/arithcore.hpp>
#include "nodecolors.hpp"

namespace decomp
{

  class ITree;
  
  std::ostream& print_color(std::ostream& os, node_color_t c);
  /// interface for all tree deocmposition structures
  class ITreeNode
  {
  public:
    virtual ~ITreeNode() {  }
    ITreeNode() { }

    // Funktionen sind hier direkt virtual deklariert
    // um implementierenden Klassen covariante Rückgaben zu erlauben
    virtual const ITreeNode& operator[](unsigned idx) const = 0;
    virtual ITreeNode& operator[](unsigned idx) = 0;
    virtual const ITreeNode* parent() const = 0;
    virtual ITreeNode* parent() = 0;
    virtual const ITree& tree() const = 0;
    virtual ITree& tree() = 0;

    // TODO: iteratoren?
    inline bool root() const
      {
	return root_();
      }

    inline void split()
      {
        split_();
      }

    inline void join()
      {
        join_();
      }

    inline bool leaf() const
      {
        return leaf_();
      }

    inline unsigned level() const
      {
        return level_();
      }


    inline node_color_t color() const 
      {
	return color_();
      }

    inline size_t children() const 
      {
	return children_();
      }
        
  private:
    virtual node_color_t color_() const = 0;
    virtual void join_() = 0;
    virtual void split_()= 0;
    virtual unsigned children_() const = 0;
    virtual bool leaf_() const = 0;
    virtual unsigned level_() const = 0;
    virtual bool root_() const = 0;

    ITreeNode(const ITreeNode&);
    ITreeNode& operator=(const ITreeNode&);

  }; 
}

#endif /*__ITREENode_HPP__*/
