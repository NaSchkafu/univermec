#ifndef __LOCAL_STATE_HPP__
#define __LOCAL_STATE_HPP__

#include "globoptobserver.hpp"

namespace GlobOpt 
{
  class OptBox;
  class IOptStrategy;

  /// local state of a worker thread
  struct local_opt_env_t
  {
    local_opt_env_t()
      :to_add(0)
      {}
    /// box to add with split
    OptBox *to_add;
    /// to add but without split
    std::vector<OptBox*> no_split_lst;
    /// solution list
    GlobOptAlgo::FinalLst final_lst;
    /// list for temporary not processed boxes
    std::forward_list<OptBox*> tmp_lst;
    /// statistics
    GlobOptStatistic stats;
    /// local strategy
    std::unique_ptr<IOptStrategy> strategy;
  };

}

#endif /*__LOCAL_STATE_HPP__*/
