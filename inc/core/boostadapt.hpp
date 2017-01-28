#include "stable.h"
#ifndef BOOSTADAPT_HPP
#define BOOSTADAPT_HPP

#include <iosfwd>

#include <boost/numeric/interval.hpp>



namespace core
{
  namespace arith
  {

   // TODO: fixme Interferenz mit MTL
    class interval /*: boost::field_operators<interval, boost::field_operators<interval, real,
                     boost::equality_comparable<interval> > >, boost::orable<interval>*/
    {
    public:
      // ctors
      interval()
        :m_i(0.0, 0.0)
        {}

      explicit interval(real d)
        :m_i(d, d)
        { }

      interval(real inf, real sup)
        :m_i(inf, sup)
        { }

      interval(const interval &other)
        :m_i(other.m_i)
        { }

      // assigment operator
      inline interval& operator=(const interval &other)
        {
          m_i = other.m_i;
          return *this;
        }

      inline interval& operator=(real d)
        {
          m_i = d;
          return *this;
        }

      // TODO: Der verwendete Intervalltyp ist nicht vollkommen verifiziert
      // siehe boost::interval Dokumenation und trans.cpp in den Beispielen für Details
      typedef boost::numeric::interval<double, boost::numeric::interval_lib::policies<boost::numeric::interval_lib::save_state<boost::numeric::interval_lib::rounded_transc_opp<double> >, boost::numeric::interval_lib::checking_strict<double> > > iv_t;

// ************************************************************************************************
// Arithmetik-Konzept
// ************************************************************************************************

      // interval - interval
      inline interval& operator+=(const interval &i)
        {
          m_i += i.m_i;
          return *this;
        }

      inline interval& operator-=(const interval &i)
        {
          m_i -= i.m_i;
          return *this;
        }

      inline interval& operator*=(const interval &i)
        {
          m_i *= i.m_i;
          return *this;
        }

      inline interval& operator/=(const interval &i)
        {
          m_i /= i.m_i;
          return *this;
        }

      // interval - scalar
      inline interval& operator+=(real d)
        {
          m_i += d;
          return *this;
        }

      inline interval& operator-=(real d)
        {
          m_i -= d;
          return *this;
        }

      inline interval& operator*=(real d)
        {
          m_i *= d;
          return *this;
        }

      inline interval& operator/=(real d)
        {
          m_i /= d;
          return *this;
        }

      inline const interval operator-() const
        {
          interval result(*this);
          result.m_i = -m_i;
          return result;
        }

      inline const interval operator+() const
        {
          interval result(*this);
          return result;
        }

      // compare operators
      inline bool operator==(const interval &other) const
        {
          return m_i == other.m_i;
        }

      inline bool operator!=(const interval &other) const
        {
          return !(*this == other);
        }

      inline interval operator&=(const interval &other)
        {
         m_i = intersect(m_i, other.m_i);
         return *this;
        }


// ************************************************************************************************
      // member access
      inline real inf() const
        {
          return lower(m_i);
        }

      inline real sup() const
        {
          return upper(m_i);
        }

      // extra operators
      // Huelle
      inline interval& operator|=(const interval &i)
        {
          m_i = hull(m_i, i.m_i);
          return *this;
        }

      inline void set_sup(real d)
        {
         m_i.set(m_i.lower(), d);
        }

      inline void set_inf(real d)
        {
          m_i.set(d, m_i.upper());
        }



      // Friends
      friend bool operator<=(real, const interval&);
      friend real inf(const interval &);
      friend real sup(const interval &);
      friend interval sqr(const interval &);
      friend real mid(const interval &);
      friend interval iv_pi();
      friend interval sin(const interval&);
      friend interval cos(const interval&);
      friend interval abs(const interval&);
      friend interval sqrt(const interval&);
      friend interval pow(const interval&, int);
      friend bool isEmpty(const interval&);

    private:
      iv_t m_i;
      // Eigentlich soll nur aaf::encl() Zugriff haben wg. Konvertierung
      // TODO: Entfernen sobald AAf Bib ordentlich eingebunden!
      friend class aaf;

    private:
      explicit interval(const iv_t &i)
        :m_i(i)
        {}
    };

    // skal/iv
    inline interval operator+(double d, interval i)
    {
      return i += d;
    }

    inline interval operator-(double d, interval i)
    {
      return i -= d;
    }

    inline interval operator*(double d, interval i)
    {
      return i *= d;
    }

    inline interval operator/(double d, interval i)
    {
      return i /= d;
    }

    // iv/skal
    inline interval operator+(interval i, double d)
    {
      return i += d;
    }
    inline interval operator-(interval i, double d)
    {
      return i -= d;
    }

    inline interval operator*(interval i, double d)
    {
      return i *= d;
    }

    inline interval operator/(interval i, double d)
    {
      return i /= d;
    }

//     //Fall d,i nicht.
//     inline interval operator&=(interval i, double d)
//     {
// 	return i &= d;
//     }

//     inline interval operator&(interval i, double d)
//     {
// 	return i &= d;
//     }


    // iv
    inline interval operator+(interval i, const interval &i2)
    {
      return i += i2;
    }
    inline interval operator-(interval i, const interval &i2)
    {
      return i -= i2;
    }

    inline interval operator*(interval i, const interval &i2)
    {
      return i *= i2;
    }

    inline interval operator/(interval i, const interval &i2)
    {
      return i /= i2;
    }

    inline interval operator&(interval i1, const interval &i2)
    {
      return i1 &= i2;
    }

    inline interval operator|(interval i1, const interval &i2)
    {
      return i1 |= i2;
    }

    // element of
    inline bool operator<=(real d, const interval &i)
    {
      return d >= inf(i) && d <= sup(i);
    }

    // iostream op
    inline std::ostream& operator<<(std::ostream &os, const interval &i)
    {
      os << "[" << inf(i) << " , " << sup(i) << "]";
      return os;
    }

    inline real inf(const interval &i)
    {
      return i.inf();
    }

    inline real sup(const interval &i)
    {
      return i.sup();
    }

    inline interval sqr(const interval &i)
    {
        return interval(boost::numeric::square(i.m_i));
    }

    inline real mid(const interval &i)
    {
      return median(i.m_i);
    }

    inline interval iv_pi()
    {
        return interval(boost::numeric::interval_lib::pi<boost::numeric::interval<double> >());
    }

    inline interval sin(const interval &arg)
    {
        return interval(boost::numeric::sin(arg.m_i));
    }

    inline interval cos(const interval &arg)
    {
        return interval(boost::numeric::cos(arg.m_i));
    }

    inline interval abs(const interval &arg)
    {
        return interval(boost::numeric::abs(arg.m_i));
    }

    inline interval sqrt(const interval &arg)
    {
        return interval(boost::numeric::sqrt(arg.m_i));
    }

    inline interval pow(const interval &b, int e)
    {
        return interval(boost::numeric::pow(b.m_i, e));
    }



  } // namespace arith
} // namespace core



#endif // BOOSTADAPT_HPP
