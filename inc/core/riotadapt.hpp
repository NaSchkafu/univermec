#include "stable.h"
#ifndef __RIOTADAPT_HPP__
#define __RIOTADAPT_HPP__

/**
 * @file   riotadapt.hpp
 * @author Stefan Kiel <fluxy@lucy>
 * @date   Fri Aug 13 13:35:11 2010
 * 
 * @brief  Adapter class for Taylor Models (RiOt)
 * 
 * 
 */

#include <taylormodel.h>

namespace core 
{
  namespace arith 
  {
    class taylormodel
    {
    public:
      explicit taylormodel(mreal d)
	:m_taylormodel(riot::TaylorModel::ZERO_TM())
	{
	  m_taylormodel += to_double(d);
	}
      
      explicit taylormodel(const interval &i) 
	:m_taylormodel(riot::TaylorModel::ZERO_TM())
	{
	  m_taylormodel += Interval(to_double(inf(i)), to_double(sup(i)));
	}

      explicit taylormodel(double d)
	:m_taylormodel(riot::TaylorModel::ZERO_TM())
	{
	  m_taylormodel += d;
	}

      taylormodel(mreal lb, mreal ub)
	:m_taylormodel(riot::TaylorModel::ZERO_TM())
	{
	  m_taylormodel += Interval(to_double(lb), to_double(ub));
	}

      taylormodel(const std::string vname, double dpoint, const interval &domain)
	:m_taylormodel(vname, dpoint, Adapt::Interval(to_double(inf(domain)), 
						      to_double(sup(domain))))
	{
	}
      
      taylormodel(const std::string vname)
	:m_taylormodel(vname, 0.0, Adapt::Interval(-1, 1))
	{
	}

      taylormodel()
	:m_taylormodel(riot::TaylorModel::ZERO_TM())
	{
	}

      // taylormodel
      inline taylormodel& operator+=(const taylormodel& other) 
	{
	  m_taylormodel += other.m_taylormodel;
	  return *this;
	}

      inline taylormodel& operator-=(const taylormodel& other) 
	{
	  m_taylormodel -= other.m_taylormodel;
	  return *this;
	}

      inline taylormodel& operator*=(const taylormodel& other) 
	{
	  m_taylormodel *= other.m_taylormodel;
	  return *this;
	}

      inline taylormodel& operator/=(const taylormodel& other) 
	{
	  m_taylormodel /= other.m_taylormodel;
	  return *this;
	}

      // double 
      inline taylormodel& operator+=(mreal d) 
	{
	  m_taylormodel += to_double(d);
	  return *this;
	}

      inline taylormodel& operator-=(mreal d) 
	{
	  m_taylormodel -= to_double(d);
	  return *this;
	}

      inline taylormodel& operator*=(mreal d) 
	{
	  m_taylormodel *= to_double(d);
	  return *this;
	}

      inline taylormodel& operator/=(mreal d) 
	{
	  m_taylormodel /= to_double(d);
	  return *this;
	}

      inline taylormodel& operator=(mreal d) 
	{
	  m_taylormodel = riot::TaylorModel::ZERO_TM();
	  m_taylormodel += to_double(d);
	  return *this;
	}


      // interval
      inline taylormodel& operator+=(const interval& i) 
	{
	  m_taylormodel += i.m_adapt;
	  return *this;
	}

      inline taylormodel& operator-=(const interval& i) 
	{
	  m_taylormodel -= i.m_adapt;
	  return *this;
	}

      inline taylormodel& operator*=(const interval& i) 
	{
	  m_taylormodel *= i.m_adapt;
	  return *this;
	}

      inline taylormodel& operator/=(const interval& i) 
	{
	  m_taylormodel /= i.m_adapt;
	  return *this;
	}

      friend taylormodel operator-(const taylormodel &t);
      friend taylormodel operator+(const taylormodel &t);
      friend taylormodel sqr(const taylormodel &t);
      friend taylormodel pow(const taylormodel &t, int n);
      friend taylormodel pow(const taylormodel &t, const mreal &e);
      friend taylormodel pow(const taylormodel &t, const taylormodel &e);
      friend taylormodel sin(const taylormodel &t);
      friend taylormodel cos(const taylormodel &t);
      friend taylormodel sqrt(const taylormodel &t);
      friend taylormodel ln(const taylormodel &t);
      friend taylormodel exp(const taylormodel &t);

      friend bool operator==(const taylormodel &t1, const taylormodel &t2);
      friend bool operator!=(const taylormodel &t1, const taylormodel &t2);

      inline interval encl() const 
	{
	  Adapt::Interval i(m_taylormodel.eval());
	  return interval(mreal(i.inf()), mreal(i.sup()));
	}

      std::ostream& print(std::ostream &os) const;

      enum TM_BOUNDER { INTERVAL, MEAN_VALUE, LDB };
      static void set_order(unsigned order);
      static unsigned order();
      static void set_bounder(TM_BOUNDER bounder);

    private:
      taylormodel(const riot::TaylorModel &t)
	:m_taylormodel(t)
	{
	}

      static bool init();

    private:
      riot::TaylorModel m_taylormodel;
      typedef void trait_is_tm_t;
      static bool force_init;
    };

    // taylormodel op taylormodel
    inline taylormodel operator+(taylormodel taylormodel1, const taylormodel& taylormodel2) 
    {
      return taylormodel1 += taylormodel2;
    }

    inline taylormodel operator-(taylormodel taylormodel1, const taylormodel& taylormodel2) 
    {
      return taylormodel1 -= taylormodel2;
    }

    inline taylormodel operator*(taylormodel taylormodel1, const taylormodel& taylormodel2) 
    {
      return taylormodel1 *= taylormodel2;
    }

    inline taylormodel operator/(taylormodel taylormodel1, const taylormodel& taylormodel2) 
    {
      return taylormodel1 /= taylormodel2;
    }

    // taylormodel <op> double
    inline taylormodel operator+(taylormodel taylormodel1, mreal d) 
    {
      return taylormodel1 += d;
    }

    inline taylormodel operator-(taylormodel taylormodel1, mreal d) 
    {
      return taylormodel1 -= d;
    }

    inline taylormodel operator*(taylormodel taylormodel1, mreal d) 
    {
      return taylormodel1 *= d;
    }

    inline taylormodel operator/(taylormodel taylormodel1, mreal d) 
    {
      return taylormodel1 /= d;
    }

    // double <op> taylormodel
    inline taylormodel operator+(mreal d, const taylormodel &taylormodel1) 
    {
      return taylormodel(d) += taylormodel1;
    }

    inline taylormodel operator-(mreal d, const taylormodel &taylormodel1) 
    {
      return taylormodel(d) -= taylormodel1;
    }

    inline taylormodel operator*(mreal d, const taylormodel &taylormodel1) 
    {
      return taylormodel(d) *= taylormodel1;
    }

    inline taylormodel operator/(mreal d, const taylormodel &taylormodel1) 
    {
      return taylormodel(d) /= taylormodel1;
    }



    // taylormodel <op> interval
    inline taylormodel operator+(taylormodel taylormodel1, const interval &i) 
    {
      return taylormodel1 += i;
    }

    inline taylormodel operator-(taylormodel taylormodel1, const interval &i) 
    {
      return taylormodel1 -= i;
    }

    inline taylormodel operator*(taylormodel taylormodel1, const interval &i) 
    {
      return taylormodel1 *= i;
    }

    inline taylormodel operator/(taylormodel taylormodel1, const interval &i)
    {
      return taylormodel1 /= i;
    }



    // interval <op> taylormodel
    inline taylormodel operator+(const interval &i, const taylormodel &taylormodel1) 
    {
      return taylormodel(i) += taylormodel1;
    }

    inline taylormodel operator-(const interval &i, const taylormodel &taylormodel1) 
    {
      return taylormodel(i) -= taylormodel1;
    }

    inline taylormodel operator*(const interval &i, const taylormodel &taylormodel1) 
    {
      return taylormodel(i) *= taylormodel1;
    }

    inline taylormodel operator/(const interval &i, const taylormodel &taylormodel1) 
    {
      return taylormodel(i) /= taylormodel1;
    }


    inline taylormodel sqr(const taylormodel &t) 
    {
      return sqr(t.m_taylormodel);
    }
    
    inline taylormodel cos(const taylormodel &t)
    {
      return cos(t.m_taylormodel);
    }
    
    inline taylormodel sin(const taylormodel &t)
    {
      return sin(t.m_taylormodel);
    }
    
    inline taylormodel pow(const taylormodel &t, int n)
    {
      return power(t.m_taylormodel, n);
    }
    
    inline taylormodel pow(const taylormodel &t, const taylormodel &e)
    {
      return taylormodel(pow(t.m_taylormodel, e.m_taylormodel));
    }

    inline taylormodel pow(const taylormodel &t,const mreal& e) 
    {
      riot::TaylorModel te(riot::TaylorModel::ZERO_TM()+to_double(e));
      return taylormodel(pow(t.m_taylormodel, te));
    }
    

    inline taylormodel operator-(const taylormodel &t)
    {
      taylormodel t1(-t);
      return t1;
    }
    
    inline taylormodel operator+(const taylormodel &t) 
    {
      return t;
    }

    inline taylormodel sqrt(const taylormodel &t) 
    {
      return sqrt(t.m_taylormodel);
    }
    
    inline taylormodel ln(const taylormodel &t)
    {
      return ln(t.m_taylormodel);
    }
    
    inline taylormodel exp(const taylormodel &t)
    {
      return exp(t.m_taylormodel);
    }

    inline bool operator==(const taylormodel &t1, const taylormodel &t2)
    {
      return t1.m_taylormodel == t2.m_taylormodel;
    }
    
    inline bool operator!=(const taylormodel &t1, const taylormodel &t2)
    {
      return !(t1 == t2);
    }
    
    /** 
     * Creates a Taylor model representing an independent variable 
     * 
     * @param i Taylor model domain
     * @param vname variable name
     * 
     * @return Taylor model
     */
    inline taylormodel create_tm_var(const interval &i, const std::string &vname) 
    {
      return mid(i)+0.5*diam(i)*taylormodel(vname);
    }

    taylormodel create_tm_var(const interval &i);

    std::ostream& operator<<(std::ostream &os, const taylormodel &t);
  }
}
#endif /*__RIOTADAPT_HPP__*/
