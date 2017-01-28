#ifndef __WEIGHTRATZ92_HPP__
#define __WEIGHTRATZ92_HPP__

#include "idirweight.hpp"

#include "functions/ifunction.hpp"
#include "functions/iffeatures.hpp"
#include "functions/iderivative.hpp"

namespace decomp 
{

  /// Multisection weighting rule Ratz92
  template <typename T>
  class WeightRatz92 : public IDirWeight<T> 
  {
  public:
    /** 
     * Ctor
     * Note: obj has to support IDerivative
     * 
     * @param obj objective function
     */
    WeightRatz92(const functions::IFunction &obj);
    virtual ~WeightRatz92() {}

  private:
    virtual split_lst_t weight_(const T& box) const;

  private:
    const functions::IFunction &m_obj;
    const functions::IDerivative *m_dobj;
  };

  #include "weightratz92.inl"

}

#endif /*__WEIGHTRATZ92_HPP__*/
