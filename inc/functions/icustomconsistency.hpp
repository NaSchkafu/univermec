#ifndef __ICUSTOMCONSISTENCY_HPP__
#define __ICUSTOMCONSISTENCY_HPP__

namespace functions
{
  class IContractor;

  /// Provides access to specialized consistency methods for functions
  /**
   * [Contractors](@ref IContractor) which work only on a specific
   * @ref IVFunction instance can be bound to this function with its
   * [feature set](@ref IFFeatures). 
   * 
   * The ICustomConsistency interface can be accessed through this
   * feature set and allows obtaining the contractors. Basically, it
   * is just a container-like class for them.
   */
  class ICustomConsistency
  {
  public:
    /** 
     * Gets the number of available contractors
     * 
     * 
     * @return number of available contractors
     */
    inline unsigned size() const
      {
        return size_();
      }

    /** 
     * Gets a contractor
     * 
     * @param idx index of the contractor (0 <= idx < size() must hold)
     * 
     * @return reference to the contractor
     */
    inline const IContractor& get(unsigned idx = 0) const
      {
        return get_(idx);
      }

    virtual ~ICustomConsistency() { }

  private:
    virtual unsigned size_() const = 0;
    virtual const IContractor& get_(unsigned idx) const = 0;
  };
}

#endif /*__ICUSTOMCONSISTENCY_HPP__*/
