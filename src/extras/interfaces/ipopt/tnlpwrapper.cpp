/**
 * @file   tnlpwrapper.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 15:10:29 2009
 *
 * @brief  Impl. of TNLPWrapper class
 *
 *
 */


#include <boost/foreach.hpp>
#include <IpIpoptApplication.hpp>

#include "functions/ifunction.hpp"
#include "functions/iffeatures.hpp"
#include "functions/iderivative.hpp"
#include "functions/functionhelper.hpp"
#include "extras/interfaces/interface_exception.hpp"

// FIXME
#include "algorithm/opt-lib/helper.hpp"

#include "extras/interfaces/ipopt/tnlpwrapper.hpp"

using namespace Ipopt;
using namespace functions;

namespace extras {
  namespace interfaces
  {


    TNLPWrapper::TNLPWrapper(const IFunction &objFunction, const flst_t &ineq_constraints,
                             const core::arith::rvector &start)
      :m_F(objFunction), m_ineq_constraints(ineq_constraints), m_start(start)
    {
      if(size(start) != m_F.dim()) {
        BOOST_THROW_EXCEPTION(interface_exception() << core::error_details("Please check "
                                                                           "dimension of supplied starting point"));
      }

      m_dF =  m_F.features().derivative();
      if(!m_dF) {
        BOOST_THROW_EXCEPTION(interface_exception() << core::error_details("The TNLPWrapper can only "
                                                                           "used with objective functions supporting the IDerivative interface!"));
      }

      BOOST_FOREACH(const IFunction &f, ineq_constraints) {
        m_df_ineq_constraints.push_back(f.features().derivative());
        if(!m_df_ineq_constraints.back()) {
          BOOST_THROW_EXCEPTION(interface_exception() << core::error_details("The TNLPWrapper can only "
                                                                             "used with constraints functions supporting the IDerivative interface!"));
        }
      }

      // Init structure
      m_solution.x = core::arith::rvector(m_F.dim());
      m_solution.z_L = core::arith::rvector(m_F.dim());
      m_solution.z_U = core::arith::rvector(m_F.dim());
      m_solution.lambda = core::arith::rvector(ineq_constraints.size());
    }

    bool TNLPWrapper::get_nlp_info(Index &n, Index &m, Index &nnz_jac_g,
                                   Index &nnz_h_lag, IndexStyleEnum &index_style)
    {
      n = m_F.dim();
      m = m_ineq_constraints.size();

      // Im Prinzip kann hier eine Optimierung erfolgen
      // in jeder NB kommen maximal n/2 aller Variablen vor
      // der Einfachheit halber wird die Matrix als dicht spezifiziert
      nnz_jac_g = n*m;

      // nur ein teil der matrix muss angegeben werden (Symmetrie)
      nnz_h_lag = n*(n+1)*0.5;

      index_style = TNLP::C_STYLE;

      return true;
    }


    bool TNLPWrapper::get_bounds_info(Index n, Number *x_l, Number *x_u, Index m,
                                      Number *g_l, Number *g_u)
    {
      assert(n == (int)m_F.dim());
      assert(m == (int)m_ineq_constraints.size());

      // keine obere/untere Grenze für X
      for(Index i = 0; i < n; i++) {
        x_l[i] = -2e19;
        x_u[i] = 2e19;
      }

      // alle NBs sind Ungleichungen der Form g_i(x) <= 0
      for(int i = 0; i < m; i++) {
        g_l[i] = -2e19;
        g_u[i] = 0;
      }

      return true;
    }


    bool TNLPWrapper::get_starting_point(Index n, bool init_x, Number *x,
                                         bool init_z, Number*, Number*,
                                         Index m, bool init_lambda, Number*)
    {
      assert(n == (int)m_F.dim());
      assert(m == (int)m_ineq_constraints.size());

      if(init_z) {
        BOOST_THROW_EXCEPTION(interface_exception() << core::error_details("Init of bound "
                                                                           "multiplieres requested. This is not supported by this wrapper!"));
      }
      else if(init_lambda) {
        BOOST_THROW_EXCEPTION(interface_exception() << core::error_details("Init of constraint "
                                                                           "multiplieres requested. This is not supported by this wrapper!"));
      }
      assert(init_x);

      for(Index i = 0; i < n; i++)
        x[i] = to_double(m_start[i]);

      return true;
    }


    bool TNLPWrapper::eval_f(Index n, const Number *x, bool , Number &obj_value)
    {
      assert(n == (int)m_F.dim());

      //std::cout << "Eingabe: " << MT::doubleToVec(x, n) << std::endl;

      core::arith::mreal vf(m_F(MT::doubleToVec(x, n)));
      if(is_special(vf))
        return false;

      obj_value = to_double(vf);
      return true;
    }

    bool TNLPWrapper::eval_grad_f(Index n, const Number *x, bool, Number *grad_f)
    {
      assert(n == (int)m_F.dim());

      // BACKWARD
      //core::arith::rvector grad(m_dF->grad(MT::doubleToVec(x, n)));
      // FWD
      core::arith::rvector grad(n+1);
#pragma omp parallel for schedule(dynamic,1)
      for(unsigned i = 0; i < n; i++)
	grad[i+1] = m_dF->grad(i+1)(MT::doubleToVec(x, n));
     

      for(Index i = 0; i < n; i++) {
        if(is_special(grad[i+1]))
          return false;
        grad_f[i] = to_double(grad[i+1]);
      }

      return true;
    }

    bool TNLPWrapper::eval_g(Index n, const Number *x, bool, Index m, Number *g)
    {
      assert(n == (int)m_F.dim());
      assert(m == (int)m_ineq_constraints.size());

      core::arith::rvector args(MT::doubleToVec(x, n));
      for(Index i = 0;i < m; i++) {
        core::arith::mreal gv(m_ineq_constraints[i].get()(args));
        if(is_special(gv))
          return false;
        g[i] = to_double(gv);
      }
      return true;
    }

    bool TNLPWrapper::eval_jac_g(Index n, const Number *x, bool, Index m, Index nele_jac,
                                 Index *iRow, Index *jCol, Number *values)
    {
      // Struktur der Sparse Matrix aufbauen
      if(iRow && jCol) {
        int l = 0, k = 0, i = 0;
        for(i = 0; i < m*n; i++) {
#ifdef DEBUG_OUTPUT
          printf("iRow[%d] = %d; jCol[%d] = %d\n",i,i/l,i,l);
#endif
          iRow[i] = k;
          jCol[i] = l++;
          if(!(l % n)) {
            l = 0;
            k++;
          }
        }
        // Index muss nicht null elementen in der Jacobimatrix entsprechen
        assert(i == nele_jac);
      }
      else if(x && values) {
        core::arith::rvector args(MT::doubleToVec(x, n));
        Index k = 0;
        for(int j = 0; j < m; j++) {
          core::arith::rvector g(m_df_ineq_constraints[j]->grad(args));
//	  std::cout << g << std::endl;
          for(int i = 1; i <= n; i++) {
            // TODO: FIX
            if(is_special(g[i]))
              return false;
            values[k++] = to_double(g[i]);
          }
        }
      }
      else {
        BOOST_THROW_EXCEPTION(interface_exception() << core::error_details("Arguments are not "
                                                                           "supported. Seems like an error in the  IpOPT library"));
      }
      return true;
    }

    bool TNLPWrapper::eval_h(Index n, const Number *x, bool, Number obj_factor,
                             Index m, const Number *lambda, bool,
                             Index nele_hess, Index *iRow, Index *jCol, Number *values)
    {
      assert(n == (int)m_F.dim());
      assert(m == (int)m_ineq_constraints.size());

      if(iRow && jCol) {
        int i = 0;
        for(Index row = 0; row < n; row++)
          for(Index col = 0; col <= row; col++) {
            iRow[i] = row;
            jCol[i++] = col;
          }
        // Index muss Anzahl der nicht null elemente der Hesse Matrix der Lagrange Funktion entsprechen
        assert(i == nele_hess);
      }
      else if(values) {
        // Erzeugen der unteren linken Dreiecksmatrix der Hessematrix der Lagrange Funktion

        core::arith::rvector args(MT::doubleToVec(x, n));

        Index i = 0;

	// Backward
        //core::arith::rmatrix hess(hessian(m_F, args));
	// FWD
	core::arith::rmatrix hess(n,n);
#pragma omp parallel for schedule(dynamic,1)
	for(unsigned i = 0; i < n; i++)
	  for(unsigned j = 0; j < n; j++)
	    if(i >= j)
	      hess[i][j] = m_dF->grad(i+1).features().derivative()->grad(j+1)(args);


	for(unsigned i = 0; i < n; i++)
	  for(unsigned j = 0; j < n; j++)
	    if(i < j)
	      hess[i][j] = hess[j][i];
	
        hess *= obj_factor;

	for(Index c = 0; c < m; c++)
	  hess += lambda[c]*hessian(m_ineq_constraints[c], args);

	//std::cout << hess << std::endl;

        // IpOpt Format konvertieren
        for(Index row = 0; row < n; row++)
          for(Index col = 0; col <= row; col++)
            if(is_special(hess[row][col]))
              return false;
            else
              values[i++] = to_double(hess[row][col]);
      }


      //return to_double(hess);




      //   for(Index row = 1; row <= n; row++) {
      //        for(Index col = 1; col <= row; col++) {



      //          interval iv(m_F.gradF(row).gradF(col)(args));
      //          if(MT::isSpecialValue(iv))
      //            return false;
      //          values[i] = obj_factor*_double(mid(iv));
      //          for(Index c = 0; c < m; c++) {
      //            iv = m_constraints[c].get().gradF(row).gradF(col)(args);
      //            values[i] += lambda[c]*_double(mid(iv));
      //          }
      //          i++;
      //        }
      //   }
      // }
      else {
        BOOST_THROW_EXCEPTION(interface_exception() << core::error_details("Arguments are not "
                                                                           "supported. Seems like an error in the  IpOPT library"));
      }

      return true;
    }

    void TNLPWrapper::finalize_solution(SolverReturn status, Index n, const Number *x,
                                        const Number *z_L, const Number *z_U, Index m,
                                        const Number *g, const Number *lambda,
                                        Number obj_value, const IpoptData *ip_data, IpoptCalculatedQuantities *ip_cq)
    {
      for (Index i=0; i<n; i++) {
        m_solution.x[i] = x[i];
      }

      for (Index i = 0; i < n; i++) {
        m_solution.z_L[i] = z_L[i];
      }

      for (Index i = 0; i < n; i++) {
        m_solution.z_U[i] = z_U[i];
      }

      for(Index i = 0; i < m; i++)
        m_solution.lambda[i] = lambda[i];

      m_solution.obj_value = obj_value;
      m_solution.status = status;
    }



    bool TNLPWrapper::intermediate_callback(AlgorithmMode mode, Index iter, Number obj_value,
                                            Number inf_pr, Number inf_du,
                                            Number mu, Number d_norm, Number regularization_size, Number alpha_du,
                                            Number alpha_pr, Index ls_trials, const IpoptData *ip_data,
                                            IpoptCalculatedQuantities *ip_cq)
    {
      m_status.objValue = core::arith::mreal(obj_value);
      BOOST_FOREACH(TNLPObserver &o, m_obs) {
        o.tnlpStatusUpdate(m_status);
      }
      return true;
    }

    void TNLPWrapper::registerObserver(TNLPObserver &ob)
    {
      m_obs.push_back(boost::reference_wrapper<TNLPObserver>(ob));
    }

    TNLPSolution run_ipopt(const functions::IFunction &obj, 
			   const TNLPWrapper::flst_t &ineq_constraints,
			   const core::arith::rvector &start,
			   double tol,
			   int max_iter, bool exact)
    {
      assert(tol > 0 && "tol <= 0!");
      assert(max_iter >= 0 && "max_iter < 0!");

      std::cout << "Starting point " << start << std::endl;

      Ipopt::SmartPtr<Ipopt::IpoptApplication> app = new Ipopt::IpoptApplication;
//     // nur versuchen einen guten Startwert f�r globalUB zu finden
     app->Options()->SetNumericValue("tol", tol);
     app->Options()->SetNumericValue("acceptable_tol", 100*tol);
     app->Options()->SetIntegerValue("max_iter", max_iter);
     app->Options()->SetNumericValue("constr_viol_tol", 1e-5);
     if(exact)
       app->Options()->SetStringValue("hessian_approximation", "exact");
     else
       app->Options()->SetStringValue("hessian_approximation", "limited-memory");
     //app->Options()->SetStringValue("evaluate_orig_obj_at_resto_trial", "no");
     //app->Options()->SetIntegerValue("max_soc", 0);
     //app->Options()->SetStringValue("alpha_for_y", "full");
     
//     app->Options()->SetIntegerValue("print_level", 4);
//#ifdef DEBUG 
     //app->Options()->SetStringValue("derivative_test","second-order");
     //app->Options()->SetStringValue("derivative_test_print_all","no");
//#endif

     Ipopt::ApplicationReturnStatus status;
     status = app->Initialize();
     if(status != Ipopt::Solve_Succeeded) {
       BOOST_THROW_EXCEPTION(interface_exception() << core::error_details("Could not init IpOpt!"));
     }

     TNLPWrapper *tnlp_wrap = new TNLPWrapper(obj, ineq_constraints, start);
     Ipopt::SmartPtr<Ipopt::TNLP> tnlp(tnlp_wrap);
     status = app->OptimizeTNLP(tnlp);
     return TNLPSolution(tnlp_wrap->solution());
    }
  }
}
