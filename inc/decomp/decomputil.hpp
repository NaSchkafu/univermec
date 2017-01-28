#ifndef __DECOMPUTILITY_HPP__
#define __DECOMPUTILITY_HPP__ 

#include "itree.hpp"
#include "iivtree.hpp"

namespace decomp 
{

  /** 
   * Splits the tree until a node of the specified color is found
   * 
   * @param t root node of the tree
   * @param color color to search
   * @param level maximum level
   *  
   * @return pointer to the node or 0 if not found
   */
  ITreeNode* find_node(ITree &t, node_color_t color, 
		     unsigned level = std::numeric_limits<int>::max());

  /*! 
  ** Splits the tree up to a specified depth
  ** It collapses all nodes with level() > level and splits
  *  all nodes with level() < level until level() == level 
  ** 
  ** @param t root node of the tree
  ** @param level maximum tree level after completion of the tree
  */
  void split(ITree &t, unsigned level);

  /*! 
  ** Counts the number of (child) nodes
  ** 
  ** @param t start node
  ** 
  ** @return number of child nodes of t (including t)
  */
  inline unsigned node_count(const ITreeNode& t)
  {
    unsigned count = 1;
    unsigned ch = t.children();
    for(unsigned i = 0; i < ch; i++)
      count += node_count(t[i]);
    return count;
  }

  inline unsigned node_count(const ITree &t)
  {
    return node_count(t.root());
  }


  /*! 
  ** Counts the number of (child) nodes
  ** 
  ** @param t start node
  ** 
  ** @return number of child nodes of t (including t)
  */
  template<typename T>
  unsigned node_count(const T& t, node_color_t color)
  {
    unsigned count = t.color() == color ? 1 : 0;
    unsigned ch = t.children();
    for(unsigned i = 0; i < ch; i++)
      count += node_count(t[i]);
    return count;
  }

  inline bool is_white(const ITreeNode &n)
  {
    return n.color() & 0x9;
  }
  
  inline bool is_black(const ITreeNode &n)
  {
    return n.color() & 0x2;
  }
  
  inline bool is_gray(const ITreeNode &n)
  {
    return n.color() & 0x4;
  }

  inline bool is_not_white(const ITreeNode &n)
  {
    return n.color() & 64;
  }

  inline bool is_solid(const ITreeNode &n) 
  {
    return is_black(n) || is_white(n);
  }

  inline bool is_dividable(const ITreeNode &n)
  {
    return is_gray(n) || is_not_white(n);
  }

  std::vector<core::arith::ivector> convert_inversion(const IIVTreeNode &n);

  core::arith::mreal volume(const IIVTree &t, node_color_t color);
  core::arith::mreal volume(const IIVTreeNode &t, node_color_t color);

  

}

#endif /*__DECOMPUTILITY_HPP__ */

