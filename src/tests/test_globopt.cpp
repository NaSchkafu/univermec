#include <boost/foreach.hpp>
#include <boost/timer/timer.hpp>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "functions/ifunction.hpp"
#include "functions/iffeatures.hpp"
#include "functions/igpueval.hpp"
#include "functions/igpufuture.hpp"
#include "functions/iderivative.hpp"
#include "functions/parser/functionparser.hpp"

#include "algorithm/opt-lib/globoptalgo.hpp"
#include "algorithm/opt-lib/defaultstrategy.hpp"

struct opt_problem_t 
{
  opt_problem_t(functions::IFunction *objective, const core::arith::ivector &sbox) 
    :obj(objective), startbox(sbox)
    {
      for(unsigned i = 0; i < size(startbox); i++) {
	std::stringstream sstr;
	sstr << "x" << i << "-" << sup(startbox[i]);
	constraints.push_back(boost::reference_wrapper<functions::IFunction>(*functions::parser::parse_string(sstr.str(),obj->dim())));
	sstr.str(std::string());
	sstr << inf(startbox[i]) << "-x" << i;
	constraints.push_back(boost::reference_wrapper<functions::IFunction>(*functions::parser::parse_string(sstr.str(),obj->dim())));
      }
    }

  ~opt_problem_t()
    {
      delete obj;
      BOOST_FOREACH(functions::IFunction &c, constraints)
	delete &c;
    }

  functions::IFunction *obj;
  core::arith::ivector startbox;
  GlobOpt::FLst constraints;
};

// Six-Hump-Camel Back problem
opt_problem_t shcb()
{
  functions::IFunction *objective = functions::parser::parse_string("0.3333*pow(x0,6)-2.1*pow(x0,4)+4*sqr(x0)+x0*x1+4*pow(x1,4)-4*sqr(x1)");  
  core::arith::ivector startbox(2, core::arith::interval(-5, 5));
  return opt_problem_t(objective, startbox);
}

// Testproblem von Zowe
opt_problem_t zowe()
{
  functions::IFunction *objective = functions::parser::parse_string("sqr(x0-2)+sqr(x1-1)");
  core::arith::ivector startbox(2, core::arith::interval(-10, 10));
  opt_problem_t prob(objective, startbox);
  prob.constraints.push_back(boost::reference_wrapper<functions::IFunction>(*functions::parser::parse_string("sqr(x0)-x1")));
  prob.constraints.push_back(boost::reference_wrapper<functions::IFunction>(*functions::parser::parse_string("x0+x1-2")));
  return prob;
}

// Testproblem Levy-Gomez
opt_problem_t levy_gomez()
{
  functions::IFunction *objective = functions::parser::parse_string("0.1*(sqr(x0)+sqr(x1))");
  core::arith::ivector startbox(2, core::arith::interval(-1, 1));
  opt_problem_t prob(objective, startbox);
  prob.constraints.push_back(boost::reference_wrapper<functions::IFunction>(*functions::parser::parse_string("2*sin(2*3.14159265*x1)-sin(4*3.14159265*x0)")));
  return prob;
}

opt_problem_t wingo() 
{
  functions::IFunction *objective = functions::parser::parse_string("pow(x0,6)-2.08*pow(x0,5)+0.4875*pow(x0,4)+7.1*pow(x0,3)-3.95*sqr(x0)-x0+0.1");
  core::arith::ivector startbox(1, core::arith::interval(-2,11));
  return opt_problem_t(objective,startbox);
}

void test_globopt()
{
  unsigned iterations = 0;
  
  //const opt_problem_t &prob = shcb();
  //const opt_problem_t &prob = zowe();
  //const opt_problem_t &prob = levy_gomez();
  const opt_problem_t &prob = wingo();

  boost::timer::cpu_timer globopt_timer;
  std::unique_ptr<GlobOpt::DefaultStrategy> strat(new GlobOpt::DefaultStrategy(10e-2, 10e-4, 10e-5));
  GlobOpt::GlobOptAlgo algo(*prob.obj, prob.constraints, *strat, iterations);
  algo.start(prob.startbox);
  const GlobOpt::GlobOptResult& r = algo.result();
  std::cout << "****************************************************************" << std::endl;
  std::cout << "Laufzeit Algorithmus (allein): " << globopt_timer.format() << std::endl;
  std::cout << "****************************************************************" << std::endl;

  std::cout << "Beginne mit Lösungsverarbeitung" << std::endl;
  std::cout << "****************************************************************" << std::endl;
  unsigned size = r.finalLst.size();
  auto itEnd(r.finalLst.end());
  std::cout << "Found solutions: " << size << std::endl;
  BOOST_FOREACH(const GlobOpt::OptBox &b, r.finalLst) {
    std::cout << "Box: " << b.ref_X() << std::endl;
    std::cout << "Bound: " << b.bound() << std::endl;
    std::cout << "****************************************************************" << std::endl;
  }
  std::cout << r;
}
