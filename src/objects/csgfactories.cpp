#include <objects/csgfactories.hpp>

#include <objects/icsgnodefactory.hpp>
#include <objects/csg/standardcsgnodefactory.hpp>

namespace objects
{
  // hier alle CSGFactories registrieren
  CSGFactoriesImpl::CSGFactoriesImpl()
    :m_default(CSG_MIN_MAX_FACTORY)
  {
    ICSGNodeFactory *fac = new details::StandardCSGNodeFactory();
    m_factories.push_back(fac);
  }

  const ICSGNodeFactory& CSGFactoriesImpl::get(csg_factory_t type) const
  {
    if(type == DEFAULT)
      type = m_default;
    if(type >= 0 && static_cast<unsigned>(type) < m_factories.size())
      return m_factories[type];
    assert(!"Non existent factory requested!");
    // TODO: Exception schmeißen
    //throw 
    // wg. Compilerwarnung
    return m_factories[0];
  }

  CSGFactoriesImpl::csg_factory_t CSGFactoriesImpl::get_default() const 
  {
    return m_default;
  }

  void CSGFactoriesImpl::set_default(csg_factory_t def) 
  {
    m_default = def;
  }

}
