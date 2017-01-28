#ifndef __IMPLSURFACE_HPP__
#define __IMPLSURFACE_HPP__

#include "igeoobj.hpp"

namespace objects
{
  /// class for constructing a geometric object from an implicit function
  /*!
  ** This class acts as wrapper for an implicit function. It enables use of the function
  ** as a geometric object.
  */
  class ImplSurface : public IGeoObj
  {
  public:
    /** 
     * Ctor
     * Construct a new implicit surface from the provided function
     *  
     * @param f reference to function
     * 
     */
    explicit ImplSurface(const functions::IFunction &f);
    virtual ~ImplSurface();

  private:
    virtual const functions::IFunction& cf_() const;
    virtual IGeoObj* clone_() const;
    virtual unsigned dim_() const;
    virtual const functions::IVFunction* normals_() const;

  private:
    const functions::IFunction& m_F;
    std::unique_ptr<functions::IVFunction> m_nf;
  };
}

#endif /*__IMPLSURFACE_HPP__*/

