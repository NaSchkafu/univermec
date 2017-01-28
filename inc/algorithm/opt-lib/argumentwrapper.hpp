#include "stable.h"
/**
 * @file   argumentwrapper.hpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:02:51 2009
 * 
 * @brief Used for adjusting the number of arguments requested by a function
 * 
 * 
 */


#ifndef __ARGUMENTWRAPPER_HPP__
#define __ARGUMENTWRAPPER_HPP__ __ARGUMENTWRAPPER_HPP__

#include "exception.hpp"
#include "simplefunction.hpp"

namespace GlobOpt {

/// Wrapper for functions
/**
 * The ArgumentWrapper can used for adjusting the number of
 * arguments required by a function
 * 
 */
template<class F, class GRAD>
class ArgumentWrapper : public F {
public:
  ArgumentWrapper(const F &func, std::map<int, int> mappings, unsigned int dim)
    :F(func), _mappings(mappings), _dim(dim), _zero(core::arith::interval(0,0), dim)
  {

  }

  virtual const GRAD& gradF(unsigned int i) const
  {
    if(i > _dim)
      THROW(IllegalArguments, "Gradient member does not exist!");
    std::map<int, int>::const_iterator it = _mappings.find(i);
    if(it == _mappings.end()) {
      //THROW(IllegalArguments, "Value is not mapped!");
      return _zero;
    }
    return F::gradF(it->second);
  }

  virtual core::arith::ivector grad(const core::arith::ivector &) const
  {
    THROW(OpNotSupported, "Op not implemented!");
  }

  virtual core::arith::ivector grad(const core::arith::rvector &) const
  {
    THROW(OpNotSupported, "Op not implemented!");
  }

  virtual unsigned int dim() const { return _dim; }

  // for RenderFunction(s)
  virtual void render() { F::render(); }

  void addMapping(int oldArg, int newArg)  { _mappings[oldArg] = newArg; }

protected:
  // Function
  virtual core::arith::interval calc(const core::arith::ivector &args) const { return F::calc(mapArgs(args)); }
  virtual core::arith::interval calc(const core::arith::rvector &args) const { return F::calc(mapArgs(args)); }

  template<class V>
  core::arith::ivector mapArgs(const V& args) const {
    core::arith::ivector nArgs(1, F::dim());
    std::map<int, int>::const_iterator it;
    for(int i = 1; i <= Ub(args); i++) {
      it = _mappings.find(i);
      if(it == _mappings.end())
        THROW(IllegalArguments, "Value is not mapped!");
      nArgs[it->second] = args[i];
    }
    return nArgs;
  }

private:
  //F _F;
  std::map<int, int> _mappings;
  unsigned int _dim;
  ConstantFunction _zero;

};

}

#endif
