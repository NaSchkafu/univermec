#ifndef __RATZSECTION_HPP__
#define __RATZSECTION_HPP__

#include "imultisection.hpp"
#include "ipointchooser.hpp"
#include "idirweight.hpp"

namespace decomp 
{
  template <typename T>
  class RatzSection : public IMultiSection<T> 
  {
  public:
    /** 
     * Constructs a new Ratz mutlisection scheme
     * 
     * @param weight weight for direction selection
     * @param chooser chooser for split point
     * @param threshold Coordinates with w(X) < threshold are ignored
     * 
     */
    RatzSection(IDirWeight<T> *weight, IPointChooser<T> *chooser,
		const core::arith::mreal &threshold = 0.0);

  private:
    virtual typename IMultiSection<T>::box_lst_t section_(const T &box) const;

  private:
    std::unique_ptr<IDirWeight<T> > m_weight;
    std::unique_ptr<IPointChooser<T> > m_chooser;
    core::arith::mreal m_thres;
  };
}

  #include "ratzsection.inl"

#endif /*__RATZSECTION_HPP__*/
