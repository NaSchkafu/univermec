
#include <decomp/sivtree.hpp>

namespace decomp
{
  namespace details
  {

    core::arith::ivector sivtree_eval<core::arith::taylormodel>::eval(const core::arith::ivector &X,
								      const objects::IGeoObj &obj,
								      bool normal)
    {
//      char name[9];
      unsigned dim = size(X);
      //assert(dim <= 10 && "Maximum 10 Variables supported. Increase array size for more!");
      core::arith::tvector tx(dim);
      for(unsigned i = 0; i < dim; i++) {

	//using namespace riot;
	//static unsigned number = 0;
	std::stringstream sstr;
	sstr << "autotm" << i;
	tx[i] = mid(X[i])+0.5*wid(X[i])*core::arith::taylormodel(sstr.str());


        //sprintf(name, "sivtree%d",i);
	//tx[i] = core::arith::create_tm_var(X[i]);
	  //tx[i] = core::arith::taylormodel(X[i]);
//      std::cout << tx[i] << " for " << X[i]  << " with name=" << name << std::endl;
      }
      if(normal) {
	const functions::IVFunction *nf = obj.normals();
	if(nf) {
	  return enclosure_cast<core::arith::ivector>(nf->operator()(tx));
	}
      }
      // std::cout << "TM eval: " << acf << std::endl;
      core::arith::interval cf = enclosure_cast<core::arith::interval>(obj.cf()(tx));
      // std::cout << "TM eval iv: " << cf << std::endl;
      return core::arith::ivector(1,cf);
    }

  }
}
