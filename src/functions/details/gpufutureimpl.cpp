#include "functions/details/gpufutureimpl.hpp"

#include "functions/details/igpuhostfuture.hpp"
#include "core/arithcore.hpp"

namespace functions
{
  namespace details
  {
    std::vector<core::arith::ivector> result_vector(double *results, unsigned size,
						    const core::arith::ivector&)
    {
      std::vector<core::arith::ivector> ret(size);
      for(unsigned i = 0; i < size; i++) {
        // FIX ME: cdim > 1!
        ret[i] = core::arith::ivector(1, core::arith::interval(results[2*i], results[2*i+1]));
      }
      return ret;
    }

    std::vector<core::arith::rvector> result_vector(double *results, unsigned size, 
						    const core::arith::rvector&)
    {
      std::vector<core::arith::rvector> ret(size);
      for(unsigned i = 0; i < size; i++) {
        // FIX ME: cdim > 1!
        ret[i] = core::arith::rvector(1, core::arith::mreal(results[i]));
      }
      return ret;
    }

    template <typename T>
    GPUFutureImpl<T>::GPUFutureImpl(IGPUHostFuture* host)
      :m_host(host)
    {}

    template <typename T>
    bool GPUFutureImpl<T>::ready_() const
    {
      return m_host && m_host->ready();
    }

    template <typename T>
    bool GPUFutureImpl<T>::valid_() const
    {
      return m_host.get();
    }

    template <typename T>
    std::vector<T> GPUFutureImpl<T>::results_()
    {
      unsigned size;
      double* results = m_host->results(size);
      const std::vector<T>& ret = result_vector(results, size, T());
      delete[] results;
      m_host.reset();
      return ret;
    }

    template class GPUFutureImpl<core::arith::ivector>;
    template class GPUFutureImpl<core::arith::rvector>;
  }
}
