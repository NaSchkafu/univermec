#include <polyeval.h>

#include "core/arithcore.hpp"

namespace core 
{
  namespace arith 
  {
    // std::ostream& operator<<(std::ostream &os, const taylormodel &t)
    // {
    //   return t.print(os);
    // }

    // std::ostream& taylormodel::print(std::ostream &os) const
    // {
    //   os << m_taylormodel;
    //   return os;
    // }

    bool taylormodel::init() 
    {
      unsigned order = 4;
      std::cout << "Init TaylorModel Order to " << order << std::endl;
      riot::TaylorModel::set_order(order);
      riot::TaylorModel::set_polynomial_range_evaluation(riot::LDB::getObject());
      return true;
    }

    void taylormodel::set_order(unsigned order) 
    {
      riot::TaylorModel::set_order(order);
    }

    unsigned taylormodel::order() 
    {
      return riot::TaylorModel::order();
    }

    void taylormodel::set_bounder(taylormodel::TM_BOUNDER bounder) 
    {
      switch(bounder) {
      case TM_INTERVAL:
	riot::TaylorModel::set_polynomial_range_evaluation(riot::IntervalEvaluation::getObject());
	break;
      case TM_MEAN_VALUE:
	riot::TaylorModel::set_polynomial_range_evaluation(riot::MeanValueForm::getObject());
	break;
      case TM_LDB:
	riot::TaylorModel::set_polynomial_range_evaluation(riot::LDB::getObject());
	break;
      default:
	assert(!"Unknown bounder");
      }
    }

    

    // taylormodel create_tm_var(const interval &i)
    // {
    //   using namespace riot;
    //   static unsigned number = 0;
    //   std::stringstream sstr;
    //   // TODO: Threadsicher machen
    //   // #pragma atomic ...
    //   sstr << "autotm" << number++;
    //   // std::cout << "Erzeuge neue Variables für interval [" << inf(i) 
    //   // 		<< "," << sup(i) << "] # " << vname  << std::endl;
    //   // TaylorModel *tm = new TaylorModel();
    //   return mid(i)+0.5*diam(i)*taylormodel(sstr.str());
    // }

    bool taylormodel::force_init = taylormodel::init();

  }
}
