/*
 * @file   globoptalgo.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:19:19 2009
 *
 * @brief  Impl. of global optimization algorithm
 *
 *
 */
#include <algorithm/opt-lib/globoptalgo.hpp>

#include <iostream>
#include <forward_list>
#include <queue>
// TODO C11
#include <boost/foreach.hpp>
#include <boost/dynamic_bitset.hpp>

#include "functions/ifunction.hpp"
#include "functions/iffeatures.hpp"
#include "functions/iderivative.hpp"

#include <algorithm/opt-lib/globalcriteria.hpp>
#include <algorithm/opt-lib/general.hpp>
#include <algorithm/opt-lib/helper.hpp>

// Access to Boxlist Manager
#include "decomp/iboxlistmgr.hpp"
#include "decomp/imultisection.hpp"

// Access to strategy engine
#include <algorithm/opt-lib/ioptstrategy.hpp>
#include <algorithm/opt-lib/optenv.hpp>
#include <algorithm/opt-lib/stratele.hpp>
#include <algorithm/opt-lib/local_state.hpp>

// ipopt wrapper
//#include "tnlpwrapper.hpp"
//#include <coin/IpIpoptApplication.hpp>

// Beschleunigungstechniken
// #include "bcineq.hpp"
// #include "boxconsistency.hpp"
#include <algorithm/opt-lib/linbound.hpp>
#include <algorithm/opt-lib/linesearch.hpp>
#include <algorithm/opt-lib/checkjohn.hpp>
#include <algorithm/opt-lib/newtonstep.hpp>

#include "algorithm/utils/upperbound.hpp"

//#include "cachefunction.hpp"

#define DEBUG_OUTPUT

#ifdef _OPENMP
#include <omp.h>
#endif

using namespace std;
using namespace core::arith;
using namespace functions;

namespace GlobOpt {

  template <typename T>
  unsigned fwd_size(const std::forward_list<T> &list) 
  {
    return distance(list.begin(), list.end());
  }

  struct global_state_t
  {
    global_state_t()
      :iterates(0)
      {}

    unsigned iterates;
    // Globale Upperbound
   ::algorithm::utils::UpperBound ubound;
    /// Global configuration
    IOptStrategy::global_config_t config;
  };

  // Schritt 9 Hansen
#define CHECK_CRIT(x) if(termCriterion((x), uBound(), finalLst, obj) != NOTHING) continue;


  GlobOptAlgo::GlobOptAlgo(const IFunction &objF, const FLst& constraints,
                           IOptStrategy &strat, unsigned int maxIt)
    :m_obj(objF), m_constraints(constraints), m_result(m_finalLst),
     m_epsilonNwt(2*max(GC::Instance().epsX(), GC::Instance().epsF())), m_finalCounter(0),
     m_maxIt(maxIt), m_strategy(strat)
  {
#ifdef DEBUG_OUTPUT
    cout << "****************************************************************" << endl;
    cout << "#NBs " << m_constraints.size() << endl;
    BOOST_FOREACH(const IFunction &f, m_constraints) {
      cout << f.text() << endl;
      // for(unsigned int i = 1; i <= f.dim(); i++)
      //        cout << f.gradF(i) << endl;
      cout << "****************************************************************" << endl;
    }
#endif
    assert(objF.features().derivative() && objF.features().derivative()->grad(1).features().derivative()
           && "Second order informations required!");

  }

  GlobOptAlgo::~GlobOptAlgo()
  {}

  /*!
  ** Starts the global optimization algorithm
  **
  ** @param startBox initial box for the search
  */

  void GlobOptAlgo::start(const ivector &startBox, const rvector &intPoint)
  {
    OptBox::MetaList cList;
    interval lagMult(0,1);
    core::arith::mreal lagMultNwt( 1.0/(m_constraints.size() + 1));
    for(unsigned int i = 0; i < m_constraints.size(); i++) {
      CBoxMeta meta(m_constraints[i], lagMult, lagMultNwt);
      cList.push_back(meta);
    }
    m_initBox = std::unique_ptr<OptBox>(new OptBox(startBox, lagMult, lagMultNwt, cList));
    m_intPoint = intPoint;

    m_alg_status = RUNNING;

    // start
    work();
  }

  /**
   * Applies all strategies in a strategy list
   *
   * @param lst list of strategies
   * @param env local environment
   *
   * @return true if no further processing is required
   */
  TERM_RESULT GlobOptAlgo::apply_strats(const IOptStrategy::strat_ele_lst_t &lst,
                                        opt_worker_state_t &state,
                                        local_opt_env_t &env,
                                        IOptStrategy::phase_config_t &config)
  {
    TERM_RESULT res(NONE);
    auto sit_end(lst.end());
    for(auto sit(lst.begin()); !res && sit != sit_end; ++sit) {
      res = (*sit)->apply(state) ? DISCARDED : (*config.term_crit)(state);

      // std::cout << "Status für " << (*sit)->name() << ": " << res << std::endl;
      switch(res) {
      case NONE:
        // Box wird temporär herausgenommen
        break;
      case REDUCED:
        env.no_split_lst.push_back(state.cbox);
        break;
      case SOLUTION:
        env.final_lst.push_back(state.cbox);
        break;
      case DISCARDED:
        state.m_discards[(**sit).name()]++;
        // std::cout << "DEL @" << state.cbox << std::endl;
        delete state.cbox;
        break;
      default:
        assert(!"Unknown state in apply_strats()!");
        break;
      }
    }

    if(res)
      state.cbox = 0;

    return res;
  }

  // GPU Strategien
  TERM_RESULT GlobOptAlgo::apply_strats(const IOptStrategy::gpu_strat_ele_lst_t &lst,
                                        opt_worker_state_t &state,
                                        IOptStrategy::phase_config_t &lphase,
                                        local_opt_env_t &env,
                                        global_state_t &global_state)
  {
#pragma omp master
    {
      OptBox* old_cbox = state.cbox;
      std::deque<std::pair<OptBox*, bool> > boxes;

      auto sit_end(lst.end());
      for(auto sit(lst.begin()); sit != sit_end; ++sit) {
        // Boxen holen und neue per split generieren
        unsigned min_ele = (*sit)->number_elements().first;
        unsigned max_ele = (*sit)->number_elements().second;
        if(boxes.size() < min_ele)
          request_boxes(state, lphase, env, global_state, (max_ele - boxes.size())/3, &boxes);
	//std::cout << "boxes.size()" << boxes.size() << std::endl;
	//std::cout << boxes.fraont().first->ref_X() << " w(): " << w(boxes.front().first->ref_X()) << std::endl;
        if(!boxes.empty()) {
          // Passende Anzahl von Boxen für die GPU generieren
          while(boxes.size() < max_ele) {
            // Split
            OptBox* to_split = boxes.front().first;
            auto new_boxes(lphase.section->section(*to_split));
            for_each(new_boxes.begin(), new_boxes.end(), [&](OptBox *b){
                // TODO: neue Split Phase integrieren
                //std::cout << "Aktuelle Split Box " << b << std::endl;
                // Referenz Box löschen
                b->reset_old();
                // state.cbox = b;
                // if(!apply_strats(lphase.psplit, state, local_env, lphase))
                //   local_env.no_split_lst.push_back(state.cbox);
                boxes.push_back(make_pair(b, false));
              });
            delete to_split;
            boxes.pop_front();
          }
	  //std::cout << "Size Boxes: " << boxes.size() << std::endl;

	  // Ueberfluessige Boxen zurueck kopieren
          while(boxes.size() > max_ele) {
	    state.cbox = boxes.back().first;
	    // Wichtig: Falls w(state.cbox) < Splitting threshold wird
	    // nicht mehr gesplittet, daher ueberfluessige Boxen
	    // finalisieren falls sie das Abbruchkriterium erfuellen!
	    if((*lphase.term_crit)(state) == SOLUTION)
	      env.final_lst.push_back(state.cbox);
	    else
	      env.no_split_lst.push_back(state.cbox);
            boxes.pop_back();
          }

          (*sit)->apply(state, boxes);
	  // for(auto bit(boxes.begin()); bit != boxes.end(); ++bit)
	  //   std::cout << bit->first << std::endl;

	  //std::cout << "#BOXES: " << boxes.size() << std::endl;
	  auto bit(boxes.begin());
	  while(bit != boxes.end()) {
            TERM_RESULT res(NONE);
            state.cbox = bit->first;
            res = bit->second ? DISCARDED : (*lphase.term_crit)(state);
            switch(res) {
            case NONE:
	      //std::cout << "NONE" << std::endl;
              break;
            case REDUCED:
	      //std::cout << "RED" << std::endl;
              env.no_split_lst.push_back(state.cbox);
              break;
            case SOLUTION:
	      //std::cout << "SOL " << std::endl;
	      env.final_lst.push_back(state.cbox);
              break;
            case DISCARDED:
              state.m_discards[(**sit).name()]++;
              //std::cout << "DEL @" << state.cbox << std::endl;
              delete state.cbox;
              break;
            default:
              assert(!"Unknown state in apply_strats()!");
              break;
            }
            if(res != NONE) {
              bit = boxes.erase(bit);
	    }
	    else
	      ++bit;
          }
        }
        else // Keine Boxen verfügbar, in Hauptschleife wechseln
          break;
      }

      for_each(boxes.begin(), boxes.end(), [&env](std::pair<OptBox*, bool> &b){ env.no_split_lst.push_back(b.first); });
      state.cbox = old_cbox;
    }
    return NONE;
  }


  void GlobOptAlgo::request_boxes(opt_worker_state_t &state,
                                  IOptStrategy::phase_config_t &lphase,
                                  local_opt_env_t &local_env,
                                  global_state_t &global_state,
                                  unsigned number,
                                  gpu_strat_ele_t::gpu_box_lst *ret)
  {
#pragma omp critical(BOXMGR)
    {
      //std::cout << "!!!!!! #Count " << global_state.config.bmgr->count() << std::endl;
      // Max It überschritte, BoxMgr leeren und auf die lokale Liste kopieren
      // TODO: last auf threads verteilen!
      if((lphase.max_it && global_state.iterates > lphase.max_it)) {
        std::copy(local_env.no_split_lst.begin(), local_env.no_split_lst.end(),
                  front_inserter(local_env.tmp_lst));
        const decomp::IBoxListMgr<OptBox>::box_lst_t &lst = global_state.config.bmgr->list();
        std::copy(lst.begin(), lst.end(), front_inserter(local_env.tmp_lst));
        global_state.config.bmgr->clear();
        // BOOST_FOREACH(OptBox *b, local_env.tmp_lst){
        //   std::cout << "Box " << (*b).X() << " @" << b << std::endl;
        // }
      }
      else
        global_state.config.bmgr->add(local_env.no_split_lst);
      local_env.no_split_lst.clear();
      // Anzahl der Boxen
      unsigned lbcount = global_state.config.bmgr->count();
      if(lbcount)
        if(number == 1)
          state.cbox = global_state.config.bmgr->select();
        else
          for(unsigned i = 1; i <= min(number, std::max(1u, lbcount/2)); i++)
            ret->push_back(make_pair(global_state.config.bmgr->select(), false));
      //std::cout << "Aktiviere Box @" << state.cbox << std::endl;
      global_state.ubound.sync(state.ubound);
    }

  }

  void GlobOptAlgo::work()
  {
    //unsigned int gapsCount = 0;
    GlobOptStatistic globalStats;

    global_state_t global_state;

    // keine EQ Constraints
    FLst eq_constraints;

    // Globale Daten
    unsigned next_its = 0;
    // Globaler Einschluss
    core::arith::interval gencl(core::arith::infinity(), -core::arith::infinity());
    // Globales Flag ob Upper Bound in Lsg Box ist
    bool gub_inside = false;

    // Initialisierung der Strategie
    opt_worker_state_t tmp_state(m_obj, m_constraints, eq_constraints);
    tmp_state.cbox = m_initBox.get();
    global_state.config = m_strategy.global_config(tmp_state);
    IOptStrategy::phase_config_t gphase(m_strategy.first_phase(tmp_state));
    std::queue<OptBox*> pre_boxes;
    pre_boxes.push(new OptBox(*tmp_state.cbox, true));
    for(unsigned i = 0; i < global_state.config.pre_split; i++) {
      OptBox *next = pre_boxes.front();
      auto sboxes(gphase.section->section(*next));
      BOOST_FOREACH(OptBox *b, sboxes)
        pre_boxes.push(b);
      pre_boxes.pop();
      delete next;
    }
    while(!pre_boxes.empty()) {
      global_state.config.bmgr->add(pre_boxes.front());
      pre_boxes.pop();
    }


    boost::dynamic_bitset<> active_threads;
    unsigned no_threads = 1;
    enum THREAD_STATUS { T_PENDING, T_NEW_BOXES, T_NEXT_PHASE  } lt_status;

    // Threads
#pragma omp parallel default(none) shared(global_state, eq_constraints, no_threads, active_threads, next_its, gub_inside, std::cout, gencl)
    {
      // Worker Status
      opt_worker_state_t state(m_obj, m_constraints, eq_constraints);
      state.cbox = m_initBox.get();
      local_opt_env_t local_env;
      //unsigned lbcount = 0;
      THREAD_STATUS lt_status;
      unsigned thread_no = 0;

      // Strategie für jeden Worker kopieren
#pragma omp critical(STRATEGY)
      {
        local_env.strategy = std::unique_ptr<IOptStrategy>(m_strategy.clone());
      }
      IOptStrategy::phase_config_t lphase(local_env.strategy->first_phase(state));

#pragma omp single
      {
        m_alg_status = RUNNING;
#ifdef _OPENMP
        no_threads = omp_get_num_threads();
        std::cout << no_threads << " aktiv!" << std::endl;
#endif
        active_threads.resize(no_threads, true);
      }
#ifdef _OPENMP
      thread_no = omp_get_thread_num();
#endif
      //std::cout << "Thread " << thread_no << " betritt Hauptschleife!" << std::endl;
      // Lokale Schleife
      while(m_alg_status == RUNNING) {
#pragma omp atomic
        ++global_state.iterates;

// Maximale Iterationsanzahl
#pragma omp master
        {
          if(m_maxIt && global_state.iterates > m_maxIt) {
            std::cout << "Maximale Iterationsanzahl überschritten!" << std::endl;
            m_alg_status = MAX_IT_EXCEEDED;
          }
        }

        // if(iterates > 120)
        //   exit(-1);

        // std::cout << "Boxen no_split ";
        // BOOST_FOREACH(OptBox *b, local_env.no_split_lst) {
        //   std::cout << "@" << b << " ";
        // }
        // std::cout << std::endl;
        // std::cout << "to_add @" << local_env.to_add << std::endl;

        // Splitting
        if(local_env.to_add) {
          auto boxes(lphase.section->section(*local_env.to_add));
          for_each(boxes.begin(), boxes.end(), [&](OptBox *b){
              //std::cout << "Aktuelle Split Box " << b << std::endl;
              // Referenz Box löschen
              b->reset_old();
              state.cbox = b;
              if(!apply_strats(lphase.psplit, state, local_env, lphase)) {
		//std::cout << "NONE" << std::endl;
                local_env.no_split_lst.push_back(state.cbox);
	      }
            });
          //std::cout << "DEL " << local_env.to_add << std::endl;
          delete local_env.to_add;
          local_env.to_add = 0;
        }

	if(apply_strats(lphase.gpu_psplit, state, lphase, local_env, global_state))
	  continue;

        // Keine Aktive Box vorhanden
        state.cbox = 0;
        request_boxes(state, lphase, local_env, global_state);

        // Keine Box gefunden
        if(!state.cbox) {
#pragma omp critical(ACTIVETHREADS)
          {
            std::cout << "Thread " << thread_no
		      << " blockiert und wartet auf neue Boxen." << std::endl;
            active_threads.reset(thread_no);
          }
          lt_status = T_PENDING;
          // Busy Waiting
          while(lt_status == T_PENDING) {
#pragma omp critical(BOXMGR)
            {
              if(global_state.config.bmgr->count())
                lt_status = T_NEW_BOXES;
            }
#pragma omp critical(ACTIVETHREADS)
            {
              if(active_threads.none())
                lt_status = T_NEXT_PHASE;
            }
          } // Ende Busy Waiting
          if(lt_status == T_NEW_BOXES) {
#pragma omp critical(ACTIVETHREADS)
            {
              active_threads.set(thread_no);
            }
#ifdef DEBUG_OUTPUT
            //cout << "Thread " << thread_no  << " reaktiviert!" << endl;
#endif
            continue;
          }
          /* Es droht Deadlock-Gefahr wenn der alg_status nicht mehr auf RUNNING steht
           * weil z.B. die maximale Iterationszahl aufgebraucht wurde.
           * In diesem Fall könnte bereits ein Thread die Hauptschleife verlassen haben und die
           * Barriere daher nie erreichen. Daher erzwungenes rausspringen aus der
           * Hauptschleife.
           */
          if(m_alg_status != RUNNING)
            continue;

          // ok alle Threads sind angehalten wg. Boxmangel
          // Strategie Update
          // Einleiten der neuen Phase
          lphase = local_env.strategy->next_phase(state, fwd_size(local_env.tmp_lst),
                                                  local_env.final_lst.size(), global_state.iterates);
          // Starten der Endbearbeitungen
#pragma omp master
          {
            std::cout << "Nächste Phase bei Its " << global_state.iterates << std::endl;
          }

          // BOOST_FOREACH(OptBox *b, local_env.tmp_lst) {
          //   std::cout << (*b).X() << " @" << b << std::endl;
          // }
          // std::cout << "FERTIG" << std::endl;

          // Einmalige Elemente auf finallst anwenden
          // auto rem_it(remove_if([&lphase, &state, &local_env, this](OptBox *b) {
	  // 	state.cbox = b;
	  // 	return apply_strats(lphase.sol_strats, state, local_env, lphase);}));

          // BOOST_FOREACH(OptBox *b, local_env.tmp_lst) {
          //   std::cout << (*b).X() << " @" << b << std::endl;
          // }
          // std::cout << "FERTIG" << std::endl;

          // Einmalige Strategien auf temporary list anwenden
          for_each(local_env.tmp_lst.begin(), local_env.tmp_lst.end(), [&](OptBox *b) {
              state.cbox = b;
              if(!apply_strats(lphase.tmp_strats, state, local_env, lphase))
                local_env.no_split_lst.push_back(b); });
          local_env.tmp_lst.clear();

          // BOOST_FOREACH(OptBox *b, local_env.tmp_lst) {
          //   std::cout << (*b).X() << " @" << b << std::endl;
          // }
          // std::cout << "FERTIG" << std::endl;

#pragma omp critical(BOXMGR)
          {
            global_state.config.bmgr->add(local_env.no_split_lst);
            local_env.no_split_lst.clear();
          }
#pragma omp barrier
#pragma omp single
          {
            if(global_state.config.bmgr->count())
              active_threads.set(); // Alle Threads reaktivieren
            else
              m_alg_status = FINISHED;
          } // Implizite Barriere
          continue;
        }

        // HACK für VeriCell Obj
        // bei nicht beschränkter Box SubDiv
        if(!(sup(state.cbox->bound()) < core::arith::infinity())) {
          local_env.to_add = state.cbox;
          continue;
        }
#pragma omp master
        {
          //if(global_state.iterates > next_its) {

	    //}
        }

        // Phase A
        if(apply_strats(lphase.pa, state, local_env, lphase))
          continue;

        // Box nicht sicher zulässig
        // Phase NonFeas
        if(!state.cbox->feasible()) {
          if(apply_strats(lphase.pinfeas, state, local_env, lphase))
            continue;
        }

        // Phase B
        if(apply_strats(lphase.pb, state, local_env, lphase))
          continue;

        // Phase Feas
        if(state.cbox->feasible()) {
          if(apply_strats(lphase.pfeas, state, local_env, lphase))
            continue;
        }

        // Phase C
        if(apply_strats(lphase.pc, state, local_env, lphase))
          continue;

        // Phase Strictly Feas
        if(state.cbox->strictly_feasible()) {
          if(apply_strats(lphase.pfeas, state, local_env, lphase))
            continue;
        }

        // Phase D
        if(apply_strats(lphase.pd, state, local_env, lphase))
          continue;

        if(wid(state.cbox->ref_X()) < lphase.min_width) {
          local_env.tmp_lst.push_front(state.cbox);
          // std::cout << "TMP LST " << local_env.tmp_lst.size() << " Added @" << state.cbox
          //        << state.cbox->bound() << std::endl;
        }
        else
          local_env.to_add = state.cbox;

        // std::cout << "TMP START" << std::endl;
        // BOOST_FOREACH(OptBox *b, local_env.tmp_lst) {
        //   std::cout << (*b).X() << " @" << b << std::endl;
        // }
        // std::cout << "FERTIG" << std::endl;

#pragma omp master 
	{
	  if(!(global_state.iterates % 10)) {
	    std::cout << "****************************************************************" << std::endl;
	    BOOST_FOREACH(auto it, state.m_discards) {
	      std::cout << it.first << " #DISCA" << it.second << std::endl;
	    }
	    std::cout << "GES #BOXES: " << global_state.config.bmgr->count() << std::endl;
	    std::cout << "LOC #FINAL: " << local_env.final_lst.size() << std::endl;
	    std::cout << "LOC #TMPLI: " << fwd_size(local_env.tmp_lst) << std::endl;
	    std::cout << "LOC UBOUND: " << state.ubound() << std::endl;
	    std::cout << "GES ITERAT: " << global_state.iterates << std::endl;
	    std::cout << "WID CURRBO: " << wid(state.cbox->X()) << std::endl;
	    std::cout << "CURBOX    : " << state.cbox->X() << std::endl;
	  }
	}
	
      } // Hauptschleife Ende
// Alle Threads verlassen die Hauptschleife
#pragma omp critical(ACTIVETHREADS)
      {
        active_threads.reset();
      }

#pragma omp critical(INFO)
      {
        std::cout << "****************************************************************" << std::endl;
        std::cout << "Thread " << thread_no << " hat Hauptschleife verlassen." << std::endl;
//      std::cout << "Box: " << state.cbox->ref_X() << " @" << state.cbox << std::endl;
//      std::cout << "w(Box): " << w(state.cbox->ref_X()) << std::endl;
//      std::cout << "Zielfunktion: " << state.cbox->bound() << std::endl;
        std::cout << "Aktive Boxen: " << global_state.config.bmgr->count() << std::endl;
        std::cout << "Final: " << local_env.final_lst.size() << std::endl;
        std::cout << "TMP: " << fwd_size(local_env.tmp_lst) << std::endl;
        std::cout << "No Split: " << local_env.no_split_lst.size() << std::endl;
        std::cout << "To Add: " << local_env.no_split_lst.size() << std::endl;
        std::cout << "Its: " << global_state.iterates << std::endl;
        std::cout << "Min_Width: " << lphase.min_width << std::endl;
        std::cout << "Upper bound " << state.ubound() << std::endl;
        for(auto it(state.m_discards.begin()); it != state.m_discards.end(); ++it) {
          std::cout << "Discard " << it->first << ": " << it->second << std::endl;
        }
        std::cout << "****************************************************************" << std::endl;
      }
#pragma omp barrier

// Upper Bounds synchen
#pragma omp critical(UB)
      {
        global_state.ubound.sync(state.ubound);
      }

// Falls MGR nicht leer, alles rüberkopieren
// TODO: optimieren und last auf threads verteilen
#pragma omp master
      {
        if(global_state.config.bmgr->count()) {
          const decomp::IBoxListMgr<OptBox>::box_lst_t &lst = global_state.config.bmgr->list();
          std::copy(lst.begin(), lst.end(), front_inserter(local_env.tmp_lst));
        }
        global_state.config.bmgr->clear();
      }

      // Finale Phase holen
      lphase = local_env.strategy->final_phase(state, fwd_size(local_env.tmp_lst),
                                               local_env.final_lst.size(), global_state.iterates);

      unsigned current(fwd_size(local_env.tmp_lst));
      unsigned next = 0.9*current;
      // // Einmalige Strategien auf temporary list anwenden
      local_env.tmp_lst.remove_if([&state, &lphase, &current, &next, thread_no](OptBox *b) {
          current--;
          if(current < next) {
            std::cout << "Tmp List of thread " << thread_no << " " << current << " elements left!"
                      << std::endl;
            next *= 0.9;
          }
          state.cbox = b;
          for(auto it(lphase.tmp_strats.begin()); it != lphase.tmp_strats.end(); ++it)
            if((*it)->apply(state)) {
              delete b;
              return true;
            }
          return false;
        });

// Upper Bounds synchen
#pragma omp critical(UB)
      {
        global_state.ubound.sync(state.ubound);
      }
      current = local_env.final_lst.size();
      next = 0.9*current;
      // Einmalige Strategien auf die finale Liste anwenden
      auto rit(remove_if(local_env.final_lst.begin(), local_env.final_lst.end(), [&state, &lphase, &current, &next, thread_no](OptBox *b) {
          current--;
          if(current < next) {
            std::cout << "Tmp List of thread " << thread_no << " " << current << " elements left!"
                      << std::endl;
            next *= 0.9;
          }
          state.cbox = b;
          for(auto it(lphase.sol_strats.begin()); it != lphase.sol_strats.end(); ++it)
            if((*it)->apply(state)) {
              delete b;
              return true;
            }
          return false;
	  }));
	local_env.final_lst.erase(rit, local_env.final_lst.end());
	//local_env.final_lst.shrink_to_fit();

// Zweimal synchen damit der beste ubound über alle Threads propagiert wird
#pragma omp critical(UB)
      {
        global_state.ubound.sync(state.ubound);
      }
#pragma omp critical(UB)
      {
        global_state.ubound.sync(state.ubound);
      }
      // Listen zusammenfügen
      for_each(local_env.tmp_lst.begin(), local_env.tmp_lst.end(), [&local_env](OptBox *b) {
	  local_env.final_lst.push_back(b);
	});
      local_env.tmp_lst.clear();
      //local_env.final_lst.splice(local_env.final_lst.begin(), local_env.tmp_lst);


      // Minimum Bestimmen
      // 50
      core::arith::mreal minF(core::arith::infinity());
      core::arith::mreal maxF(-core::arith::infinity());
      bool ub_inside =  minInterval(minF, maxF, local_env.final_lst, state.ubound);
      bool has_boxes = !local_env.final_lst.empty();

#pragma omp critical(FINALLST)
      {
        // Liste kopieren
        for_each(local_env.final_lst.begin(), local_env.final_lst.end(),
		 [this](OptBox *b) {m_finalLst.push_back(b);});
	local_env.final_lst.clear();
        // Optimum einfügen
        if(has_boxes) {
	  gencl = core::arith::interval(min(inf(gencl), minF), max(sup(gencl), maxF));
          gub_inside |= ub_inside;
        }
      }
    } // Parallel Ende, Barriere

    // Update Einschluss mit Upper Bound
    gencl = core::arith::interval(inf(gencl), min(sup(gencl), global_state.ubound()));
 
    // Problemtyp festlegen
    if(global_state.ubound.hasPoint())
      m_result.problemType = PB_CERTAINLY_FEASIBLE;
    else if(!m_finalLst.size())
      m_result.problemType = PB_INFEASIBLE;
    else
      m_result.problemType = PB_POSSIBLY_FEASIBLE;

    if((m_result.problemType != PB_INFEASIBLE) && (m_alg_status != USER_ABORT)) {
      // Punkt speichern
      if(gub_inside) {
        m_result.point = global_state.ubound.point();
        m_result.pointType = RP_CERTAINLY_FEASIBLE;
      }
      else {
        m_result.point = mid(m_finalLst.front().ref_X());
        m_result.pointType = RP_EPSP_FEASIBLE;

        // evtl erg�nzen mit weiteren Checks
        BOOST_FOREACH(const OptBox &b, m_finalLst) {
          if(isFeasible(mid(b.ref_X()), m_constraints)) {
            m_result.point = mid(b.ref_X());
            m_result.pointType = RP_CERTAINLY_FEASIBLE;
            break;
          }
        }
      }
    }

    // Ergebnisintervall sichern
    m_result.minInterval = gencl;

    // uBound
    if(global_state.ubound.hasPoint()) {
      m_result.uPoint = global_state.ubound.point();
      m_result.uBound = global_state.ubound();
    }
    m_result.hasUPoint = global_state.ubound.hasPoint();

    m_result.statistic = globalStats;
    switch(m_alg_status) {
    case FINISHED:
      m_result.solverReturn = RE_SOLVED;
      break;
    case USER_ABORT:
      m_result.solverReturn = RE_USER_ABORT;
      break;
    case MAX_IT_EXCEEDED:
      m_result.solverReturn = RE_MAX_IT;
      break;
    default:
      m_result.solverReturn = RE_INTERNAL_ERROR;
    };
    m_result.iterations = global_state.iterates;

    // if(!m_critFlag)
    //  ubInside = false;
  }



//   bool GlobOptAlgo::doLineSearch(const OptBox &b, UpperBound &uBound, const IFunction &obj) const
//   {
//     // if(obj(mid(b.X())) > uBound())
//     //   return false;

//     // rvector ubPoint;
//     // if(lineSearch(b, m_initBox->X(), obj, ubPoint, m_query == PROXIMITY_QUERY))
//     //   return uBound.update(ubPoint);
//     // return false;
//   }

//   void GlobOptAlgo::measureRed(const OptBox &b, unsigned int &counter) const
//   {
// // #ifdef GLOBOPT_PERFMON
// //     if(MT::w(b.X()) < MT::w(b.lastX()))
// //       counter++;
// // #endif
//   }

//   bool GlobOptAlgo::bcUBound(OptBox &b, const core::arith::mreal &uBound) const
//   {
//     // TODO FIX

//     // bool empty = false;
//     // core::arith::interval right(m_query == PROXIMITY_QUERY ? 0 : -core::arith::infinity(), uBound);
//     // BCInEq bcF(m_F, right);
//     // b.update(applyBC(b.X(), bcF, empty));
//     // return empty;

//     return false;
//   }

  /*!
  ** Checks whether the criteria for progress has been matched
  ** (see Hansen 14.8 Step 9)
  ** @param box the box to check
  **
  ** @return ADDED_BOX if the box has been added to the m_finalLst List
  *          REDUCED_BOX if the box has been sufficiently reduced, the reduced will be added to
  *                      _boxes list
  *          NOTHING no criterion has been satiesfied
  */

  // GlobOptAlgo::TERM_RES GlobOptAlgo::termCriterion(OptBox &box, const core::arith::mreal &uBound,
//                                                 FinalLst &lst, const IFunction &obj)
//   {
//     // TODO: const correctness verletzt an dieser Stelle
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
//   }

  // bool GlobOptAlgo::hcConstraints(OptBox &box) const
  // {
  //   // interval iRight(-core::arith::infinity(), 0);
  //   // bool deleted = false;
  //   // BOOST_FOREACH(CBoxMeta &meta, box.mConstraints()) {
  //   //   IFunction &c = meta.constraint();
  //   //   if(c.supportsHC())
  //   //     box.update(c.hc().applyHC(box.X(), iRight, deleted));
  //   //   else {
  //   //     // Midpoint test
  //   //     deleted = inf(c(box.X())) > 0;
  //   //   }
  //   //   if(deleted)
  //   //     break;
  //   // }
  //   // return deleted;
  // }

//   bool GlobOptAlgo::bcConstraints(OptBox &box) const
//   {
//     // TODO: FIX
//     // Kein BC fuer Abstandsberechnung
// //     if(m_query == PROXIMITY_QUERY)
// //       return false;

//     bool empty = false;
//     interval right(-core::arith::infinity(), 0);

//     BOOST_FOREACH(CBoxMeta &meta, box.mConstraints()) {
//       IFunction &c = meta.constraint();
//       // BCInEq bcF(c, right);
//       // box.update(applyBC(box.X(), bcF, empty));
//       // if(empty)
//       //     break;
//     }
//     return empty;
//   }


//   bool GlobOptAlgo::hcUBound(OptBox &b, const core::arith::mreal &uBound, const IFunction &obj) const
//   {
//     bool deleted = false;
// /*    if(m_F.supportsHC()) {
//       interval iRight(-core::arith::infinity(), uBound);
//       const ivector &newBox = m_F.hc().applyHC(b.X(), iRight, deleted);
//       if(!deleted)
//         b.update(newBox);
//     }
//     else {
//       // kann auskommentiert werden, in diesem Fall f�hrt die
//       // n�chste Zeile einen normalen Midpointtest durch
//       // allerdings nicht wirklich effektiv!
//       //S_THROW("obj does not support hc!");
//       deleted = inf(m_F(b.X())) > uBound;
//     }
//     return deleted;
// */

//     deleted = std::min(inf(obj(b.X())), inf(enclosure_cast<interval>(obj(avector(b.X()))))) > uBound;
//     return deleted;
//   }

// bool GlobOptAlgo::doLinBound(OptBox &b, const core::arith::mreal &uBound, const IFunction &obj) const
//   {
//     return linBound(mid(b.X()), b.ref_X(), b.gaps(), obj, uBound);
//   }

  void GlobOptAlgo::registerObserver(GlobOptObserver& observer)
  {
    m_observers.push_back(boost::reference_wrapper<GlobOptObserver>(observer));
  }

  const GlobOptResult& GlobOptAlgo::result() const
  {
    return m_result;
  }

  // void GlobOptAlgo::findUB(UpperBound &ub) const
  // {
  //   if(!m_useIpOpt)
  //     return;

//     //
//     if(Ub(m_intPoint) - Lb(m_intPoint) + 1 != static_cast<int>(m_F.dim())) {
//       cout << "No valid starting point for IPOPT specified. Using middle of the box!" << endl;
//       mid(m_initBox.X());
//     }

//     if(MT::inside(m_intPoint, m_initBox.X()) && m_initBox.isFeasible(m_intPoint)) {
//       ub.update(m_intPoint);
//     }

//     Ipopt::SmartPtr<Ipopt::IpoptApplication> app = new Ipopt::IpoptApplication;
//     // nur versuchen einen guten Startwert f�r globalUB zu finden
//     app->Options()->SetNumericValue("tol", 1e-6);
//     app->Options()->SetIntegerValue("max_iter", 100);
// //    app->Options()->SetNumericValue("constr_viol_tol", 1e-20);
//     //app->Options()->SetStringValue("hessian_approximation", "limited-memory");
//     app->Options()->SetStringValue("derivative_test","second-order");
// //    app->Options()->SetStringValue("derivative_test_print_all","yes");


//     Ipopt::ApplicationReturnStatus status;
//     status = app->Initialize();
//     if(status != Ipopt::Solve_Succeeded) {
//       return;
//     }

//     TNLPWrapper *tnlpwrap = new TNLPWrapper(m_F, m_constraints, m_intPoint);
//     Ipopt::SmartPtr<Ipopt::TNLP> tnlp(tnlpwrap);
//     status = app->OptimizeTNLP(tnlp);
//     if(status == Ipopt::Solve_Succeeded) {
//       TNLPSolution sol(tnlpwrap->solution());
//       if(MT::inside(sol.x, m_initBox.X())) {
//      if(m_initBox.isFeasible(sol.x)) {
//        ub.update(sol.x);
// #ifdef DEBUG_OUTPUT
//        std::cout << "Ipopt hat Punkt gefunden. Neuer UB: " << ub() << std::endl;
// #endif
//      }
// // Dieser Teil kann als Basis dienen, falls die Toleranzwerte f�r IPOPT erhoeht werden
// /*
//      else {
//        core::arith::mreal eps(0.001);
//        std::cout << "Ipopt Punkt unzul�ssig!" << std::endl;
//        // erzeugen einer neuen Box
//        core::arith::ivector box(sol.x);
//        for(int i = 1; i <= Ub(box); i++) {
//          inf(box[i]) = inf(box[i]) - eps;
//          sup(box[i]) = sup(box[i]) + eps;
//        }
//        box &= m_initBox.X();

//        // erzeugen und feasibility status updaten
//        Box b(m_initBox);
//        b.update(box);
//        b.updateFeas();

//        for(int i = 0; i < 10; i++) {
//          if(hcConstraints(b))
//            break;
//          if(bcConstraints(b))
//            break;
//          if(b.isFeasible(mid(b.X()))) {
//            ub.update(mid(b.X()));
//            break;
//          }
//        }
//      }
// */
//       }
//     }
// #endif
  //   return;
  // }


  // bool GlobOptAlgo::strictlyFeasBox(OptBox &b, const functions::IFunction &obj) const
  // {
  //   return false;

  //   // verwerfen im Falle der Abstandsberechnung
  //   bool deleted = m_query == PROXIMITY_QUERY;

  //   // HC-Methode oder einfacher Mittelpunkttest
  //   interval iRight(0,0);
  //   for(unsigned int i = 1;i <= obj.dim() && !deleted; i++) {
  //     // if(m_F.gradF(i).supportsHC()) {
  //     //     const ivector &newBox = m_F.gradF(i).hc().applyHC(b.X(), iRight, deleted);
  //     //   if(!deleted)
  //     //     b.update(newBox);
  //     // }
  //     // else {
  //     deleted = !(0 <= obj.features().derivative()->grad(i)(b.X()));
  //     // }
  //   }

  //   // Newtonschritt auf g(x) = 0
  //   // if(!deleted) {
  //   //   FLst gradient;
  //   //   for(unsigned int i = 1; i <= m_F.dim(); i++) {
  //   //       gradient.push_back(boost::reference_wrapper<functions::IFunction>(m_F.features().derivative()->grad(i)));
  //   //   }
  //   //   rvector expPoint(mid(b.X()));
  //   //   b.update(newtonStep(b.X(), expPoint, gradient, deleted, b.gaps()));
  //   // }

  //   // nicht Konvexit�tstest
  //   if(!deleted) {
  //     iRight = interval(0, core::arith::infinity());
  //     for(unsigned int i = 1; i <= obj.dim() && !deleted; i++) {
  //       // if(m_F.gradF(i).gradF(i).supportsHC()) {
  //       //   const ivector &newBox = m_F.gradF(i).gradF(i).hc().applyHC(b.X(), iRight, deleted);
  //       //   if(!deleted)
  //       //     b.update(newBox);
  //       // }
  //       // else {
  //    deleted = sup(obj.features().derivative()->grad(i).
  //                  features().derivative()->grad(i)(b.X())) < 0;
  //       // }
  //     }
  //   }
  //   return deleted;
  // }
  bool GlobOptAlgo::minInterval(core::arith::mreal &minF, core::arith::mreal &maxF,
                                FinalLst &lst, const ::algorithm::utils::UpperBound &uBound)

  {
    unsigned int count = lst.size();
    std::cout << "Final list processing " << count << " elements...";
    FinalLst::iterator finalIt(lst.begin());
    bool ubInside = false;
    while(finalIt != lst.end()) {
      minF = min(minF, inf((*finalIt)->bound()));
      maxF = max(maxF, sup((*finalIt)->bound()));
      if(uBound.hasPoint() && MT::inside(uBound.point(), (*finalIt)->ref_X()))
        ubInside = true;
      ++finalIt;
    }
    count--;
    if(!(count % 1000))
      std::cout << count << "...";
    std::cout << "Enclosure [" << minF << ", " << maxF <<  "] Finished!" << std::endl;
    return ubInside;
  }


}



