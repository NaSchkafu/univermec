// -*-c++-*-
#include "ratzsection.hpp"

#include <boost/foreach.hpp>


namespace decomp 
{
  template <typename T>
  RatzSection<T>::RatzSection(IDirWeight<T> *weight, IPointChooser<T> *chooser, 
			      const core::arith::mreal &thres)
    :m_weight(weight), m_chooser(chooser), m_thres(thres)
  {
    
  }

  template <typename T>
  typename IMultiSection<T>::box_lst_t RatzSection<T>::section_(const T &box) const
  {
    typename IMultiSection<T>::box_lst_t ret;

    // Gewichten
    split_lst_t &&splits = m_weight->weight(box);
    m_chooser->pchoose(box, splits, splits.size());
    sort(splits.begin(), splits.end());
    if(m_thres > 0) {
      auto it(splits.begin());
      while(it != splits.end()) {
	if(wid(box.ref_X()[it->dir]) < m_thres) {
	  it = splits.erase(it);
	}
	else
	  ++it;
      }
    }
    unsigned size(splits.size());

    //std::cout << &box << std::endl;
    T* tmp = new T(box);
    //std::cout << tmp << std::endl;

    for(unsigned i = 0; i < size; ++i) {
      unsigned short dir = splits[i].dir;
      const core::arith::interval &spoint = splits[i].point;
      // std::cout << "Splitte Box " << tmp->ref_X() << " an " 
      // 		<< dir << " und Punkt " <<spoint << std::endl;
      T *x = tmp->bisect(dir, spoint);
      core::arith::mreal wx(wid(x->ref_X()));
      core::arith::mreal wtmp(wid(tmp->ref_X()));

      // Beide leer
      if(wx < 0 && wtmp < 0)
	break;

      // Größere Box speichern      
      if(wx >= wtmp) {
	ret.push_back(x);
	// std::cout << "Füge hinzu " << x->ref_X() << std::endl;
      }
      else {
	ret.push_back(tmp);
	// std::cout << "Füge hinzu " << tmp->ref_X() << std::endl;
	tmp = x;
	wtmp = wx;
      }
      
      // Abbruch falls Box leer ist
      if(wtmp < 0)
	break;
     
    }

    if(!core::arith::is_empty(tmp->ref_X())) {
      ret.push_back(tmp);
#ifdef DEBUG_OUTPUT
      std::cout << "Füge hinzu: " << tmp.X() << std::endl
		<< " Geaddet: " << genBoxes << std::endl
		<< "****************************************************************" << std::endl;
#endif
    }

    // std::cout << "Ratz Split: " << std::endl;
    // std::cout << "Original: " << box.X() << std::endl;
    // BOOST_FOREACH(T *r, ret){
    //   std::cout << r->X() << " @" << r << std::endl;
    // }
    // std::cout << " **************** " << std::endl;

    return ret;
  }
}
