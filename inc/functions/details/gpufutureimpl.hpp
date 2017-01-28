#ifndef __GPUFUTUREIMPL_HPP__
#define __GPUFUTUREIMPL_HPP__

#include "../igpufuture.hpp"
#include <memory>

namespace functions
{
  namespace details
  {
    class IGPUHostFuture;

    template <typename T>
    class GPUFutureImpl : public IGPUFuture<T>
    {
    public:
      explicit GPUFutureImpl(IGPUHostFuture *host);

    private:
      virtual bool ready_() const;
      virtual bool valid_() const;
      virtual std::vector<T> results_();

    private:
      /// GPU host program
      std::unique_ptr<IGPUHostFuture> m_host;
    };
  }
}

#endif /*__GPUFUTUREIMPL_HPP__*/
