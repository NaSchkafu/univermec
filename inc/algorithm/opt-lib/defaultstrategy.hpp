#ifndef __DEFAULTSTRATEGY_HPP__
#define __DEFAULTSTRATEGY_HPP__

#include "algorithm/opt-lib/ioptstrategy.hpp"

//#include "extras/vericell/vericell_helper.hpp"

namespace GlobOpt
{
  /// Strategy class for solving the VeriCell problem
  class DefaultStrategy : public IOptStrategy
  {
  public:
    /** 
     * Ctor
     * 
     * @param type type of model
     * @param eps_f maximum width of objective functione val
     * @param eps_x minimum subdivision width
     * @param eps_viol maximum violation of a constraint
     * @param use_gpu Use GPU acceleration if available
     *
     * 
     * NOTE: The parameters eps_f, eps_x and eps_viol are global for
     * all instance of DefaultStrategy. 
     */
    DefaultStrategy(const core::arith::mreal &eps_f, const core::arith::mreal &eps_x, 
		     const core::arith::mreal &eps_viol);
    static TERM_RESULT progress(const opt_worker_state_t &state);
    virtual ~DefaultStrategy();

  private:
    virtual IOptStrategy::phase_config_t next_phase_(opt_worker_state_t &state, unsigned tmps,
                                       unsigned sols, unsigned its);
    virtual IOptStrategy::phase_config_t final_phase_(opt_worker_state_t &state, unsigned tmps,
                                       unsigned sols, unsigned its);
    virtual IOptStrategy::phase_config_t first_phase_(opt_worker_state_t &state);
    virtual IOptStrategy::global_config_t global_config_(opt_worker_state_t &state);

    virtual IOptStrategy* clone_() const;

  private:
    IOptStrategy::phase_config_t m_config;
    IOptStrategy::global_config_t m_g_config;
    std::unique_ptr<functions::IFunction> m_l_obj;

    static core::arith::mreal S_EPS_F;
    static core::arith::mreal S_EPS_X;
    static core::arith::mreal S_EPS_VIOL;
  };
}

#endif /*__VERICELLSTRATEGY_HPP__*/
