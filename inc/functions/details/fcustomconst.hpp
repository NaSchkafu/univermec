#include "functions/icustomconsistency.hpp"
#include "functions/icontractor.hpp"
#include "customconstimpl.hpp"

namespace functions
{
  namespace details
  {
    /// Provides easy access to a single consistency technique!
    /**
     * This interface accesses the underlying functor through the FMaster<T>
     * interface and call T::prune. 
     * 
     * Additional consistencies may be added through the add() method of CustomConstImpl
     */
    template<typename T>
    class FCustomConst : public CustomConstImpl, virtual public FMaster<T>
    {
    public:
      virtual ~FCustomConst();

      void init();
    };

    /// Wrapper class for accessig prune of a functor through the IConsistency interface
    template<typename T>
    class FIConstBase : public IContractor
    {
    public:
      explicit FIConstBase(const FCustomConst<T> &base);
      virtual ~FIConstBase();

    private:
      virtual bool prune_(core::Box &box, const core::arith::interval &rhs) const;
      virtual bool prune_(core::GappedBox &box, const core::arith::interval &rhs) const;
      virtual std::string text_() const;
      virtual float check_box_(const core::Box &box) const;
    private:
      const FCustomConst<T> &m_c_base;
    };


    #include "fcustomconst.inl"
  }
}
