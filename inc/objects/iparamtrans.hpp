#ifndef __IPARAMTRANS_HPP__
#define __IPARAMTRANS_HPP__

#include "iparamsurface.hpp"

namespace objects 
{
  /// Represents a transformation of parametric surface
  class IParamTrans : public IParamSurface 
  {    
  public:
    /** 
     * Gets the child of the transformation
     * 
     * 
     * @return child function
     */
    inline const IParamSurface& child() const 
      {
	return child_();
      }

  private:
    virtual const IParamSurface& child_() const = 0;
  };
}

#endif /*__IPARAMTRANS_HPP__*/
