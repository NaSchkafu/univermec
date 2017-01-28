#ifndef __CUSTOMCONSTIMPL_HPP__
#define __CUSTOMCONSTIMPL_HPP__

#include <boost/ptr_container/ptr_vector.hpp>
#include "../icustomconsistency.hpp"

namespace functions
{
  namespace details 
  {
    /// implementation of ICustomConsistency interface
    class CustomConstImpl : public ICustomConsistency
    {
    public:
      virtual ~CustomConstImpl();

      /** 
       * Adds a new IConsistency object to this storage
       * The ownership is transfered to this object!
       * 
       * @param co pointer to IConsistency interface to add
       */
      void add(IContractor* co);
 
    private:
      virtual unsigned size_() const;
      virtual const IContractor& get_(unsigned idx) const;

    private:
      boost::ptr_vector<IContractor> m_const;
    };

  }
}


#endif /*__CUSTOMCONSTIMPL_HPP__*/
