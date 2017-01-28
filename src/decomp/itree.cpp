#include <decomp/itree.hpp>
namespace decomp 
{
  std::ostream& print_color(std::ostream &os, node_color_t c)
  {
    switch(c) {
    case 1:
      os << "WHITE";
      break;
    case 2:
      os << "BLACK";
      break;
    case 4:
      os << "GRAY";
      break;
    case 8:
      os << "WHITE_INV";
      break;
    default:
      os << "UNKNOWN";
    }
    return os;
  }
}
