#ifndef __IGPUHOST_HPP__
#define __IGPUHOST_HPP__

namespace functions
{
  namespace details
  {

    class IGPUHostFuture;

    /// Interface for GPU Host programs
    /**
     * This interface encapsulates the functions provided by a GPU
     * host program. It is used by the rest of the framework to start
     * GPU computations.
     */
    class IGPUHost
    {
    public:
      /**
       * Opens the device
       *
       */
      bool open_device()
        {
          return open_device_();
        }

      /**
       * Closes the device
       *
       */
      void close_device()
        {
          close_device_();
        }
      
      /**
       * Starts an interval computation of the GPU
       *
       * @param args arguments
       * @param size number of argument tuples
       * 
       * Note: size is the number of tuples, that is, args needs
       * dim*size*2 entries in total (where dim is the dimension of
       * the function to be evaluated)
       *
       * @return future to results 
       */
      IGPUHostFuture* comp_iv(double *args, unsigned size) const
        {
          return comp_iv_(args, size);
        }

      IGPUHostFuture* comp_rv(double *args, unsigned size) const
        {
          return comp_rv_(args, size);
        }


      // double* results_iv(const gpu_future_t* handle)
      //   {
      //     return results_iv_(handle);
      //   }

      // double* results_rv(const gpu_future_t* handle)
      //   {
      //     return results_rv_(handle);
      //   }


    private:
      virtual bool open_device_() = 0;
      virtual void close_device_() = 0;
      virtual IGPUHostFuture* comp_iv_(double* args, unsigned size) const = 0;
      virtual IGPUHostFuture* comp_rv_(double* args, unsigned size) const = 0;
      // virtual double* results_iv_(const gpu_future_t* handle) = 0;
      // virtual double* results_rv_(const gpu_future_t* handle) = 0;
    };
  }
}

#endif /*__IGPUHOST_HPP__*/
