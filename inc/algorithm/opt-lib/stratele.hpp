#ifndef __STRATELE_HPP__
#define __STRATELE_HPP__

#include "optenv.hpp"

namespace GlobOpt 
{

  struct strat_ele_t 
  {
    virtual ~strat_ele_t() {}

    /** 
     * Registers the strategy element at the specified worker
     * 
     * @param env environment of the worker
     */
    void reg(opt_worker_state_t &env) const 
      {
	env.m_discards[name()] = 0;
      }

    /** 
     * Applies an optimization strategy element
     * 
     * @param env current optimization environment
     * 
     * @return true if the current box can be discarded
     */
    virtual bool apply(opt_worker_state_t &env) const = 0;

    virtual std::string name() const 
      {
	return "name() not implemented!";
      }
  };

  struct gpu_strat_ele_t
  {
    virtual ~gpu_strat_ele_t() {}

    /// A list of GPU boxes
    /**
     * The list holds pairs of boxes and a bool
     * If the box can be discarded the associated bool is set to true
     */
    typedef std::deque<std::pair<OptBox*, bool> > gpu_box_lst;

    /** 
     * Registers the strategy element at the specified worker
     * 
     * @param env environment of the worker
     */
    void reg(opt_worker_state_t &env) const 
      {
	env.m_discards[name()] = 0;
      }

    /** 
     * Get the number of elements the GPU worker requests
     *  
     * 
     * @return pair<min, max>
     */
    virtual std::pair<unsigned, unsigned> number_elements() const = 0;

    /** 
     * Applies an optimization strategy element
     * 
     * @param env current optimization environment
     * @param boxes list with boxes
     * 
     */
    virtual void apply(opt_worker_state_t &env, gpu_box_lst &boxes) const = 0;

    virtual std::string name() const 
      {
	return "name() not implemented!";
      }
  };
}

#endif /*__STRATELE_HPP__*/
