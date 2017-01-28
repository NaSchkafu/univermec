#ifndef __NODECOLORS_HPP__
#define __NODECOLORS_HPP__

namespace decomp 
{
  enum node_color_t { WHITE = 1, BLACK = 2, GRAY = 4, WHITE_INV = 8, BLACK_INV = 16, GRAY_INV = 32, 
		      NOT_WHITE = 64};
}

#endif /*__NODECOLORS_HPP__*/
