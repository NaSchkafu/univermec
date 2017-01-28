#ifndef __IGPUFUTUREHOST_HPP__
#define __IGPUFUTUREHOST_HPP__

namespace functions
{
  namespace details
  {
    /// Interface that links the visible future interface to the
    /// actual GPU host code
    class IGPUHostFuture
    {
    public:
      /**
       * Checks if the results are available
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
       *
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
       * @param size set to the number of argument tuples
       * 
       * @return vector containing the results
       */
      double* results(unsigned &size)
        {
          return results_(size);
        }

      virtual ~IGPUHostFuture() {}
      IGPUHostFuture() {}

    private:
      virtual bool ready_() const = 0;
      virtual bool valid_() const = 0;
      virtual double* results_(unsigned &size) = 0;
    };
  }
}

#endif /*__IGPUFUTUREHOST_HPP__*/
