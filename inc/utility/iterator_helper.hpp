#ifndef __ITERATOR_HELPER_HPP__
#define __ITERATOR_HELPER_HPP__

#include <boost/iterator/iterator_facade.hpp>

namespace utility
{

  template<class T>
  class tree_iterator : public boost::iterator_facade<tree_iterator<T>, T,
                                                      boost::forward_traversal_tag>
  {
  public:
    tree_iterator();
    explicit tree_iterator(T* base);

  private:
    friend class boost::iterator_core_access;
    void increment();
    bool equal(tree_iterator const& other) const;
    T& dereference() const;

  private:
    T* m_base;
    unsigned m_pos;
  };

  #include "iterator_helper.inl"

}


#endif /*__ITERATOR_HELPER_HPP__*/
