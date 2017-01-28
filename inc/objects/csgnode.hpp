#include "stable.h"
#ifndef __CSGNODE_HPP__
#define __CSGNODE_HPP__

#include "igeoobj.hpp"

namespace objects
{

  class CSGNode : public IGeoObj
  {
  public:
    bool is_leaf() const;
    
	

  private:
    virtual unsigned primitives_() const;
    virtual unsigned functions_() const;
    virtual const IFunction& cf_() const;
    virtual boost::shared_ptr<IGeoObj> simplify_(const interval &X) const;
  };
}


#endif /*__CSGNODE_HPP__*/
