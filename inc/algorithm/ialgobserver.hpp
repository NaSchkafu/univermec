#include "stable.h"
#ifndef __IALGOBSERVER_HPP__
#define __IALGOBSERVER_HPP__

namespace algorithm 
{
  
  enum alg_finish_status_t { ALG_SUCCESS, ALG_ERROR, ALG_ITERATES_EX, ALG_TIME_EX, ALG_USER_ABORT };
  
  /// interface for algorithm observers
  class IAlgObserver 
  {
  public:
    inline void finish(alg_finish_status_t status) 
      {
	finish_(status);
      }

  private:
    virtual void finish_(alg_finish_status_t status) = 0;
  };
  
}

#endif /*__IALGOBSERVER_HPP__*/
