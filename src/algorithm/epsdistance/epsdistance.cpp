#include <algorithm/epsdistance/epsdistance.hpp>

#ifdef _OPENMP
#include <omp.h>
#endif

#include <boost/thread/mutex.hpp>
// TODO C11
#include <boost/foreach.hpp>
#include <boost/dynamic_bitset.hpp>

#include "objects/igeoobj.hpp"

#include "decomp/iivtree.hpp"
#ifdef _OPENMP
#include "decomp/syncivtree.hpp"
#endif
#include "decomp/decomputil.hpp"
#include "decomp/iboxlistmgr.hpp"
#include "decomp/sortedlistmgr.hpp"

#include "utility/utility.hpp"

#include "algorithm/idistobserver.hpp"
#include "algorithm/epsdistance/nodeprox.hpp"
#include "algorithm/utils/upperbound.hpp"

//#define DEBUG_OUTPUT

namespace epsdistance
{

  using namespace core::arith;
  using namespace decomp;

  struct dis_ele_t
  {
    dis_ele_t(IIVTreeNode &a_n1,
	      //const std::shared_ptr<boost::mutex> &a_lock_n1,
	      IIVTreeNode &a_n2, 
	      //const std::shared_ptr<boost::mutex> &a_lock_n2,
	      const interval &a_d)
      :n1(&a_n1), //lock_n1(a_lock_n1),
       n2(&a_n2), //lock_n2(a_lock_n2), 
       d(a_d)
      {  }

    // dis_ele_t()
    //   :n1(0), n2(0), d(1,-1)
    //   {   }

//    typedef omp_lock_t lock_t;

    IIVTreeNode* n1;
    IIVTreeNode* n2;

    // std::shared_ptr<boost::mutex> lock_n1;
    // std::shared_ptr<boost::mutex> lock_n2;g

    interval d;
  };

  std::ostream& operator<<(std::ostream &os, const dis_ele_t &d)
  {
    os << "X1 (";
    print_color(os, d.n1->color());
    os << "): " << d.n1->X() << " X2 (";
    print_color(os, d.n2->color());
    os << ": " << d.n2->X() << " d: " << d.d;
    return os;
  }

  struct DisEleCmp
  {
    DisEleCmp()
      :m_criterion(USE_INF)
      {
      }

    bool operator()(const dis_ele_t *e1, const dis_ele_t *e2)
      {
        switch(m_criterion) {
        case USE_WEIGHT:
          return wid(e1->n1->X())+wid(e1->n2->X()) < wid(e2->n1->X())+wid(e2->n2->X());
        case USE_INF:
          return !(inf(e1->d) < inf(e2->d));
        case USE_UBOUND:
          return abs(m_ubound - inf(e1->d)) < abs(m_ubound - inf(e2->d));
        }
      }

    enum criterion_t { USE_INF = 0, USE_UBOUND = 1, USE_WEIGHT = 2};
    criterion_t m_criterion;
    mreal m_ubound;
  };

  inline bool has_point(const IIVTreeNode &n)
  {
    return n.color() == decomp::BLACK || n.color() == decomp::NOT_WHITE;
  }

  struct local_env_t
  {
    typedef std::list<dis_ele_t*> final_lst_t;
    final_lst_t final_lst;
    unsigned final_count;

    algorithm::utils::UpperBoundIV ubound;
    algorithm::utils::UpperBoundIV lbound;

    //dis_ele_t lb_candid;
    std::list<dis_ele_t*> to_add;
  };

  struct global_env_t
  {
    global_env_t(const mreal& a_eps, const algorithm::IDistObserver *a_obs)
      :eps(a_eps), eps2(1.0), obs(a_obs), iterates(0), status(RUNNING)
      {}

    mreal eps;
    mreal eps2;
    const algorithm::IDistObserver* obs;
    unsigned iterates;
    enum ALG_STATUS { RUNNING, FINISHED, USER_ABORT, TIME_EXCEEDED } status;
    unsigned next_update;
    DisEleCmp cmp;
  };

  inline core::arith::ivector concat_ivec(const core::arith::ivector &v1, const core::arith::ivector &v2)
  {
    unsigned dim(mtl::size(v1));
    core::arith::ivector v(dim*2);
    v[mtl::irange(0,dim)] = v1[mtl::irange(0,dim)];
    v[mtl::irange(dim,2*dim)] = v2[mtl::irange(0,dim)];
    return v;
  }

  core::arith::ivector cross(const ivector &first, const ivector &second)
  {
    ivector ret(3);
    ret[0] = first[1]*second[2] - first[2]*second[1];
    ret[1] = first[2]*second[0] - first[0]*second[2];
    ret[2] = first[0]*second[1] - first[1]*second[0];
    return ret;
  }

  bool check_normal(core::arith::ivector x1, core::arith::ivector n1, 
		    core::arith::ivector x2, core::arith::ivector n2,
		    normal_test_t ntest)
  {
    if(!mtl::size(n1) || !mtl::size(n2))
      return true;
    if(ntest == NORMALS_CONE) {
      ivector x1x2(x1-x2);
      ivector x2x1(x2-x1);
      // CONE Check
      // OLD

      bool n_n2x1x2 = utility::normalize(x1x2) && utility::normalize(n2);
      bool n_n1x2x1 = utility::normalize(x2x1) && utility::normalize(n1);

      if(n_n2x1x2 && n_n1x2x1)
       	return !(is_empty(x1x2 & n2 & ivector(-n1)) || is_empty(x2x1 & n1 & ivector(-n2)));
      else if(n_n2x1x2)
	return !is_empty(x1x2 & n2);
      else if(n_n1x2x1)
	return !is_empty(x2x1 & n1);
      return true;
    }

    return sup(-mtl::dot(n1,n2)) >= 0 && ivector(3, interval(0.0)) <= cross(n1,n2);

//    else if(ntest == NORMALS_CROSS) {
      // Cross Product Check

      //  }
    //
    //return true;
  }

  core::arith::mreal final_proc(local_env_t &env,
                                global_env_t &g_env,
                                bool force_exit)
  {
    unsigned final_count_next = env.final_count - 1000;
    mreal tmplb = env.lbound();
    BOOST_FOREACH(dis_ele_t *e, env.final_lst) {
      // if(ubound - inf(e.d) >  eps)
      //   std::cout << "ubound - inf(e.d): " << ubound - inf(e.d) << std::endl;
      if(inf(e->d) <= env.ubound()) {
        //e.d = distance(*e.n1,*e.n2, tm1, tm2, itm1, itm2, true);
        //if(inf(e.d) <= ubound) {
        if(env.ubound() - inf(e->d) <= g_env.eps || force_exit) {
          env.lbound.update(std::min(env.lbound(), inf(e->d)));
          tmplb = std::min(tmplb, inf(e->d));
          // sichern fuer Observer
          // TODO: FIX
          //lb_candid = e;
          delete e;
        }
        else {
          env.to_add.push_back(e);
          tmplb = std::min(tmplb, inf(e->d));
        }
      }
      else
	delete e;
      env.final_count--;
      if(final_count_next > env.final_count) {
        // Statusupdate
#pragma omp master
        {
          if(g_env.obs->status(g_env.iterates, 0, env.final_count)){
            g_env.status = global_env_t::USER_ABORT;
          }
        }
        final_count_next = env.final_count*0.9;
      }
    }
    env.final_lst.clear();
    env.final_count = 0;
    return tmplb;
  }


  // berechnet abstand zwischen zwei Bäumen auf $eps$ genau
  interval eps_distance(decomp::IIVTree &a_t1, decomp::IIVTree &a_t2,
                        const mreal &eps, mreal a_ubound, unsigned max_usplit,
			normal_test_t ntest,
                        unsigned threads, const algorithm::IDistObserver* obs)
  {
    using namespace decomp;
    switch(ntest){
    case NORMALS_NONE:
      std::cout << "Normal support not requested!" << std::endl;
      break;
    case NORMALS_CONE:
      std::cout << "Normal support (Cone) requested!" << std::endl;
      break;
    case NORMALS_CROSS:
      std::cout << "Normal support (Cross) requested!" << std::endl;
      break;
    }
      
    if(ntest && !(a_t1.normals() && a_t2.normals())) {
      ntest = NORMALS_NONE;
      std::cout << "Trees do not support normals. Normal support deactivted!" << std::endl;
    }

    std::unique_ptr<decomp::IIVTree> t1ptr;
    std::unique_ptr<decomp::IIVTree> t2ptr;
#ifdef _OPENMP
    if(threads)
      omp_set_num_threads(static_cast<int>(threads));

    if(threads != 1) {
      std::cout << "Thread Unterstützung für epsdistance. Erzeuge SyncIVTree-Wrapper für Bäume!" << std::endl;
      t1ptr = make_unique<decomp::SyncIVTree>(a_t1);
      t2ptr = make_unique<decomp::SyncIVTree>(a_t2);
    }
#else
    threads = 1;
#endif

    decomp::IIVTree &t1 = t1ptr ? *t1ptr : a_t1;
    decomp::IIVTree &t2 = t2ptr ? *t2ptr : a_t2;

    //omp_set_num_threads(1);

    // Globaler UpperBound
    algorithm::utils::UpperBoundIV g_ubound;
    if(a_ubound < 0.0)
      a_ubound = infinity();
    // pre init falls max_usplit > 0 und ubound == infinity()
    if(a_ubound == infinity() && max_usplit) {
      IIVTreeNode* b1 = dynamic_cast<IIVTreeNode*>(find_node(t1, BLACK, max_usplit));
      IIVTreeNode* b2 = dynamic_cast<IIVTreeNode*>(find_node(t2, BLACK, max_usplit));

      if(b1 && b2) {
        a_ubound = inf(distance::node_distance(*b1,*b2));
        obs->ub(b1->X(), b2->X(), a_ubound);
      }
    }
    if(a_ubound >= 0.0)
      g_ubound.update(a_ubound);

    // global_env
    global_env_t g_env(eps, obs);
    decomp::SortedBoxListMgr<dis_ele_t, DisEleCmp> mgr(g_env.cmp);

    mgr.add(new dis_ele_t(t1.root(), //std::make_shared<boost::mutex>(),
			  t2.root(), //std::make_shared<boost::mutex>(),
                          distance::node_distance(t1.root(), t2.root())));
    //mreal lbound(infinity());
//    DisEleCmp cmp;

    //bool cont = true;
    bool force_exit = false;


    bool running = true;


    boost::dynamic_bitset<> active_threads;
    core::arith::mreal g_tmplb(infinity());
    algorithm::utils::UpperBoundIV g_lbound;
    unsigned g_ele_threads;
    IBoxListMgr<dis_ele_t>::box_lst_t g_rele;
    unsigned no_threads;

#pragma omp parallel default(none) shared(running, active_threads, g_env, mgr, std::cout, g_rele, g_ele_threads, g_ubound, g_lbound, no_threads, force_exit, g_tmplb, eps, ntest, a_ubound)
    {
      enum THREAD_STATUS { T_PENDING, T_NEW_BOXES, T_NEXT_PHASE  } lt_status;
      // local_env
      local_env_t env;
      // if(a_ubound >= 0)
      // 	env.ubound.update(a_ubound);
      
      unsigned long long next_update = 5000;
      unsigned thread_no = 0;
#ifdef _OPENMP
      thread_no = omp_get_thread_num();
#endif

#pragma omp single 
      {
#ifdef _OPENMP
	no_threads = omp_get_num_threads();
#else
	no_threads = 1;
#endif
	std::cout << "EpsDistance mit " << no_threads << " Threads gestartet!" << std::endl;
	active_threads.resize(no_threads, true);	
      }
      

      while(running) {

#pragma atomic
	g_env.iterates++;
	
        // Status Update
#pragma omp master
        {
          if(g_env.iterates > next_update) {
            if(g_env.obs->status(g_env.iterates, mgr.count(), env.final_count))
//#pragma omp atomic
              g_env.status = global_env_t::USER_ABORT;
            next_update = g_env.iterates + 10000;
	    if(env.ubound.hasPoint()) {
	      unsigned dim(mtl::size(env.ubound.point())/2);
	      g_env.obs->ub(ivector(env.ubound.point()[mtl::irange(0, dim)]),
			    ivector(env.ubound.point()[mtl::irange(dim, 2*dim)]),
			    env.ubound());
	    }
	    if(env.lbound.hasPoint()) {
	      unsigned dim(mtl::size(env.lbound.point())/2);
	      g_env.obs->lb(ivector(env.lbound.point()[mtl::irange(0, dim)]),
			    ivector(env.lbound.point()[mtl::irange(dim, 2*dim)]),
			    env.lbound());
	    }
          }
        }

        dis_ele_t *e = nullptr;

// Auswahl des nächsten Elements
#pragma omp critical(BOXMGR)
        {
          if(!env.to_add.empty())
            for_each(env.to_add.begin(), env.to_add.end(), [&mgr](dis_ele_t *e) { mgr.add(e); } );
          if(!mgr.empty() && g_env.status == global_env_t::RUNNING)
            e = mgr.select();
	  g_ubound.sync(env.ubound);
	  g_lbound.sync(env.lbound);
        }
        env.to_add.clear();

        // Kein Element vorhanden
        if(!e) {
#pragma omp critical(ACTIVETHREADS)
          {
#ifdef DEBUG_OUTPUT
            std::cout << "Thread " << thread_no
                      << " blockiert und wartet auf neue Boxen." << std::endl;
#endif
            active_threads.reset(thread_no);
          }
          lt_status = T_PENDING;
          // Busy Waiting
          while(lt_status == T_PENDING) {
#pragma omp critical(BOXMGR)
            {
              if(mgr.count() && g_env.status == global_env_t::RUNNING)
                lt_status = T_NEW_BOXES;
            }
#pragma omp critical(ACTIVETHREADS)
            {
              if(active_threads.none())
                lt_status = T_NEXT_PHASE;
            }
            usleep(100);
          } // Ende Busy Waiting
          if(lt_status == T_NEW_BOXES) {
#pragma omp critical(ACTIVETHREADS)
            {
              active_threads.set(thread_no);
            }
#ifdef DEBUG_OUTPUT
	    std::cout << "Thread " << thread_no  << " reaktiviert!" << std::endl;
#endif
            continue;
          }
#ifdef DEBUG_OUTPUT
	  std::cout << "Thread " << thread_no  << " Phase 2!" << std::endl;
#endif
#pragma omp single
          {
            if(!mgr.empty()) {
              g_rele = mgr.list();
              mgr.clear();
              g_ele_threads = (g_rele.size()-1)/no_threads;
            }
            else
              g_ele_threads = 0;
          } // Barriere
          if(g_ele_threads) {
#pragma omp critical(COPY)
            {
              unsigned offset(g_rele.size()-g_ele_threads-1);
              std::copy(g_rele.begin()+offset, g_rele.end(), std::back_inserter(env.final_lst));
              g_rele.erase(g_rele.begin()+offset, g_rele.end());
            }
          }
#pragma omp barrier

          // Finale Liste abarbeiten
#pragma omp single
          {
            std::copy(g_rele.begin(), g_rele.end(), std::back_inserter(env.final_lst));
            g_rele.clear();
            if(g_env.eps2 > 10e-15 && g_env.status == global_env_t::RUNNING)
              g_env.eps2 *= 0.9;
            else
              force_exit = true;
            if(env.ubound() < infinity()) {
              g_env.cmp.m_ubound = env.ubound();
              g_env.cmp.m_criterion = static_cast<DisEleCmp::criterion_t>((g_env.cmp.m_criterion + 1) % 2);
// #ifdef DEBUG_OUTPUT
//               std::cout << "Kriterium wird gewechselt auf " << (cmp.m_use_inf ? "inf" : "ubound")
//                         << " mit eps2 " << eps2 << std::endl;
// #endif
              mgr = SortedBoxListMgr<dis_ele_t, DisEleCmp>(g_env.cmp);
            }
          } // Barriere

          // Finale Proc
	  bool was_running = g_env.status == global_env_t::RUNNING;
          core::arith::mreal tmplb(final_proc(env, g_env, force_exit));

#pragma omp critical(TMPLB)
          {
            g_tmplb = std::min(g_tmplb, tmplb);
          }

          // Wieder einfügen!
          if(!env.to_add.empty()) {
#pragma omp critical(BOXMGR)
            {
              for_each(env.to_add.begin(), env.to_add.end(), [&mgr](dis_ele_t *e) { mgr.add(e); } );
            }
            env.to_add.clear();
          }
#pragma omp barrier

          // Kandidaten, Distanz und Status updaten
#pragma omp master
          {
            // if(env.lb_candid.n1)
            //   g_env.obs->lb(env.lb_candid.n1->X(), env.lb_candid.n2->X(), env.lbound());
            g_env.obs->distance(core::arith::interval(tmplb, env.ubound()));
	    g_env.obs->status(g_env.iterates, mgr.count(), env.final_count);
            if(mgr.empty() && g_env.status == global_env_t::RUNNING)
              g_env.status = global_env_t::FINISHED;
	    // Sicherstellen einer Endbearbeitung aller Boxen vor verlassen der Schleife
	    running = was_running || g_env.status == global_env_t::RUNNING;	    
            g_tmplb = core::arith::infinity();
            active_threads.set();
          }
#pragma omp barrier
          continue;
        }
        // Check
//	std::cout << "Thread " << thread_no << "ENTER" << std::endl;
	if(inf(e->d) <= env.ubound()) {
          // splitten
	  bool first = (wid(e->n1->X()) > wid(e->n2->X()) || is_solid(*e->n2)) && is_dividable(*e->n1);
	  IIVTreeNode *sn = sn = (first ? e->n1 : e->n2);
	  sn->split();
	  unsigned ch = sn->children();

          for(unsigned i = 0; i < ch; i++) {
            decomp::IIVTreeNode& nn1 = first ? (*sn)[i] : *e->n1;
            decomp::IIVTreeNode& nn2 = !first ? (*sn)[i] : *e->n2;

	    //assert( &(nn1->tree()) != &(nn2->tree()) && "TREE" );
	    

            if(!(decomp::is_white(nn1) || decomp::is_white(nn2)) &&
	       (!ntest || check_normal(nn1.X(), nn1.normal(), nn2.X(), nn2.normal(), ntest))) {
	      
	      //assert(nn1->color() != decomp::WHITE && nn2->color != decomp::WHITE && "WHITE");
	      
	      // neues Element konstruiert
              dis_ele_t *nen = new dis_ele_t(nn1, nn2, distance::node_distance(nn1, nn2));
              if(inf(nen->d) <= env.ubound()) {
                // Beide Knoten schwarz, Distanz berechnen
                if(has_point(*nen->n1) && has_point(*nen->n2)) {
                  env.ubound.update(concat_ivec(nen->n1->X(), nen->n2->X()), sup(nen->d));
		  // assert ( sup(nn1->tree()->cf()(nn1->X())) <= 0.0 &&
		  // 	   sup(nn2->tree()->cf()(nn2->X())) <= 0.0
		  // 	   && "BLACK" );
		}
		
                if(nen->n1->color() == decomp::BLACK && nen->n2->color() == decomp::BLACK) {
                  env.lbound.update(concat_ivec(nen->n1->X(), nen->n2->X()), inf(nen->d));
                  delete nen;
	      	}
                // Genauigkeitsschranke erreicht
                else if((env.ubound() - inf(nen->d) <= eps)) {
                  env.lbound.update(concat_ivec(nen->n1->X(), nen->n2->X()), inf(nen->d));
                  delete nen;
                }
                // Zweite Schranke
                else if((wid(nen->n1->X()) < g_env.eps2 || is_solid(*nen->n1)) &&
                        (wid(nen->n2->X()) < g_env.eps2 || is_solid(*nen->n2))) {
                  env.final_lst.push_back(nen);
                  env.final_count++;
                }
                // Box für weitere Bearbeitung speichern
                else {
                  env.to_add.push_back(nen);
                }
              }
            }
          }
        }
	delete e;
//	std::cout << "Thread " << thread_no << "LEAVE" << std::endl;
      }

      // Ende der Hauptschleife
      // Zusammenführen der Informationen
#pragma omp critical(FINAL)
      {
        g_tmplb = std::min(g_tmplb, env.lbound());
        g_ubound.sync(env.ubound);
      }
    }

    unsigned dim(mtl::size(g_ubound.point())/2);
    if(g_ubound.hasPoint()) {
      unsigned dim(mtl::size(g_ubound.point())/2);
      g_env.obs->ub(ivector(g_ubound.point()[mtl::irange(0, dim)]),
		    ivector(g_ubound.point()[mtl::irange(dim, 2*dim)]),
		    g_ubound());
    }
    
    if(g_lbound.hasPoint()) {
      unsigned dim(mtl::size(g_lbound.point())/2);
      g_env.obs->lb(ivector(g_lbound.point()[mtl::irange(0, dim)]),
		    ivector(g_lbound.point()[mtl::irange(dim, 2*dim)]),
		    g_lbound());
    }
    

    std::cout << "EpsDistance Finished!" << std::endl;

    if(g_tmplb == infinity())
      return interval(0, infinity());
    return sqrt(interval(g_tmplb, g_ubound()));
  }
}

//
// epsdistance.cpp ends here
