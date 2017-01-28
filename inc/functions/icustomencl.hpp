#ifndef __ICUSTOMENCL_HPP__
#define __ICUSTOMENCL_HPP__

namespace functions
{

  class IEncl;
  
  /// Interface for providing custom enclosure methods for a function
    /**
   * [Enclosures](@ref IEncl) which work only on a specific
   * @ref IVFunction instance can be bound to this function with its
   * [feature set](@ref IFFeatures). 
   * 
   * The ICustomEncl interface can be accessed through this
   * feature set and allows obtaining the enclosures. Basically, it
   * is just a container-like class for them.
   */
  class ICustomEncl
  {
  public:
    /** 
     * Gets the number of available IEncl in this object
     * 
     * 
     * @return number of available enclosures
     */
    inline unsigned size() const
      {
        return size_();
      }

    /** 
     * Gets an enclosure instance
     * 
     * @param idx index of the enclosure (0 <= index < size() must hold)
     * 
     * @return reference to the enclosure
     */
    inline const IEncl& get(unsigned idx = 0) const
      {
        return get_(idx);
      }

  private:
    virtual unsigned size_() const = 0;
    virtual const IEncl& get_(unsigned idx) const = 0;
  };
}
#endif /*__ICUSTOMENCL_HPP__*/
