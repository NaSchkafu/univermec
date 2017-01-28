/**
 * @file   box.hpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:07:16 2009
 * 
 * @brief  Header of the box class
 * 
 * 
 */


#ifndef __OPTBOX_HPP__
#define __OPTBOX_HPP__ __OPTBOX_HPP__

#include "types.hpp"
#include "core/gappedbox.hpp"

namespace functions
{
  class IFunction;
}

namespace GlobOpt {

  /// this class stores meta information for a box
  /*!
  ** This class stores informations about a box and a constraint
  ** (lagrange Multiplicator for the constraint, newton exp point and reference to the constraint
  **
  */
  class CBoxMeta {
  public:
    CBoxMeta(functions::IFunction &constraint, const core::arith::interval &lagMult,
	     const core::arith::mreal &nwtExp);

    inline functions::IFunction& constraint() const { return m_constraint; }
    inline core::arith::interval& lagMult() { return m_lagMult; }
    inline core::arith::mreal& nwtExp() { return m_nwtExp; }

  private:
    boost::reference_wrapper<functions::IFunction> m_constraint;
    core::arith::interval m_lagMult;
    core::arith::mreal m_nwtExp;
  };

  /// feasibility status of a box
  enum FEASIBILITY { FEAS_NONE = 1, FEAS_CERT = 2, FEAS_CERT_STRICT = 3, 
		     FEAS_UNKNOWN = 4, FEAS_INFEAS = 0 };


  /// a box
  /*!
  ** This class stores a box an several informations about the box
  ** It is used in the global optimzation algorithm
  ** NOTE: It is assumed that the box is never enlarged using the update
  ** or refX() function.
  **
  */
  class OptBox : public core::GappedBox {
  public:
    typedef std::vector<Gap> GapList;
    typedef std::vector<CBoxMeta> MetaList;

    // Konstuktoren
    OptBox(const core::arith::ivector &X, const core::arith::interval &lambda0,
	const core::arith::mreal &lambda0Nwt, const MetaList &mConstraints);

    OptBox() { /* UNSINN */ }

    virtual ~OptBox() {     }

    /*!
     * Constructor for copying a box
     **
     ** @param b box to copy
     ** @param clearOld true if the old reference box should be cleared
     ** (reference is set to the current box in this case)
     */
    OptBox(const OptBox &b, bool clearOld);

    /*!
    ** Get the reference box
    **
    **
    ** @return const ref to the reference box
    */
    inline const core::arith::ivector& old() const { return m_oldX; }

    /*!
    ** Get the reference box
    **
    **
    ** @return copy of the reference box
    */
    inline core::arith::ivector getOld() const { return m_oldX; }

    /*!
    ** Get the lambda0 value
    **
    **
    ** @return lambda0 value
    */
    inline const core::arith::interval& lambda0() const { return m_lambda0; }

    /**
     * Get the point for newton expansion for lambda0
     *
     *
     * @return lambda0 newton expansion point
     */
    inline const core::arith::mreal& lambda0Nwt() const { return m_lambda0Nwt; }

#ifdef GLOBOPT_PERFMON
    inline const core::arith::ivector& lastX() const { return m_lastX; }
#endif

    /**
     * Gets a  reference to the constraint meta informations
     * of this box
     *
     *
     * @return  ref to constraint meta info
     */
    inline  MetaList& mConstraints() { return m_mConstraints; }

    /**
     * Gets the feasibility status of the box
     * and performs a partial update of the constraint meta data
     *
     * @return
     */
    FEASIBILITY getFeas() const;

    /**
     * Gets the feasibility status and forces a
     * complete update of the constraint meta data
     *
     * @return
     */
    FEASIBILITY updateFeas() const;

    /**
     * Checks if the box is CERTAINLY infeasible
     *
     *
     * @return
     */
    bool infeasible() const;

    /**
     * Checks if the box is CERTAINLY feasible
     *
     *
     * @return
     */
    bool feasible() const;

    /**
     * Checks if the box is CERTAINLY infeasible
     *
     *
     * @return
     */
    bool strictly_feasible() const;

    /**
     * Checks if the point is feasible
     *
     * @param x point inside THIS box
     * NOTE: An exception is thrown, if x lies outside of this box
     *
     * @return true if the point is certainly feasible
     */
    bool feasible(const core::arith::rvector &x) const;

    /** 
     * Gets the maximum violation of the constraints of this box
     * If no maximum violation is known, infinity() is returned
     * 
     * @return maximum violation
     */
    core::arith::mreal max_viol() const;

    // non const
    /*!
    ** Update the box
    **
    ** @param X new box
    */
    void update(const core::arith::ivector &X);

    /*!
    ** Update the reference box
    **
    ** @param oldX new reference box
    */
    void updateOld(const core::arith::ivector &oldX);

    /** 
     * Resets the reference box to the current box4
     * 
     */
    void reset_old();

    /*!
    ** Updates lambda0 for this box
    **
    ** @param lagMult new lambda0 value
    */
    void setLambda0(const core::arith::interval &lambda0);

    /*!
    ** Updates expansion point for lambda0
    **
    ** @param lambda0Nwt new exp. point
    */
    void setLambda0Nwt(const core::arith::mreal &lambda0Nwt);

    /*!
    ** Split the box
    **
    ** @param direction direction for splitting
    **
    ** @return new box b with b.X() U this->X() = originalOptBox.X()
    */
    //OptBox split(unsigned int direction);

    /*!
    ** Split the box
    **
    ** @param direction direction for splitting
    ** @param gap gap for bisection, this interval is no longer 
    ** contained in the two resulting intervals
    ** 
    **
    ** @return new box b with b.X() U this->X() = originalOptBox.X()
    */
    OptBox* bisect(unsigned int direction, const core::arith::interval& gap);

    /*!
    ** Gets a (non const) reference to the gaps vector
    **
    **
    ** @return vector containing gaps
    */
    inline GapList& gaps() { return m_gaps; }

    /*!
    ** Merge gaps if possible
    **
    */
    void mergeGaps();

    inline bool isDeleted() const { return m_deleted; }

    inline const core::arith::interval& bound() const 
      {
	return m_bound;
      }
    
    inline core::arith::interval& bound()
      {
	return m_bound;
      }

    inline const core::arith::ivector& grad() const 
      {
	return m_grad;
      }
    
    inline core::arith::ivector& grad()
      {
	return m_grad;
      }

  private:
    /*!
    ** Update the gaps for the box
    **
    ** @param b the box
    */
    void updateGaps(unsigned int direction);

    /**
     * Updates the feasibilty status
     *
     */
    void updateFeasibility(bool noBreak = false) const;

    /// the box
    //core::arith::ivector m_X;

    /// original box
    core::arith::ivector m_oldX;

    /// box before last update
    core::arith::ivector m_lastX;

    ///  box flags
    unsigned int m_flags;

    /// feasibility status
    mutable FEASIBILITY m_feasible;

    /// informations about active constraints
    mutable MetaList m_mConstraints;

    /// enclosure of lambda0
    core::arith::interval m_lambda0;

    /// expansion point for lambda0
    core::arith::mreal m_lambda0Nwt;

    /// gaps where no solutions are possible
    GapList m_gaps;

    bool m_deleted;

    /// bound on objective function
    core::arith::interval m_bound;

    /// bound on the gradient
    core::arith::ivector m_grad;

    /// maximum violation
    mutable core::arith::mreal m_viol;
  };

  typedef std::vector<OptBox*> OptBoxLst;
  typedef std::vector<OptBox*>::iterator OptBoxIt;
  typedef std::vector<OptBox*>::const_iterator OptBoxConstIt;
}

#endif

