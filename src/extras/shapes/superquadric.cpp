#include <extras/shapes/superquadric.hpp>

#include "functions/details/fderiv.hpp"
#include "functions/details/fvbase.hpp"

using namespace core::arith;
using namespace std;
namespace shapes
{
  namespace details
  {

    template<bool BEND>
    struct SQImpl
    {
      SQImpl(mreal rx, mreal ry, mreal rz, int eps1_p, unsigned eps1_q,
             int eps2_p, unsigned eps2_q)
        :m_rx(rx), m_ry(ry), m_rz(rz), m_eps1_p(eps1_p), m_eps2_p(eps2_p), m_eps1_q(eps1_q),
         m_eps2_q(eps2_q)
        {
        }

      SQImpl(mreal rx, mreal ry, mreal rz, int eps1_p, unsigned eps1_q,
             int eps2_p, unsigned eps2_q, mreal kbend, mreal alpha)
        :m_rx(rx), m_ry(ry), m_rz(rz), m_eps1_p(eps1_p), m_eps2_p(eps2_p), m_eps1_q(eps1_q),
         m_eps2_q(eps2_q), m_kbend(kbend), m_alpha(alpha)
        {
        }



      unsigned dim() const
        {
          return 3;
        }

      std::string text() const
        {
          std::stringstream sstream;
          sstream << "Superquadric (rx=" << m_rx << ", ry=" << m_ry << ", rz=" << m_rz;
          sstream << ", eps1=" << m_eps1_p << "/" << m_eps1_q << ", eps2=" << m_eps2_p << "/"
                  << m_eps2_q;
          return sstream.str();
        }

      // Standardbend
      // TODO: kbend als IV
      template<typename T>
      bool do_bend(T& args) const
        {
//        ivector iargs(ArithTraits<ivector>::force_convert(args));
//        typedef typename ArithTraits<T>::base_t base_t;
//        base_t cosyx(interval(-1,1));
//        if(!(0 <= ArithTraits<interval>::force_convert(args[0])))
//          cosyx = cos(m_alpha - arctan(args[1]/args[0]));
//        else
//          return false;
//        // std::cout << "****************************************************************" << std::endl;
//        // std::cout << "cosyx Eingang: " << w(iargs[1]) + w(iargs[0])
//        //        << " Ausgang: " << w(ArithTraits<interval>::force_convert(cosyx)) << std::endl;

//           base_t R(sqrt(sqr(args[0])+sqr(args[1]))*cosyx);
//        // std::cout << "R Eingang: " << w(iargs[1]) + w(iargs[0]) + w(ArithTraits<interval>::force_convert(cosyx))
//        //        << " Ausgang: " << w(ArithTraits<interval>::force_convert(R))  << std::endl;

//           base_t r(1/m_kbend - sqrt(sqr(args[2])+sqr(1/m_kbend- R)));
//        // std::cout << "r Eingang: " << w(iargs[2]) + w(ArithTraits<interval>::force_convert(R))
//        //        << " Ausgang: " << w(ArithTraits<interval>::force_convert(r))  << std::endl;

//           base_t invkr(1/m_kbend - R);
//        // std::cout << "invkr Eingang: " << w(ArithTraits<interval>::force_convert(R))
//        //        << " Ausgang: " << w(ArithTraits<interval>::force_convert(invkr))  << std::endl;

//           if(0 <= ArithTraits<interval>::force_convert(invkr))
//             return false;

//        // std::cout << "arg"  << args[2] << std::endl;
//        // std::cout << "invkr" << invkr << std::endl;
//        // std::cout << "/" << args[2]/invkr << std::endl;
//        //std::cout << args[2] << "/" << invkr << std::endl;
//        base_t as(args[2]/invkr);
//        interval ias(iargs[2]/ArithTraits<interval>::force_convert(invkr));
//        // std::cout << "as Eingang: " << w(iargs[2]) + w(ArithTraits<interval>::force_convert(invkr))
//        //        << " Ausgang: " << w(ArithTraits<interval>::force_convert(as))  << " IV: "
//        //        << w(ias) << std::endl;

//           base_t gamma(arctan(as));
//        // std::cout << "gamma Eingang: " << w(ArithTraits<interval>::force_convert(as))
//        //        << " Ausgang: " << w(ArithTraits<interval>::force_convert(gamma))  << std::endl;
//        //std::cout << gamma << std::endl;
//           args[0] = args[0] - (R - r)*cos(m_alpha);
//        // std::cout << "arg1 Eingang: " << w(iargs[0]) + w(ArithTraits<interval>::force_convert(R)) + w(ArithTraits<interval>::force_convert(r))
//        //        << " Ausgang: " << w(ArithTraits<interval>::force_convert(args[0]))  << std::endl;

//           args[1] = args[1] - (R - r)*sin(m_alpha);
//        // std::cout << "arg1 Eingang: " << w(iargs[1]) + w(ArithTraits<interval>::force_convert(R)) + w(ArithTraits<interval>::force_convert(r))
//        //        << " Ausgang: " << w(ArithTraits<interval>::force_convert(args[1]))  << std::endl;

//           args[2] = gamma/m_kbend;

// /*
//        std::cout << "================================================================" << std::endl;
//        std::cout << ArithTraits<interval>::force_convert(iargs[0]) << " "
//                  << ArithTraits<interval>::force_convert(iargs[1]) << " "
//                  << ArithTraits<interval>::force_convert(iargs[2]) << std::endl;

//        std::cout << w(ArithTraits<interval>::force_convert(iargs[0])) << " "
//                  << w(ArithTraits<interval>::force_convert(iargs[1])) << " "
//                  << w(ArithTraits<interval>::force_convert(iargs[2])) << std::endl;

//        std::cout << " iyx: " << setw(8) << w(cos(m_alpha - arctan(iargs[1]/iargs[0])))
//                  << " iR : " << setw(8) << w(sqrt(sqr(iargs[0])+sqr(iargs[1]))*cos(m_alpha - arctan(iargs[1]/iargs[0])))
//                  << " ir : " << setw(8) << w(1/m_kbend - sqrt(sqr(iargs[2])+sqr(1/m_kbend- sqrt(sqr(iargs[0])+sqr(iargs[1]))*cos(m_alpha - arctan(iargs[1]/iargs[0])))))
//                  << " ikr: " << setw(8) << w(sqrt(sqr(iargs[0])+sqr(iargs[1])))
//                  << " iz: "  << setw(8) << w(iargs[2]/sqrt(sqr(iargs[0])+sqr(iargs[1])))
//                  << " ig : " << setw(8) << w(arctan(iargs[2]/sqrt(sqr(iargs[0])+sqr(iargs[1])))) << std::endl;

//        std::cout << setprecision(6) << " ayx: " << setw(8) << w(ArithTraits<interval>::force_convert(cosyx))
//                  << " aR : " << setw(8) << w(ArithTraits<interval>::force_convert(R))
//                  << " ar : " << setw(8) << w(ArithTraits<interval>::force_convert(r))
//                  << " akr: " << setw(8) << w(ArithTraits<interval>::force_convert(invkr))
//                  << " az : " << setw(8) << w(ArithTraits<interval>::force_convert(as))
//                  << " ag : " << setw(8) << w(ArithTraits<interval>::force_convert(gamma)) << std::endl;

//        std::cout << setprecision(6) << " ayx: " << setw(8) << ArithTraits<interval>::force_convert(cosyx)
//                  << " aR : " << setw(8) << ArithTraits<interval>::force_convert(R)
//                  << " ar : " << setw(8) << ArithTraits<interval>::force_convert(r)
//                  << " akr: " << setw(8) << ArithTraits<interval>::force_convert(invkr)
//                  << " az:  " << setw(8) << ArithTraits<interval>::force_convert(as)
//                  << " ag : " << setw(8) << ArithTraits<interval>::force_convert(gamma) << std::endl;

//                  std::cout << "===================================================
          // =============" << std::endl;*/
          return true;
        }

      // Wertet y/x mit arctan aus bei Verwendung erweiterter Arith
      /*void eval_arctan(ivector &args) const
        {
        for(unsigned i = 0; i < size(args) && !isEmpty(args[i]); i++) {
        if(inf(args[i]) == -infinity() && sup(args[i]) == infinity())
        args[i] = interval(-inf(iv_pi()/2), sup(iv_pi()/2));
        else if(sup(args[i]) == infinity())
        args[i] = interval(inf(arctan(args[i])), sup(iv_pi()/2));
        else if(inf(args[i]) == -infinity())
        args[i] = interval(inf(-iv_pi()/2), sup(arctan(args[i])));
        else
        args[i] = arctan(args[i]);
        }
        }
      */

      template<typename T>
      typename T::value_type eval(const T& args) const
        {
          // if(m_eps1 == 0.0)
          //   return pow(sqr(args[0]/m_rx),1/m_eps2)+pow(sqr(args[1]/m_ry),1/m_eps2)+(args[2]/m_rz) - 2;
          // else if(m_eps2 == 0.0)
          //   return pow(sqr(args[0]/m_rx),1/m_eps1)+pow(sqr(args[1]/m_ry),1/m_eps1) + 2*pow(sqr(args[2]/m_rz),1/m_eps1)-2;
          // TODO: Fix Grenfälle
          mreal eps1 = ((double)m_eps1_p)/m_eps1_q;
          mreal eps2 = ((double)m_eps2_p)/m_eps2_q;
          //std::cout << eps1 << " " << eps2 << std::endl;
          typename T::value_type w1(pow(pow(sqr(args[0]/m_rx), 1/eps2) + pow(sqr(args[1]/m_ry), 1/eps2),
                                        eps2/eps1) + pow(sqr(args[2]/m_rz), 1/eps1) - 1);
          return w1;
        }



      // template<typename T>
      // interval bend(T) const
      //   {
      //     std::cerr << "Bending ist nur fuer Intervalle erlaubt!";
      //     return interval(-1,1);
      //   }

      template <typename T>
      typename T::value_type bend(T args) const 
	{
	  typedef typename T::value_type base_t;
	  base_t R(args[0]*cos(base_t(m_alpha)) + args[1]*sin(base_t(m_alpha)));	  
	  
	  base_t Rminus(1.0/m_kbend - R);
	  // TODO: Ableitung im Falle von Rangetypen ist nicht korrekt
	  // !!! Nicht verwenden !!!
	  core::arith::interval Rminus_iv(enclosure_cast<core::arith::interval>(Rminus));
	  if(sup(Rminus_iv) < 0.0)
	    return forced_cast<base_t>(core::arith::mreal(std::numeric_limits<double>::quiet_NaN()));
	  else if(inf(Rminus_iv) < 0.0) {
	    Rminus_iv &= core::arith::interval(0, core::arith::infinity());
	    Rminus = forced_cast<base_t>(Rminus_iv);
	  }
	  base_t r(1.0/m_kbend - sqrt(sqr(Rminus) + sqr(args[2])));
	  
	  args[0] -= (R-r)*cos(base_t(m_alpha));
	  args[1] -= (R-r)*sin(base_t(m_alpha));
	  base_t denom(1.0/m_kbend - r);
	  if(!(0 <= enclosure_cast<core::arith::interval>(denom))) {
	    denom = args[2] / denom;
	    if(inf(enclosure_cast<core::arith::interval>(denom)) >= -1.0 &&
	       sup(enclosure_cast<core::arith::interval>(denom)) <= 1.0) {
	      args[2] = 1.0/m_kbend*asin(denom);
	      return eval(args);
	    }
	  }
	  return forced_cast<base_t>(core::arith::interval(-core::arith::infinity(), 
							   core::arith::infinity()));
	}

      core::arith::mreal bend(core::arith::rvector args) const 
	{
	  core::arith::mreal R(args[0]*cos(core::arith::mreal(m_alpha)) + args[1]*sin(core::arith::mreal(m_alpha)));
	  if(1.0/m_kbend - R < 0.0)
	    return 1.0;
	  //std::cout << "R " << R << std::endl;
	  core::arith::mreal r(1.0/m_kbend - sqrt(sqr(1.0/m_kbend - R) + sqr(args[2])));
	  //std::cout << "r " << r << std::endl;
	  args[0] = args[0] - (R-r)*cos(core::arith::mreal(m_alpha));
          //std::cout << "args[0] " << args[0] << std::endl;
	  args[1] = args[1] - (R-r)*sin(core::arith::mreal(m_alpha));
	  //std::cout << "args[1] " << args[1] << std::endl;
	  //std::cout << "asin: " << args[2] / (1.0/m_kbend - r) << " "
	  //<<  asin(args[2] / (1.0/m_kbend - r)) << std::endl;
	  	  
	  args[2] = 1.0/m_kbend*asin(args[2] / (1.0/m_kbend - r));
	  //std::cout << "args[2] " << args[2] << std::endl;
	  return eval(args);
	}


       template<typename T>
       typename boost::enable_if_c<BEND, typename T::value_type>::type operator()(const T& args) const
	{
	  return bend(args);
	}
      
      template<typename T>
      typename boost::disable_if_c<BEND, typename T::value_type>::type operator()(const T& args) const
        {
          return eval(args);
        }


      mreal m_rx, m_ry, m_rz;
      int m_eps1_p, m_eps2_p;
      unsigned m_eps1_q, m_eps2_q;
      mreal m_kbend, m_alpha;
      bool m_imp_aaf;
      bool m_bended;
    };


    //template<int IDX>
    struct SQParamImpl
    {
      SQParamImpl(mreal rx, mreal ry, mreal rz, int eps1_p, unsigned eps1_q,
                  int eps2_p, unsigned eps2_q)
        :m_rx(rx), m_ry(ry), m_rz(rz), m_eps1_p(eps1_p), m_eps2_p(eps2_p), m_eps1_q(eps1_q),
         m_eps2_q(eps2_q)
        { 
	  m_eps1 = (double)m_eps1_p/(double)m_eps1_q;
	  // Fixed: Hier stand eps1_p
	  m_eps2 = (double)m_eps2_p/(double)m_eps2_q;
	}

      interval eval_x(const ivector& args, const mreal& sgn) const
        {
	  // core::arith::interval res1(pow(cos(args[0]) & core::arith::interval(0, 1.0), m_eps1));
	  // core::arith::interval res2(pow(sgn*(cos(args[1]) & sgn*core::arith::interval(0, 1.0)), m_eps2));
	  // std::cout << m_rx*pow(cos(args[0]) & core::arith::interval(0, 1.0), m_eps1) << std::endl;
	  // std::cout << pow(sgn*(cos(args[1]) & sgn*core::arith::interval(0, 1.0)), m_eps2) << std::endl;
          return m_rx*sgn*pow(cos(args[0]) & core::arith::interval(0, 1.0), m_eps1)
	    *pow(sgn*(cos(args[1]) & sgn*core::arith::interval(0, 1.0)), m_eps2);
        }

      interval eval_y(const ivector& args, const mreal& sgn) const
        {
          return m_ry*sgn*pow(cos(args[0]) & core::arith::interval(0, 1.0), m_eps1)
	   *pow(sgn*(sin(args[1]) & sgn*core::arith::interval(0, 1.0)), m_eps2);
        }

      interval eval_z(const ivector& args, const mreal& sgn) const
        {
          return m_rz*sgn*pow(sgn*(sin(args[0]) & sgn*core::arith::interval(0, 1.0)), m_eps1);
        }


      template <typename T>
      typename T::value_type eval_x(const T& args, const mreal& sgn) const
        {
	  // std::cout << m_rx*pow(cos(args[0]), m_eps1) << std::endl;
	  // std::cout << pow(sgn*cos(args[1]), m_eps2) << std::endl;
          return m_rx*pow(cos(args[0]), m_eps1)*pow(sgn*cos(args[1]), m_eps2);
        }

      template <typename T>
      typename T::value_type eval_y(const T& args, const mreal& sgn) const
        {
          return m_ry*pow(cos(args[0]), m_eps1)*pow(sgn*sin(args[1]), m_eps2);
        }

      template <typename T>
      typename T::value_type eval_z(const T& args, const mreal& sgn) const
        {
          return m_rz*pow(sgn*sin(args[0]), m_eps1);
        }

      unsigned dim() const
	{
	  return 2;
	}

      unsigned cdim() const
	{
	  return 3;
	}

      std::string text() const
        {
          std::stringstream sstream;
          sstream << "Parametric Superquadric (rx=" << m_rx << ", ry=" << m_ry << ", rz=" << m_rz;
          sstream << ", eps1=" << m_eps1_p << "/" << m_eps1_q << ", eps2=" << m_eps2_p << "/"
                  << m_eps2_q;
          return sstream.str();
        }

      // template <typename T>
      // typename boost::enable_if_c<IDX == -1, T>::type rval(const T& r) const 
      // 	{
      // 	  return r;
      // 	}

      // template <typename T>
      // typename boost::disable_if_c<IDX == -1, typename T::value_type>::type rval(const T& r) const 
      // 	{
      // 	  return r[IDX];
      // 	}

      core::arith::interval sign(const core::arith::interval &x) const
	{
	  if(sup(x) < 0)
	    return core::arith::interval(-1);
	  else if(inf(x) > 0)
	    return core::arith::interval(1);
	  return core::arith::interval(-1,1);
	}

      core::arith::interval my_abs(const core::arith::interval &x)  const
	{
	  core::arith::mreal infx(abs(inf(x)));
	  core::arith::mreal supx(abs(sup(x)));
	  return core::arith::interval(std::min(infx, supx), std::max(infx, supx));
	}

      template <typename T>
      T operator()(const T& args) const
        {
          core::arith::interval pi_half(interval_pi()/2.0);
          core::arith::interval iarg1(enclosure_cast<core::arith::interval>(args[1]));
          T ret(3);

//          if(IDX == 0 || IDX == -1) {
            // 1. Komponente
            if(sup(iarg1) <= -sup(pi_half) || inf(iarg1) >= sup(pi_half))
              ret[0] = eval_x(args, -1.0);
            else if(inf(iarg1) >= -inf(pi_half) && sup(iarg1) <= inf(pi_half))
              ret[0] = eval_x(args, 1.0);
            else {
      	      core::arith::ivector nargs(enclosure_cast<core::arith::ivector>(args));
              nargs[1] = iarg1 & interval(inf(-pi_half), sup(pi_half));
              interval res(enclosure_cast<interval>(eval_x(nargs, 1.0)));

              nargs[1] =  iarg1 & interval(-inf(interval_pi()), -sup(pi_half));
              if(!core::arith::is_empty(nargs[1]))
                res |= enclosure_cast<interval>(eval_x(nargs, -1.0));

              nargs[1] =  iarg1 & interval(inf(pi_half), sup(interval_pi()));
              if(!core::arith::is_empty(nargs[1]))
                res |= enclosure_cast<interval>(eval_x(nargs, -1.0));

              ret[0] = forced_cast<typename T::value_type>(res);
            }
	    //        }

//          if(IDX == 1 || IDX == -1) {
            // 2. Komponente
            if(sup(iarg1) <= 0.0)
              ret[1] = eval_y(args, -1.0);
            else if(inf(iarg1) >= 0.0)
              ret[1] = eval_y(args, 1.0);
            else {
      	      core::arith::ivector nargs(enclosure_cast<core::arith::ivector>(args));
              nargs[1] = iarg1 & interval(-inf(interval_pi()), 0);
              interval res(enclosure_cast<interval>(eval_y(nargs, -1.0)));

              nargs[1] = iarg1 & interval(0, sup(interval_pi()));
              res |= enclosure_cast<interval>(eval_y(nargs, 1.0));
              ret[1] = forced_cast<typename T::value_type>(res);
            }
	    //        }

//          if(IDX == 2 || IDX == -1) {
            // 3. Komponente
            core::arith::interval iarg0(enclosure_cast<core::arith::interval>(args[0]));
            if(sup(iarg0) < 0.0)
              ret[2] = eval_z(args, -1.0);
            else if(inf(iarg0) > 0.0)
              ret[2] = eval_z(args, 1.0);
            else {
      	      core::arith::ivector nargs(enclosure_cast<core::arith::ivector>(args));
              nargs[0] = iarg0 & interval(-inf(pi_half), 0);
              interval res(enclosure_cast<interval>(eval_z(nargs, -1.0)));

              nargs[0] = iarg0 & interval(0, sup(pi_half));
              res |= enclosure_cast<interval>(eval_z(nargs, 1.0));
              ret[2] = forced_cast<typename T::value_type>(res);
            }
//          }

	  //
	  // core::arith::interval iarg0(enclosure_cast<core::arith::interval>(args[0]));

	  // core::arith::ivector iret(3);
	  // iret[0] = m_rx * sign(cos(iarg0)) * pow(abs(cos(iarg0)), m_eps1) * 
	  //   sign(cos(iarg1)) * pow(my_abs(cos(iarg1)), m_eps2);
	  
	  // iret[1] = m_ry * sign(cos(iarg0)) * pow(abs(cos(iarg0)), m_eps1) * 
	  //   sign(sin(iarg1)) * pow(my_abs(sin(iarg1)), m_eps2);

	  // iret[2] = m_rz * sign(sin(iarg0)) * pow(my_abs(sin(iarg0)), m_eps1);
	  
	  // return rval(forced_cast<T>(iret));
	  return ret;
        }


    private:
      mreal m_rx, m_ry, m_rz;
      int m_eps1_p, m_eps2_p;
      unsigned m_eps1_q, m_eps2_q;
      mreal m_eps1, m_eps2;
    };

    struct SQParamNormals 
    {
      explicit SQParamNormals(const functions::IVFunction &fun) 
	:m_fun(fun)
	{}

      template <typename T>
      bool check_normals(const T& args) const
	{
	  for(unsigned i =  0; i < 3; i++)
	    if(0 <= enclosure_cast<interval>(args[i]))
	      return false;
	  return true;
	}
	  

      // template <typename T>
      // typename boost::enable_if_c<IDX == -1, T>::type rval(T r) const 
      // 	{
      // 	  return r;
      // 	}

      // template <typename T>
      // typename boost::disable_if_c<IDX == -1, typename T::value_type>::type rval(T r) const 
      // 	{
      // 	  return r[IDX];
      // 	}

      template <typename T>
      T operator()(const T& args) const
	{
	  T ret(6);
	  ret[mtl::irange(0,3)] = m_fun(args);
	  if(check_normals(ret[mtl::irange(0,3)])) {
	    typename T::value_type sqrcos1(sqr(cos(args[0])));
	    ret[3] = sqrcos1*sqr(cos(args[1]))/ret[0];
	    ret[4] = sqrcos1*sqr(sin(args[1]))/ret[1];
	    ret[5] = sqr(sin(args[0]))/ret[2];
	  } 
	  else {
	    T r(3); // mtl::irange(0,3) fuehrt zu Speicherfehler in aufrufender Funktion?!
	    for(unsigned i = 0; i < 3;i++)
	      r[i] = ret[i];
	    return r;
	  }
	  return ret;
	}

      unsigned dim() const
	{
	  return 2;
	}

      unsigned cdim() const
	{
	  return 6;
	}

      std::string text() const
        {
          std::stringstream sstream;
          sstream << "Normals for " << m_fun.text();
          return sstream.str();
        }

    private:
      const functions::IVFunction& m_fun;
    };
  }

  functions::IVFunction* param_sq_normals(const functions::IVFunction &sq)
  {
    std::vector<functions::IFunction*> members;
    // members.push_back(new functions::details::FVBase<details::SQParamNormals<0> >( new details::SQParamNormals<0>(sq)));
    // members.push_back(new functions::details::FVBase<details::SQParamNormals<1> >( new details::SQParamNormals<1>(sq)));
    // members.push_back(new functions::details::FVBase<details::SQParamNormals<2> >( new details::SQParamNormals<2>(sq)));
    // members.push_back(new functions::details::FVBase<details::SQParamNormals<3> >( new details::SQParamNormals<3>(sq)));
    // members.push_back(new functions::details::FVBase<details::SQParamNormals<4> >( new details::SQParamNormals<4>(sq)));
    // members.push_back(new functions::details::FVBase<details::SQParamNormals<5> >( new details::SQParamNormals<5>(sq)));
    return new functions::details::FVBase<details::SQParamNormals>(new details::SQParamNormals(sq));
  }

  functions::IFunction* sq(mreal rx, mreal ry, mreal rz, int eps1_p, unsigned eps1_q,
                           int eps2_p, unsigned eps2_q)
  {
    return new functions::details::FJoin<details::SQImpl<false>, functions::details::FDeriv<details::SQImpl<false> > >
      (new details::SQImpl<false>(rx, ry, rz, eps1_p, eps1_q, eps2_p, eps2_q));
  }

  functions::IFunction* sq_bended(mreal rx, mreal ry, mreal rz, int eps1_p, unsigned eps1_q,
                                  int eps2_p, unsigned eps2_q, mreal kbend, mreal alpha)
  {
    return new functions::details::FJoin<details::SQImpl<true>, functions::details::FDeriv<details::SQImpl<true> > >
      (new details::SQImpl<true>(rx, ry, rz, eps1_p, eps1_q, eps2_p, eps2_q, kbend, alpha));
  }

  functions::IVFunction* param_sq(mreal rx, mreal ry, mreal rz, int eps1_p, unsigned eps1_q,
                                  int eps2_p, unsigned eps2_q)
  {
    std::vector<functions::IFunction*> members;
    // ohne AD, Differenzierbarkeit ungeklärt an den Nahtstellen
    // members.push_back(new functions::details::FBase<details::SQParamImpl<0> >( new details::SQParamImpl<0>(rx, ry, rz, eps1_p, eps1_q, eps2_p, eps2_q)));
    // members.push_back(new functions::details::FBase<details::SQParamImpl<1> >( new details::SQParamImpl<1>(rx, ry, rz, eps1_p, eps1_q, eps2_p, eps2_q)));
    // members.push_back(new functions::details::FBase<details::SQParamImpl<2> >( new details::SQParamImpl<2>(rx, ry, rz, eps1_p, eps1_q, eps2_p, eps2_q)));
    return new functions::details::FVBase<details::SQParamImpl>(new details::SQParamImpl(rx, ry, rz, eps1_p, eps1_q, eps2_p, eps2_q));
  }

  bool get_sq_params(const functions::IFunction &sq, core::arith::rvector &scale, int &eps1_p,
		     unsigned &eps1_q, int &eps2_p, unsigned &eps2_q)
    {
      auto sq_ptr = dynamic_cast<const functions::details::FJoin<details::SQImpl<false>, functions::details::FDeriv<details::SQImpl<false> > >*>(&sq);
      if(sq_ptr) {
	const details::SQImpl<false> &sqs = sq_ptr->get_f();
	scale[0] = sqs.m_rx;
	scale[1] = sqs.m_ry;
	scale[2] = sqs.m_rz;
	eps1_p = sqs.m_eps1_p;
	eps1_q = sqs.m_eps1_q;
	eps2_p = sqs.m_eps2_p;
	eps2_q = sqs.m_eps2_q;
      }
      return sq_ptr;
    }
}
