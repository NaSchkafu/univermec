/**
 * @file   box.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:07:02 2009
 *
 * @brief  Implementation of the Box class
 *
 *
 */


#include <algorithm/opt-lib/optbox.hpp>

#include <boost/foreach.hpp>

#include "functions/ifunction.hpp"

#include <algorithm/opt-lib/helper.hpp>
#include <algorithm/opt-lib/exception.hpp>

namespace GlobOpt {

// ********************************************************************************
// OptBox members
  OptBox::OptBox(const core::arith::ivector &X, const core::arith::interval &lambda0, const core::arith::mreal
                 &lambda0Nwt, const MetaList &mConstraints)
    :GappedBox(X), m_oldX(X), m_lastX(X), m_flags(0), m_feasible(FEAS_NONE), m_mConstraints(mConstraints),
     m_lambda0(lambda0), m_lambda0Nwt(lambda0Nwt), m_deleted(false),
     m_bound(core::arith::interval(-core::arith::infinity(), core::arith::infinity())), 
     m_viol(core::arith::infinity())
  { }

  void OptBox::update(const core::arith::ivector &X)
  {
#ifdef GLOBOPT_PERFMON
    m_lastX = ref_X();
#endif
    ref_X() = X;
    if(!(m_feasible == FEAS_CERT_STRICT || m_feasible == FEAS_INFEAS))
      m_feasible = FEAS_NONE; // neue feasibility ist nicht bekannt
    //m_flags &= ~BOX_CHANGED;
  }

  void OptBox::updateOld(const core::arith::ivector &newOld)
  {
    m_oldX = newOld;
    //THROW(OpNotSupported, "Create a new box!");
  }

  OptBox::OptBox(const OptBox &b, bool clearOld)
    :GappedBox(b.X()), m_oldX(clearOld ? b.X() : b.old()), m_lastX(b.X()), m_flags(0),
     m_feasible(b.getFeas()), m_mConstraints(b.m_mConstraints), m_lambda0(b.m_lambda0),
     m_lambda0Nwt(b.m_lambda0Nwt), m_deleted(false), m_bound(b.m_bound)
  {
  }

  // OptBox OptBox::split(unsigned int direction)
  // {
  //   return split(direction, core::arith::interval(mid(m_X[direction])));
  // }

  void OptBox::setLambda0(const core::arith::interval &lambda0)
  {
    m_lambda0 = lambda0;
  }

  void OptBox::setLambda0Nwt(const core::arith::mreal &lambda0Nwt)
  {
    m_lambda0Nwt = lambda0Nwt;
  }

  OptBox* OptBox::bisect(unsigned int direction, const core::arith::interval &gap)
  {
    // neue OptBox
    OptBox *b = new OptBox(*this, true);
    if(sup(gap) > sup(b->ref_X()[direction]))
      b->m_deleted = true;
    //b->ref_X()[direction].set_inf(sup(gap));
    b->ref_X()[direction] = core::arith::interval(sup(gap), sup(b->ref_X()[direction]));

    if(!(b->m_feasible == FEAS_CERT_STRICT || b->m_feasible == FEAS_INFEAS))
      b->m_feasible = FEAS_NONE;

    // teilen der aktuellen OptBox
    if(inf(gap) < inf(ref_X()[direction]))
      m_deleted = true;
    //ref_X()[direction].set_sup(inf(gap));
    ref_X()[direction] = core::arith::interval(inf(ref_X()[direction]), inf(gap)); 
    if(!(m_feasible == FEAS_CERT_STRICT || m_feasible == FEAS_INFEAS))
      m_feasible = FEAS_NONE;

    b->updateGaps(direction);
    updateGaps(direction);

    return b;
  }

  void OptBox::updateGaps(unsigned int direction)
  {
    GapList gaps;
    BOOST_FOREACH(Gap g, m_gaps) {
      if(g.first == direction) {
        core::arith::interval &X = ref_X()[g.first];
        if(sup(X) <= g.second) {
	  X = core::arith::interval(inf(X), inf(g.second));
          //X.set_sup(inf(g.second));
        }
        else if(inf(X) <= g.second) {
	  X = core::arith::interval(sup(g.second), sup(X));
          //X.set_inf(sup(g.second));
        }
        else if(MT::overlap(g.second, X)) {
          gaps.push_back(g);
        }
      }
    }
    m_gaps.swap(gaps);
  }

  void OptBox::mergeGaps()
  {
    std::vector<Gap>::iterator it = m_gaps.begin();
    while(it != m_gaps.end()) {
      bool erased = false;
      for(std::vector<Gap>::iterator jIt = (it+1);
          jIt != m_gaps.end(); ++jIt) {
        if((jIt->first == it->first)
           && (MT::overlap(it->second, jIt->second))) {
          jIt->second = it->second | jIt->second;
          it = m_gaps.erase(it);
          erased = true;
          break;
        }
      }
      if(!erased)
        ++it;
    }
  }

  CBoxMeta::CBoxMeta(functions::IFunction &constraint, const core::arith::interval &lagMult, const core::arith::mreal &nwtExp)
    :m_constraint(constraint), m_lagMult(lagMult), m_nwtExp(nwtExp)
  {}

  void OptBox::updateFeasibility(bool noBreak) const
  {
    
    FEASIBILITY feas = FEAS_NONE;

    if(m_mConstraints.empty())
      feas = FEAS_CERT_STRICT;
    else {
      MetaList::iterator it(m_mConstraints.begin());
      while(it != m_mConstraints.end()) {
        core::arith::interval ev(it->constraint()(ref_X()));
        if(sup(ev) < 0.0) {
          if(feas == FEAS_NONE)
            feas = FEAS_CERT_STRICT;
          it = m_mConstraints.erase(it);
          continue;
        }
        else if(sup(ev) <= 0.0) {
          if(feas == FEAS_NONE || feas == FEAS_CERT_STRICT)
            feas = FEAS_CERT;
        }
        else if(inf(ev) > 0) {
          feas = FEAS_INFEAS;
          if(!noBreak)
            break;
        }
        else {
          m_viol = std::min(m_viol, sup(ev));
          feas = FEAS_UNKNOWN;
        }
        ++it;
      }
    }
    if(m_feasible == FEAS_CERT || m_feasible == FEAS_CERT_STRICT)
      std::cout << "FEASIBLE BOX gefunden" << std::endl;
    m_feasible = feas;
  }

  FEASIBILITY OptBox::getFeas() const
  {
    if(m_feasible == FEAS_NONE)
      updateFeasibility();
    return m_feasible;
  }

  FEASIBILITY OptBox::updateFeas() const
  {
    if(m_feasible == FEAS_NONE)
      updateFeasibility(true);
    return m_feasible;
  }



  bool OptBox::infeasible() const
  {
    return getFeas() == FEAS_INFEAS;
  }


  bool OptBox::feasible() const
  {
    switch(getFeas()) {
    case FEAS_UNKNOWN:
    case FEAS_INFEAS:
      return false;
    case FEAS_CERT:
    case FEAS_CERT_STRICT:
      return true;
    default:
      return false;
    }
  }

  bool OptBox::strictly_feasible() const
  {
    return getFeas() == FEAS_CERT_STRICT;
  }

  bool OptBox::feasible(const core::arith::rvector &x) const
  {
    core::arith::ivector ix(x);

    if(!MT::inside(x, ref_X())) {
      THROW(IllegalArguments, "Point x lies not inside this box!");
    }

    BOOST_FOREACH(CBoxMeta &c, m_mConstraints) {
      if(!(sup(c.constraint()(ix)) <= 0.0)) {
        return false;
      }
    }
    return true;
  }

  void OptBox::reset_old()
  {
    m_oldX = ref_X();
  }

  core::arith::mreal OptBox::max_viol() const
  {
    return m_viol;
  }

}
