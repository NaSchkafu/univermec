#ifndef __DECOMPUTIL_CPP__
#define __DECOMPUTIL_CPP__

#include <decomp/decomputil.hpp>

#include <boost/foreach.hpp>

#include <decomp/itree.hpp>
#include <decomp/itreenode.hpp>
#include <decomp/iivtree.hpp>

#include "utility/utility.hpp"

using namespace core::arith;


namespace decomp
{
  ITreeNode* find_node2(ITree &t, node_color_t *col, unsigned level)
  {
    unsigned max_level = 0;
    std::deque<ITreeNode*> l;
    l.push_back(&t.root());
    while(!l.empty()) {
      ITreeNode &t = *l.front();
      l.pop_front();
      int tl = static_cast<int>(level) - static_cast<int>(t.level());
      if(tl <= 0)
        t.join();
      else if(tl > 0) {
        t.split();
        for(unsigned i = 0, children = t.children();
            i < children;i++) {
          if(col && t[i].color() == *col)
            return &t[i];
          l.push_back(&t[i]);
        }
      }
    }
    return 0;
  }

  ITreeNode* find_node(ITree &t, node_color_t color, unsigned level)
  {
    return find_node2(t, &color, level);
  }

  void split(ITree &t, unsigned level)
  {
    find_node2(t, 0, level);
  }

  mreal volume(const IIVTree &t, node_color_t color)
  {
    return volume(t.root(), color);
  }

  mreal volume(const IIVTreeNode &t, node_color_t color)
  {
    mreal vl = 0.0;
    if(t.color() == color && t.leaf()) {
      const std::vector<core::arith::ivector> &col = convert_inversion(t);
      BOOST_FOREACH(const core::arith::ivector &v, col) {
        vl += utility::box_volume(v);
      }
    }
    unsigned ch = t.children();
    for(unsigned i = 0; i < ch; i++)
      vl += volume(t[i], color);
    return vl;
  }

  std::vector<core::arith::ivector> convert_inversion(const IIVTreeNode &n)
  {
    std::vector<ivector> col;
    if(n.color() == WHITE_INV) {
      // Knoten beschreibt m_parent.X()/this->X()
      // diese Menge kann durch maximal 2*d Boxen genau dargestellt werden
      ivector cbox(n.parent()->X());
      unsigned dim = size(cbox);
      for(unsigned i = 0; i < dim; i++) {
        if(inf(cbox[i]) < inf(n.X()[i])) {
          ivector nbox(cbox);
          nbox[i] = interval(inf(cbox[i]), inf(n.X()[i]));
          col.push_back(nbox);
        }
        if(sup(cbox[i]) > sup(n.X()[i])) {
          ivector nbox(cbox);
          nbox[i] = interval(sup(n.X()[i]), sup(cbox[i]));
          col.push_back(nbox);
        }
        cbox[i] = interval(inf(n.X()[i]), sup(n.X()[i]));
      }
    }
    else
      col.push_back(n.X());
    return col;
  }
}

#endif /*__DECOMPUTIL_CPP__*/

