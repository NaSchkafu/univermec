#ifndef __CSGFACTORIES_HPP__
#define __CSGFACTORIES_HPP__

#include <boost/ptr_container/ptr_vector.hpp>

namespace objects 
{
  class ICSGNodeFactory;

  /// class for accessing csg factories
  class CSGFactoriesImpl 
  {
  public:
    /// CSG Factory types
    /**
     * CSG_MIN_MAX_FACTORY: The standard implementation using min/max functions
     * DEFAULT: The default factory is used
     */
    enum csg_factory_t { CSG_MIN_MAX_FACTORY = 0, DEFAULT = 1024 };

    /** 
     * Gets a csg factory
     * 
     * @param type requested factory
     * 
     * @return reference to the factory
     */
    const ICSGNodeFactory& get(csg_factory_t type = DEFAULT) const;

    /** 
     * Sets the default factory
     * 
     * @param def new default factory type
     */
    void set_default(csg_factory_t def);

    /** 
     * Gets the current default factory
     * 
     * 
     * @return current default factory type
     */
    csg_factory_t get_default() const;

    // Comp zu Loki Singleton API

    static CSGFactoriesImpl& Instance()
      {
	return m_instance;
      }

    static CSGFactoriesImpl& instance()
      {
	return m_instance;
      }

    

  private:
    CSGFactoriesImpl();
    CSGFactoriesImpl& operator=(const CSGFactoriesImpl&);
    CSGFactoriesImpl(const CSGFactoriesImpl&);

  private:
    boost::ptr_vector<ICSGNodeFactory> m_factories;
    csg_factory_t m_default;

    static CSGFactoriesImpl m_instance;
  };

  /// singleton for accessing csg factories
  typedef CSGFactoriesImpl CSGFactories;
  
}

#endif /*__CSGFACTORIES_HPP__*/
