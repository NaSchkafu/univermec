#include <algorithm/opt-lib/defaultstrategy.hpp>

#include <boost/foreach.hpp>

// Standard Elemente
#include "algorithm/opt-lib/strategies/bounder.hpp"
#include "algorithm/opt-lib/strategies/uubound.hpp"
#include "algorithm/opt-lib/strategies/feas.hpp"
#include "algorithm/opt-lib/strategies/midpoint.hpp"
#include "algorithm/opt-lib/strategies/bcubound.hpp"
#include "algorithm/opt-lib/strategies/bcconstraints.hpp"
#include "algorithm/opt-lib/strategies/iliebounder.hpp"
#include "algorithm/opt-lib/strategies/ilieconstraints.hpp"
#include "algorithm/opt-lib/strategies/bcgrad.hpp"

#include "decomp/ratzsection.hpp"
#include "decomp/weightmw.hpp"
#include "decomp/weightratz92.hpp"
#include "decomp/midchooser.hpp"

#include "decomp/iboxlistmgr.hpp"
//#include "decomp/sortedlistmgr.hpp"
#include "decomp/queuemgr.hpp"

#include <core/cxsccomp.hpp>

namespace GlobOpt 
{
  struct BestFirstCmp {    
    bool operator()(const OptBox* b1, const OptBox* b2) {
      return sup(b1->bound()) < sup(b2->bound());
    }
  };

  DefaultStrategy::DefaultStrategy(const core::arith::mreal &eps_f, const core::arith::mreal &eps_x, 
				   const core::arith::mreal &eps_viol)
  {
#pragma omp master 
    {
      S_EPS_F = eps_f;
      S_EPS_X = eps_x;
      S_EPS_VIOL = eps_viol;
    }
  }

  IOptStrategy::phase_config_t DefaultStrategy::first_phase_(opt_worker_state_t &state)
  {
    // Split
    m_config.psplit.push_back(new bounder_t); // Bounder

    // A
    m_config.pa.push_back(new mid_point_t); // Midpoint
    m_config.pa.push_back(new feas_t); // Update Feas & Discard Infeas
    
    // Possibly Infeas
    m_config.pinfeas.push_back(new ilie_cbounder_t); // ILIE Constraints Bounder
    m_config.pinfeas.push_back(new bc_const_t); // Box Consistency
    
    // B
    //m_config.pb.push_back(new u_ubound_t); // Update ubound if possible
    //m_config.pb.push_back(new mid_point_t); // Midpoint

    // Feas
    m_config.pfeas.push_back(new u_ubound_t);

    // C 
    m_config.pc.push_back(new ilie_bounder_t); //TODO FIXME
    m_config.pc.push_back(new bc_ubound_t);


    // StrictFeas
    // MidPoint Gradient
    // TODO REMOVE
    m_config.psfeas.push_back(new bc_grad_t);


    // D
    // TODO Remove
    //m_config.pd.push_back(new ilie_bounder_t);

    // Newton ubound
    // Linearisierung

    decomp::IDirWeight<OptBox> *dir_weight = new decomp::WeightRatz92<OptBox>(*state.obj);
    //decomp::IDirWeight<OptBox> *dir_weight = new decomp::WeightMW<OptBox>;
    decomp::IPointChooser<OptBox> *point_chooser = new decomp::MidChooser<OptBox>;
    // RatzSection, keine Subdivision für Teilintervalle < 10e-10
    m_config.section = new decomp::RatzSection<OptBox>(dir_weight, point_chooser, 10e-10);
    m_config.max_it = 0;
    m_config.min_width = wid(state.cbox->ref_X())/2.0;

    // lokaler bounder
    m_l_obj = std::unique_ptr<functions::IFunction>(state.org_obj.clone());
    state.obj = m_l_obj.get();

    // tmp strats
    //
    //m_config.tmp_strats.push_back(new vericell_bounder_t);
    //m_config.tmp_strats.push_back(new u_ubound_vericell_t); // Update ubound if possible
    m_config.tmp_strats.push_back(new mid_point_t);

    // criterion
    m_config.term_crit = &DefaultStrategy::progress;
    return m_config;
  }

  IOptStrategy::global_config_t DefaultStrategy::global_config_(opt_worker_state_t &)
  {
    // BoxManager
    m_g_config.bmgr = new decomp::QueueMgr<OptBox>;
    m_g_config.pre_split = 0;
    return m_g_config;
  }

  IOptStrategy::phase_config_t DefaultStrategy::next_phase_(opt_worker_state_t &state, unsigned ,
							     unsigned , unsigned )
  {
    if(state.ubound.hasPoint() && m_config.pa.size() == 1) {
      m_config.pa.push_back(new mid_point_t);
      //m_config.sol_strats.push_back(new vericell_aaf_bounder_t);
    }
    //if(m_config.min_width < 1.5 && m_config.pd.empty())
    // m_config.pd.push_back(new vericell_aaf_bounder_t);
    //m_config.pinfeas.push_back(new ilie_cbounder_t);
    if(m_config.min_width > 10e-3)
      m_config.min_width *= 0.5;
    else {
      // if(m_config.tmp_strats.empty())
      // 	m_config.tmp_strats.push_back(new vericell_aaf_bounder_t);
      m_config.min_width *= 0.3;
    }
    
    // if(m_config.pinfeas.empty() && m_config.min_width < 0.5) {
    //   // Possibly infeas
    //   m_config.pinfeas.push_back(new ilie_cbounder_t);
    //   m_config.pinfeas.push_back(new bc_const_t);     
    // }
    // if(m_config.pd.empty() && m_config.min_width < 0.3)
    //   m_config.pd.push_back(new ilie_bounder_t);
    
#pragma omp master
    {
      std::cout << "Nächste Phase in Strategy min_width: " << m_config.min_width << std::endl;
    }
    return m_config;
  }

  IOptStrategy::phase_config_t DefaultStrategy::final_phase_(opt_worker_state_t &state, unsigned tmps,
					    unsigned sols, unsigned its)
  {
    m_config.sol_strats.push_back(new mid_point_t);
    m_config.tmp_strats.clear();
    m_config.tmp_strats.push_back(new mid_point_t);
    return m_config;
  }

  TERM_RESULT DefaultStrategy::progress(const opt_worker_state_t &state)
  {
    if(wid(state.cbox->X()) <= S_EPS_X || (
       wid(state.cbox->bound()) <= S_EPS_F && 
       (state.cbox->feasible() || 
	(state.cbox->getFeas() == FEAS_UNKNOWN &&
	 state.cbox->max_viol() <= S_EPS_VIOL))))
      return SOLUTION;

    core::arith::mreal maxRed(0.0);
    for(int i = 0; i < Ub(state.cbox->ref_X()); i++)
      maxRed = std::max(maxRed, core::arith::wid(state.cbox->ref_X()[i]) - 
			core::arith::wid(state.cbox->old()[i]));
    if(0.25*wid(state.cbox->old()) - maxRed <= 0)
      return REDUCED;
    return NONE;
  }

  DefaultStrategy::~DefaultStrategy()
  {
    delete m_config.section;
    delete m_g_config.bmgr;
    BOOST_FOREACH(const strat_ele_t *ele, m_config.pa)
      delete const_cast<strat_ele_t*>(ele);
    BOOST_FOREACH(const strat_ele_t *ele, m_config.pb)
      delete const_cast<strat_ele_t*>(ele);
    BOOST_FOREACH(const strat_ele_t *ele, m_config.pc)
      delete const_cast<strat_ele_t*>(ele);
    BOOST_FOREACH(const strat_ele_t *ele, m_config.pd)
      delete const_cast<strat_ele_t*>(ele);
    BOOST_FOREACH(const strat_ele_t *ele, m_config.psplit)
      delete const_cast<strat_ele_t*>(ele);
    BOOST_FOREACH(const strat_ele_t *ele, m_config.pinfeas)
      delete const_cast<strat_ele_t*>(ele);
    BOOST_FOREACH(const strat_ele_t *ele, m_config.pfeas)
      delete const_cast<strat_ele_t*>(ele);
    BOOST_FOREACH(const strat_ele_t *ele, m_config.psfeas)
      delete const_cast<strat_ele_t*>(ele);
    BOOST_FOREACH(const strat_ele_t *ele, m_config.sol_strats)
      delete const_cast<strat_ele_t*>(ele);
    BOOST_FOREACH(const strat_ele_t *ele, m_config.tmp_strats)
      delete const_cast<strat_ele_t*>(ele);
  }

  IOptStrategy* DefaultStrategy::clone_() const
  {
    // TODO: besser eine richtige deep copy erstellen
    DefaultStrategy *strat = new DefaultStrategy(S_EPS_X, S_EPS_F, S_EPS_VIOL);
    strat->m_g_config.bmgr = 0;
    return strat;
  }

  core::arith::mreal DefaultStrategy::S_EPS_F = 60;
  core::arith::mreal DefaultStrategy::S_EPS_X = 10e-5;
  core::arith::mreal DefaultStrategy::S_EPS_VIOL = 10e-5;

  //     // TODO.: const correctness verletzt an dieser Stelle
//     // erfordert einen Umbau der Klasse Box, bzw der Verwaltung der NBs der Klasse Box
//     // evtl. einen eigenen (const) Iteratortyp oder �hnliches in Box f�r die Referenzen
//     // auf die aktuellen NBs einbauen
//     if(!m_critFlag) {
//       // 9a
//       if(GC::Instance().checkBox(box.X()) &&
//          GC::Instance().checkFunction(box.X(), obj)) {
//         lst.push_back(&box);
// #pragma omp atomic
//         m_finalCounter++;
//         return ADDED_BOX;
//       }
//     }
//     // 9b
//     else {
//       bool violated = false;
//       OptBox::MetaList::const_iterator fItEnd(box.mConstraints().end());
//       for(OptBox::MetaList::const_iterator fIt(box.mConstraints().begin());
//           fIt != fItEnd && !violated; ++fIt) {
//         if(!GC::Instance().checkInEqConstraint(box.X() , fIt->constraint()))
//           violated = true;
//       }

//       if(!violated) {
//      box.updateOld(box.ref_X());
//      box.ref_bound() &= obj(box.ref_X());
//         lst.push_back(&box);
// #pragma omp atomic
//         m_finalCounter++;
//         return ADDED_BOX;
//       }
//     }
//     // 9c
//     if(GC::Instance().hasReduced(box.X(), box.old())) {
// #pragma omp critical(BOXMGR)
//       {
//      box.updateOld(box.ref_X());
//         m_bMgr->add(&box);
//       }
//       return REDUCED_BOX;
//     }
//     return NOTHING;
}
