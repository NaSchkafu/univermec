%{
#include "stable.h"
#include "decomp/sivtree.hpp"
#include "decomp/contractingtree.hpp"
//#include "ui/treebuilder.hpp"
%}

//%include "ui/treebuilder.hpp"
//%template(tree_options_t) std::map<std::string, std::string>; 

%include "decomp/sivtree.hpp"
%include "decomp/contractingtree.hpp"

namespace decomp
{
  %template(BinIVTree) SIVTree<core::arith::interval>;
  %template(BinAAFTree) SIVTree<core::arith::aaf>;
  %template(BinTMTree)  SIVTree<core::arith::taylormodel>;
//  core::arith::mreal volume(const IIVTreeNode &t, node_color_t color);
}



