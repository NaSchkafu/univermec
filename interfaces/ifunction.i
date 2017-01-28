%{
  #include "stable.h"
  #include "ifunction.hpp"
%}


%nodefaultctor functions::IFunction;

namespace functions
{
  class IFFeatures;
  class IFunction
  {
  public:
    core::arith::interval operator()(const core::arith::ivector& args) const;
    
    core::arith::mreal operator()(const core::arith::rvector &args) const;
    core::arith::aaf operator()(const core::arith::avector &args) const;
    
    core::arith::taylormodel operator()(const core::arith::tvector &args) const;
    unsigned dim() const;
    
    
    std::string text() const;
    IFunction* clone() const;
    
    const IFFeatures& features() const ;
    
    virtual ~IFunction() { }

//  protected:
//    IFunction()  { }
  };
}
