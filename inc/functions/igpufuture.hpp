#ifndef __IGPUFUTURE_HPP__
#define __IGPUFUTURE_HPP__

#include <vector>

namespace functions
{
  /// Future to a GPU computation result
  /** 
   * Usually, evaluations of IVFunction on the GPU  are carried out asynchronously. 
   * Using the returned IGPUFuture<T> the caller can obtain the actual results of the computation.
   */
  template <typename T>
  class IGPUFuture
  {
  public:
    /** 
     * Checks if the results are available
     * This function does not block.
     * 
     * 
     * @return true if the results are available
     */
    bool ready() const
      {
	return ready_();
      }

    /** 
     * Checks if the future is valid
     * This function does not block.
     * 
     * @return true if the future is valid
     */
    bool valid() const
      {
	return valid_();
      }

    /** 
     * Gets the results
     * Blocks until the results are available if necessary
     * 
     * @return vector containing the results
     */
    std::vector<T> results()
      {
	return results_();
      }

    virtual ~IGPUFuture() {}
    IGPUFuture() {}

  private:
    virtual bool ready_() const = 0;
    virtual bool valid_() const = 0;
    virtual std::vector<T> results_() = 0;

  private:
    IGPUFuture(const IGPUFuture<T> &other);
    IGPUFuture<T>& operator=(const IGPUFuture<T>&other);
  };
}

#endif /*__IGPUFUTURE_HPP__*/
