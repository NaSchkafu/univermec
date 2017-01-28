#ifndef __IBOXLISTMGR_HPP__
#define __IBOXLISTMGR_HPP__

#include <vector>

namespace decomp 
{
  template <typename T>
  class IBoxListMgr 
  {
  public:
    typedef std::vector<T*> box_lst_t;
    virtual ~IBoxListMgr() {}


    /** 
     * Adds a new element to the box manager
     * 
     * @param b element
     */
    inline void add(T* b)
      {
	add_(b);
      }
    
    /** 
     * Adds a list of elements to the box manager
     * 
     * @param lst list of elements
     */
    inline void add(const box_lst_t& lst)
      {
	add_(lst);
      }
    
    /** 
     * Gets an element from the manager
     * The element is removed
     * 
     * @return element
     */
    T* select()
      {
	return select_();
      }

    /** 
     * Gets the number of elements
     * 
     * 
     * @return number of element
     */
    unsigned count()
      {
	return count_();
      }

    /** 
     * Gets a copy of all elements
     * 
     * 
     * @return list containing all elements
     */
    box_lst_t list() const
      {
	return list_();
      }

    /** 
     * Removes all elements
     * The elements are not deleted!
     * 
     */
    void clear()
      {
	clear_();
      }

    /** 
     * Removes all elements
     * The elements are deleted!
     */
    void del_clear()
      {
	return del_clear_();
      }

    bool empty() const
      {
	return empty_();
      }

    // const box_lst_t& ref_list() const
    //   {
    // 	return ref_list_();
    //   }

  private:
    virtual void add_(T* b) = 0;
    virtual void add_(const box_lst_t &lst) = 0;
    virtual T* select_() = 0;
    virtual unsigned count_() const = 0;
    virtual box_lst_t list_() const = 0;
    virtual void clear_() = 0;
    virtual void del_clear_() = 0;
    virtual bool empty_() const = 0;
    //virtual const box_lst_t& ref_list_() const = 0;
  };
}


#endif /*__IBOXLISTMGR_HPP__*/
