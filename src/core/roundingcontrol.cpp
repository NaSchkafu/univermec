#include <fenv.h>
#include <cassert>

#include <core/roundingcontrol.hpp>

namespace core 
{
 
  ROUNDING_MODE convert_from_c99(int rnd)
  {
    switch(rnd) {
    case FE_DOWNWARD:
      return RND_DOWNWARD;
    case FE_TONEAREST:
      return RND_TONEAREST;
    case FE_TOWARDZERO:
      return RND_TOWARDZERO;
    case FE_UPWARD:
      return RND_UPWARD;
    default:
      // TODO: Sollte nicht vorkommen, Exception
      assert(!"Invalid rounding mode!");
      return RND_TONEAREST;
    }
  }

  int convert_to_c99(ROUNDING_MODE mode) 
  {
    switch(mode) {
    case RND_DOWNWARD:
      return FE_DOWNWARD;
    case RND_TONEAREST:
      return FE_TONEAREST;
    case RND_TOWARDZERO:
      return FE_TOWARDZERO;
    case RND_UPWARD:
      return FE_UPWARD;
    default:
      // TODO: Sollte nicht vorkommen, Exception
      assert(!"Invalid rounding mode!");
      return FE_TONEAREST;
    }
  }

  ROUNDING_MODE get_round() 
  {
    return convert_from_c99(fegetround());
  }

  void set_round(ROUNDING_MODE mode) 
  {
    fesetround(convert_to_c99(mode));
  }
}
