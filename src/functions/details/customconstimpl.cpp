#include <functions/details/customconstimpl.hpp>
#include <functions/icontractor.hpp>

namespace functions
{
  namespace details
  {
    CustomConstImpl::~CustomConstImpl()
    {
    }

    void CustomConstImpl::add(IContractor* co)
    {
      m_const.push_back(co);
    }

    unsigned CustomConstImpl::size_() const
    {
      return m_const.size();
    }

    const IContractor& CustomConstImpl::get_(unsigned idx) const
    {
      assert(idx < m_const.size() && "Consistency idx out of range.");
      return m_const[idx];
    }
  }
}
