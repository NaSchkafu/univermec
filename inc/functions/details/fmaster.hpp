#ifndef __FMASTER_HPP__
#define __FMASTER_HPP__

namespace functions
{
  namespace details
  {


    /// common base class for function implementation
    /**
     * This is a common base class for all templates used for implemention of IFunction.
     * Using this pattern, delegation to a another class using multiple inheritance is possible.
     * Basically we need this to ensure that only one instance of the functor T is stored for every function.
     *
     * This class is however only a interface, the actual work is done in FBase<T,E>, which is
     * responsible for storing the functor.
     *
     */
    template<typename T>
    class FMaster
    {
    public:
      T& get_f()
        {
          return get_f_();
        }

      const T& get_f() const
        {
          return get_f_();
        }

      unsigned f_cdim() const
	{
	  return f_cdim_();
	}

    private:
      virtual T& get_f_()  = 0;
      virtual const T& get_f_() const = 0;
      virtual unsigned f_cdim_() const = 0;
    };
  }
}

#endif /*__FMASTER_HPP__*/
