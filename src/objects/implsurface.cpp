#include <objects/implsurface.hpp>

#include "functions/ifunction.hpp"
#include "functions/ivfunction.hpp"
#include <objects/details/implnormals.hpp>
#include "utility/utility.hpp"

namespace objects 
{

  ImplSurface::ImplSurface(const functions::IFunction &f)
    :m_F(f), m_nf(details::normal_fun(f))
  { }
  
  const functions::IFunction& ImplSurface::cf_() const 
  {
    return m_F;
  }

  IGeoObj* ImplSurface::clone_() const
  {
    return new ImplSurface(m_F);
  }

  unsigned ImplSurface::dim_() const
  {
    return m_F.dim();
  }

  const functions::IVFunction* ImplSurface::normals_() const
  {
    return m_nf.get();
  }

  ImplSurface::~ImplSurface()
  {}

}
