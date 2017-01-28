%{
#include "stable.h"
#include "icsgnodefactory.hpp"
#include "standardcsgnodefactory.hpp"
%}

namespace objects 
{
  namespace details 
  {
    class StandardCSGNodeFactory : public ICSGNodeFactory {  };
  }
}

  %inline %{
    const objects::ICSGNodeFactory& get_csg_factory() 
    {
      static objects::ICSGNodeFactory *singleton = 
	new objects::details::StandardCSGNodeFactory;
      return *singleton;
    }
  %}

