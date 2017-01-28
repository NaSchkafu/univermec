#ifndef __INTERVAL_UTIL_HPP__
#define __INTERVAL_UTIL_HPP__

#include <core/arithcore.hpp>

namespace core
{
  namespace arith
  {
    /*!
    ** Width of interval
    **
    ** @param i interval
    **
    ** @return width of i
    */
    // inline mreal w(const interval &i)
    // {
    //   return sup(i) - inf(i);
    // }

    /*!
    ** Width of widest component of an interval box
    **
    ** @param iv interval box
    **
    ** @return max{w(iv[i]) | i=0...size(iv)}
    */
    mreal wid(const ArithTraits<interval>::vec_t &iv);

    /*!
    ** mid of interval vector
    **
    ** @param iv interval vector
    **
    ** @return mid of iv (component wise)
    */
    ArithTraits<mreal>::vec_t mid(const ArithTraits<interval>::vec_t &iv);


    /*!
    ** Checks if an interval box is empty
    **
    ** @param iv box
    **
    ** @return true iff is_empty(iv[i]) is true for any 0 <= i < size(iv)
    */
    bool is_empty(const ArithTraits<interval>::vec_t &iv);


    /** 
     * Checks if a point p lies in an interval x
     * 
     * @param p point
     * @param x interval
     * 
     * @return true if \f$p \in \x\f$
     */
    inline bool operator<=(const mreal &p, const interval &x)
    {
      return inf(x) <= p && sup(x) >= p;
    }

    /*!
    ** inf of interval vector
    **
    ** @param iv interval vector
    **
    ** @return inf of iv (component wise)
    */
    ArithTraits<mreal>::vec_t inf(const ArithTraits<interval>::vec_t &iv); 

    /*!
    ** sup of interval vector
    **
    ** @param iv interval vector
    **
    ** @return mid of iv (component wise)
    */
    ArithTraits<mreal>::vec_t sup(const ArithTraits<interval>::vec_t &iv);

    /*!
    **
    ** @param x interval, y interval 
    **
    ** @return 1 if ture else 0 
    */
    int in (const interval &x, const interval &y);       

    /*!
    ** Inner Inclusion of two ivectors
    **
    ** @param a ivector, b ivector 
    **
    ** @return true
    */
    int in(const ArithTraits<interval>::vec_t &x, const ArithTraits<interval>::vec_t &y);

    /*! 
    ** 
    ** 
    ** @param iv 
    ** 
    ** @return midpoint matrix
    */
    ArithTraits<mreal>::mat_t mid(const ArithTraits<interval>::mat_t &iv);

    /*! 
    ** 
    ** 
    ** @param intervall 
    ** 
    ** @return intervall division
    */
    ArithTraits<interval>::vec_t ex_division(const interval &a, const interval &b);

    /*! 
    ** 
    ** 
    ** @param im 
    ** 
    ** @return inferior matrix
    */
    ArithTraits<mreal>::mat_t inf(const ArithTraits<interval>::mat_t &im);

    /*! 
    ** Equality operator for two interval vectors
    ** 
    ** @param v1 vector 1
    ** @param v2 vector 2
    ** 
    ** @return true if the vectors are component wise equal
    */
    bool operator==(const ArithTraits<interval>::vec_t &v1, 
		    const ArithTraits<interval>::vec_t &v2);

    /*! 
    ** Inequality operator for two interval vectors
    ** 
    ** @param v1 vector 1
    ** @param v2 vector 2
    ** 
    ** @return true if the vector are not component wise equal
    */
    inline bool operator!=(const ArithTraits<interval>::vec_t &v1, 
			   const ArithTraits<interval>::vec_t &v2)
    {
      return !(v1 == v2);
    }

    /*! 
    ** Square of interval vector v
    ** 
    ** @param v iv vector
    ** 
    ** @return v^2
    */
    ArithTraits<interval>::vec_t sqr(const ArithTraits<interval>::vec_t &v);

    interval dot(const ArithTraits<interval>::vec_t &v);

    // iostream op
    std::ostream& operator<<(std::ostream &os, const interval &i);

    ArithTraits<interval>::vec_t operator&(ArithTraits<interval>::vec_t v1,
    					   const ArithTraits<interval>::vec_t &v2);


    /** 
     * Checks if an interval box is contained by another interval box
     * (subset or equal)
     * 
     * @param x subbox
     * @param y parent box
     * 
     * @return true if x \subseteq y
     */
    bool operator<=(const ArithTraits<interval>::vec_t &x, const ArithTraits<interval>::vec_t &y);

    ArithTraits<interval>::vec_t operator|(const ArithTraits<interval>::vec_t &v1,
					   const ArithTraits<interval>::vec_t &v2);

  }
}

#endif /*__INTERVAL_HPP__*/
