
/*!
**@file   tnlpwrapper.hpp
**@author Stefan Kiel
**@date   Wed Jan 21 16:19:08 2009
**
**@brief  This file delcares the necessary classes for solving the
**problem using the IPOPT Library V.3.0 by A. Wächter et al.
**
**
*/

#ifndef __TNLPWRAPPER_HPP__
#define __TNLPWRAPPER_HPP__ __TNLPWRAPPER_HPP__

#include <IpTNLP.hpp>
#include <core/arithcore.hpp>

namespace functions
{
  class IFunction;
  class IDerivative;
}

namespace extras {
  namespace interfaces
  {
    struct TNLPSolution {
      Ipopt::SolverReturn status;
      core::arith::rvector x;
      core::arith::rvector z_L;
      core::arith::rvector z_U;
      core::arith::rvector g;
      core::arith::rvector lambda;
      core::arith::mreal obj_value;
    };

    struct TNLPStatus {
      //AlgorithmMode mode;
      core::arith::mreal objValue;
      //core::arith::rvector x;
    };


/// interface for a TNLPObserver
/*!
** This interface allows observing the progress of the IpOpt library
** It forwards several informations which are available to the TNLPWrapper interface
** through the intermediate callback of the IpOpt library
*/
    class TNLPObserver {
    public:
      /*!
      ** Called for updating the current status of the ipopt library
      **
      ** @param status status information
      **
      */
      virtual void tnlpStatusUpdate(const TNLPStatus &status) = 0;
    };

/// A simple wrapper for using IpOpt with the GlobOpt library
/*!
** This wrapper implements the TNLP Interface for using the IpOpt libraray developed by Andreas
** Wächter et al (see https://projects.coin-or.org/Ipopt)
**
** All constraints of this interface are handled as inequality constraints of the form g(x) <= 0
** To use the full potential of IpOpt the wrapper has to be altered.
** IpOpt supports general optimization problems with bound, equality and inequality constraints.
**
*/
    class TNLPWrapper : public Ipopt::TNLP {
    public:
      typedef std::vector<boost::reference_wrapper<const functions::IFunction> > flst_t;

      /*!
      ** Constructor
      **
      ** @param objFunction objective function
      ** @param ineq_constraints list of constraint functions
      ** @param start feasible startpoint
      */
      TNLPWrapper(const functions::IFunction &objFunction, 
		  const flst_t &ineq_constraints,
                  const core::arith::rvector &start);
      
      inline const TNLPSolution& solution() const { return m_solution; }

      /*!
      ** Registers an observer with the wrapper
      **
      ** @param ob the observer
      */
      void registerObserver(TNLPObserver &ob);

      // **************************IPOPT Interface ***************************************************
      // See IpTNLP.hpp or IpOpt documentation for details
      virtual bool get_nlp_info(Ipopt::Index &n, Ipopt::Index &m, Ipopt::Index &nnz_jac_g,
                                Ipopt::Index &nnz_h_lag, Ipopt::TNLP::IndexStyleEnum &index_style);

      virtual bool get_bounds_info(Ipopt::Index n, Ipopt::Number *x_l,
                                   Ipopt::Number *x_u, Ipopt::Index m,
                                   Ipopt::Number *g_L, Ipopt::Number *g_u);

      virtual bool get_starting_point(Ipopt::Index n, bool init_x, Ipopt::Number *x,
                                      bool init_z, Ipopt::Number *z_L, Ipopt::Number *z_U,
                                      Ipopt::Index m, bool init_lambda, Ipopt::Number *lambda);

      virtual bool eval_f(Ipopt::Index n, const Ipopt::Number *x,
                          bool new_x, Ipopt::Number &obj_value);

      virtual bool eval_grad_f(Ipopt::Index n, const Ipopt::Number *x,
                               bool new_x, Ipopt::Number *grad_f);

      virtual bool eval_g(Ipopt::Index n, const Ipopt::Number *x, bool new_x,
                          Ipopt::Index m, Ipopt::Number *g);

      virtual bool eval_jac_g(Ipopt::Index n, const Ipopt::Number *x, bool new_x,
                              Ipopt::Index m, Ipopt::Index nele_jac,
                              Ipopt::Index *iRow, Ipopt::Index *jCol, Ipopt::Number *values);

      virtual bool eval_h(Ipopt::Index n, const Ipopt::Number *x, bool new_x,
                          Ipopt::Number obj_factor, Ipopt::Index m, const Ipopt::Number *lambda, bool new_lambda,
                          Ipopt::Index nele_hess, Ipopt::Index *iRow, Ipopt::Index *jCol, Ipopt::Number *values);


      virtual void finalize_solution(Ipopt::SolverReturn status, Ipopt::Index n,
                                     const Ipopt::Number *x, const Ipopt::Number *z_L,
                                     const Ipopt::Number *z_U, Ipopt::Index m,
                                     const Ipopt::Number *g, const Ipopt::Number *lambda,
                                     Ipopt::Number obj_value, const Ipopt::IpoptData *ip_data,
                                     Ipopt::IpoptCalculatedQuantities *ip_cq);

      virtual bool intermediate_callback(Ipopt::AlgorithmMode mode, Ipopt::Index iter,
                                         Ipopt::Number obj_value, Ipopt::Number inf_pr, Ipopt::Number inf_du,
                                         Ipopt::Number mu, Ipopt::Number d_norm,
                                         Ipopt::Number regularization_size, Ipopt::Number alpha_du,
                                         Ipopt::Number alpha_pr, Ipopt::Index ls_trials, const Ipopt::IpoptData *ip_data,
                                         Ipopt::IpoptCalculatedQuantities *ip_cq);

      // ***************************************** IPOPT Interface **********************************

    private:
      const functions::IFunction &m_F;
      const functions::IDerivative *m_dF;
      const flst_t &m_ineq_constraints;
      std::vector<const functions::IDerivative*> m_df_ineq_constraints;
      core::arith::rvector m_start;
      TNLPSolution m_solution;
      TNLPStatus m_status;
      std::vector<boost::reference_wrapper<TNLPObserver> > m_obs;
    };

    TNLPSolution run_ipopt(const functions::IFunction &obj, 
			   const TNLPWrapper::flst_t &ineq_constraints,
			   const core::arith::rvector &start,
			   double tol,
			   int max_iter, bool exact = true);
  }
}


#endif

