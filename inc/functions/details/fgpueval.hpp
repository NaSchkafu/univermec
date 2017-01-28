#ifndef __FGPUEVAL_HPP__
#define __FGPUEVAL_HPP__

#include <boost/foreach.hpp>

#include "core/box.hpp"

#include "fmaster.hpp"
#include "../igpueval.hpp"
#include "../igpufuture.hpp"

#include "igpuhost.hpp"
#include "gpufutureimpl.hpp"


namespace functions
{
  namespace details 
  {
    /// Feature class for the FVBase class hierarchy that offers GPU
    /// computation features
    /**
     * Use this feature class to enable GPU computations for an
     * IFunction built using the FVBase/FJoin template classes.
     *  
     */
    template <typename T>
    class FGPUEval : public IGPUEval, public virtual FMaster<T>
    {
    public:
      virtual ~FGPUEval(); 
      /** 
       * Sets a point to the GPU host code which will actually trigger
       * the computation on the GPU
       * 
       * @param host pointer to host code
       */
      void set_gpu_host(IGPUHost *host);

      /** 
       * Sets the block size for the memory layout
       * 
       * @param size block size
       */
      void set_block_size(unsigned size);
      
      void init();

      using FMaster<T>::get_f;

    private:
      virtual bool open_device_();
      virtual void close_device_();
      virtual IGPUFuture<core::arith::ivector>* comp_(const std::vector<core::arith::ivector> &args) const;
      virtual IGPUFuture<core::arith::ivector>* comp_(const std::vector<core::Box*> &args) const;
      virtual IGPUFuture<core::arith::rvector>* comp_(const std::vector<core::arith::rvector> &args) const;

    private:
      std::unique_ptr<IGPUHost> m_gpu_host;
      unsigned m_bsize;
    };
    // Impl
    #include "fgpueval.inl"
  }
}



#endif /*__FGPUEVAL_HPP__*/
