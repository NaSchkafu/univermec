#ifndef __IGPUEVAL_HPP__
#define __IGPUEVAL_HPP__

#include <vector>
#include <core/arithcore.hpp>

namespace core
{
  class Box;
}

namespace functions
{

  template <typename T>
  class IGPUFuture;

  /// Interface for evaluating a IVFunction instance on the GPU
  /**
   * This interface can be used to evaluate a IVFunction object simultaneously over multiple points
   * or intervals using the GPU if the respective IVFunction instance supports this feature.
   * 
   * Additionally, asynchronous GPU computations are supported using a [future](@ref IGPUFuture) 
   * mechanism. However, this is not guaranteed and a specific GPU kernel can choose to block until
   * the computation finished and return the future holding the already computed results.
   */
  class IGPUEval 
  {
  public:
    /** 
     * Tries to initialize the GPU 
     * 
     * @return true if the operation succeeded
     */
    bool open_device()
      {
	return open_device_();
      }

    /** 
     * Closes the GPU device
     */
    void close_device()
      {
	close_device_();
      }

    /** 
     * Starts a computation on the device
     * 
     * @param args list of function arguments
     * 
     * @return future to results
     */
    IGPUFuture<core::arith::ivector>* comp(const std::vector<core::arith::ivector> &args) const
      {
	return comp_(args);
      }

    /** 
     * Starts a computation on the device
     * Convenience function to work on box pointers
     * (Used e.g. by the global optimization algorithm)
     * 
     * @param args list of function arguments
     * 
     * @return future to results
     */
    IGPUFuture<core::arith::ivector>* comp(const std::vector<core::Box*> &args) const
      {
	return comp_(args);
      }


    /** 
     * Starts a computation on the device
     * 
     * @param args list of function arguments
     * 
     * @return future to results
     */
    IGPUFuture<core::arith::rvector>* comp(const std::vector<core::arith::rvector> &args) const
      {
	return comp_(args);
      }

    IGPUEval() {}

  protected:
    virtual ~IGPUEval() {}

  private:
    virtual bool open_device_() = 0;
    virtual void close_device_() = 0;
    virtual IGPUFuture<core::arith::ivector>* comp_(const std::vector<core::arith::ivector> &args) const = 0;
    virtual IGPUFuture<core::arith::rvector>* comp_(const std::vector<core::arith::rvector> &args) const = 0;
    virtual IGPUFuture<core::arith::ivector>* comp_(const std::vector<core::Box*> &args) const = 0;

    IGPUEval(const IGPUEval&);
    IGPUEval& operator=(const IGPUEval&);
  };
}

#endif /*__IGPUEVAL_HPP__*/
