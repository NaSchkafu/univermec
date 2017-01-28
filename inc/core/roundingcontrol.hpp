#ifndef __ROUNDINGCONTROL_HPP__
#define __ROUNDINGCONTROL_HPP__


// Diesen Header gibt es weil MSVC derzeit keinen C99 kompatiblen fenv Header mitbringt.
// Mit C++0x hat sich dies erledigt.
namespace core 
{
  enum ROUNDING_MODE { RND_UPWARD, RND_DOWNWARD, RND_TONEAREST, RND_TOWARDZERO } ;

  /** 
   * Gets the current rounding mode
   * 
   * 
   * @return current rounding mode
   */
  ROUNDING_MODE get_round();

  /** 
   * Sets the current rounding mode
   * 
   * @param mode rounding mode
   */
  void set_round(ROUNDING_MODE mode);
}

#endif /*__ROUNDINGCONTROL_HPP__*/
