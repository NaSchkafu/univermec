#include <core/interval_util.hpp>
#include <core/roundingcontrol.hpp>

namespace core
{
  namespace arith
  {
    ArithTraits<mreal>::vec_t mid(const ArithTraits<interval>::vec_t &iv)
    {
      unsigned dim = size(iv);
      ArithTraits<mreal>::vec_t v(dim);
      for(unsigned i = 0; i < dim; i++)
        v[i] = mid(iv[i]);
      return v;
    }


    bool is_empty(const ArithTraits<interval>::vec_t &iv)
    {
      unsigned dim = size(iv);
      for(unsigned i = 0; i < dim; i++)
        if(core::arith::is_empty(iv[i]))
          return true;
      return false;
    }

    mreal wid(const ArithTraits<interval>::vec_t &iv)
    {
      unsigned dim = size(iv);
      assert(dim > 0 && "w called on box with dimension 0");
      mreal wi(wid(iv[0]));
      for(unsigned i = 1; i < dim; i++)
        wi = std::max(wi, wid(iv[i]));
      return wi;
    }

    ArithTraits<mreal>::mat_t mid(const ArithTraits<interval>::mat_t &iv)
    {
      ArithTraits<mreal>::mat_t m(num_rows(iv),num_cols(iv));
      for(unsigned i = 0; i < num_rows(m); i++)
        for(unsigned j = 0; j < num_cols(m); j++)
          m[i][j] = mid(iv[i][j]);
      return m;
    }

    ArithTraits<mreal>::mat_t inf(const ArithTraits<interval>::mat_t &im)
    {
      ArithTraits<mreal>::mat_t r(num_rows(im),num_cols(im));
      for(unsigned int i=0;i<num_rows(im);i++){
        for(unsigned int j = 0; j<num_cols(im);j++) {
          r[i][j] = inf(im[i][j]);
        }
      }
      return r;
    }

    int in ( const interval& x, const interval& y )
    {
      if(( inf(y) < inf(x)) && (sup(x) < sup(y)))
        return 1;
      else
        return 0;
    }
    //TODO_C Stimmt der Index des zahelers?
    //Von i=0 bis i<=n wobei n Anzahl der Spalten? Vielleicht noch -1
    int in(const ArithTraits<interval>::vec_t &x, const ArithTraits<interval>::vec_t &y)
    {
      int ok = 1;
      int n = num_rows(x);
      int i = 0;
      while (ok && i <= n) { ok = in(x[i],y[i]); i++; }
      return ok;
    }

    ArithTraits<mreal>::vec_t sup(const ArithTraits<interval>::vec_t &iv)
    {
      unsigned dim(mtl::size(iv));
      ArithTraits<mreal>::vec_t r(dim);
      for(unsigned int i = 0; i < dim; i++){
        r[i] = sup(iv[i]);
      }
      return r;
    }

    ArithTraits<mreal>::vec_t inf(const ArithTraits<interval>::vec_t &iv)
    {
      unsigned dim(mtl::size(iv));
      ArithTraits<mreal>::vec_t r(dim);
      for(unsigned int i = 0; i < dim; i++){
        r[i] = inf(iv[i]);
      }
      return r;
    }


    ArithTraits<interval>::vec_t ex_division(const interval &y, const interval &x)
    {
      ArithTraits<interval>::vec_t r(2);
      // Impl analog zu Hansen S. 171ff
      r[1] = interval(1,-1);
      core::ROUNDING_MODE round = core::get_round();
      if(0 <= x) {
	if(inf(y) > 0) {
	  if(inf(x) == 0) {
	    core::set_round(core::RND_UPWARD);
	    r[0] = interval(-infinity(), inf(y)/sup(x));
	    r[1] = interval(infinity());
	  }
	  else if(sup(x) == 0) {
	    core::set_round(core::RND_DOWNWARD);
	    r[0] = interval(inf(y)/inf(x),infinity());
	    r[1] = interval(-infinity());
	  } 
	  else {
	    core::set_round(core::RND_UPWARD);
	    r[0] = interval(-infinity(), inf(y)/sup(x));
	    core::set_round(core::RND_DOWNWARD);
	    r[1] = interval(inf(y)/inf(x),infinity());
	  }
	}
	else if(sup(y) < 0) {
	  if(inf(x) == 0) {
	    core::set_round(core::RND_DOWNWARD);
	    r[0] = interval(sup(y)/sup(x), infinity());
	    r[1] = interval(-infinity());
	  }
	  else if(sup(x) == 0) {
	    core::set_round(core::RND_UPWARD);
	    r[0] = interval(-infinity(),sup(y)/inf(x));
	    r[1] = interval(infinity());
	  } 
	  else {
	    core::set_round(core::RND_DOWNWARD);
	    r[0] = interval(sup(y)/sup(x), infinity());
	    core::set_round(core::RND_UPWARD);
	    r[1] = interval(-infinity(),sup(y)/inf(x));
	  }
	} 
	else {
	  r[0] = interval(-infinity(), infinity());
	}
      } 
      else {
	r[0] = y/x;
      }
      core::set_round(round);
      return r;
      // Alte Impl.
      // r[1] = interval(1,-1);
      // if(sup(b) == 0)
      //   r[0] = interval(-infinity(), 1 / inf(b)) * a;
      // else if(inf(b) == 0)
      //   r[0] =  interval(1 / sup(b), infinity()) * a;
      // else if((inf(b)<0) && (sup(b)>0)) {
      //   r[0] = interval(-infinity(),1/inf(b))*a;
      //   r[1] = interval(1/sup(b),infinity())*a;
      // }else {
      //   r[0] = a / b;
      // }
      // return r;
    }

    bool operator==(const ArithTraits<interval>::vec_t &v1, 
		    const ArithTraits<interval>::vec_t &v2)
    {
      unsigned dim = size(v1);
      assert(dim == size(v2) && "Vector size does not match in operator==!");
      for(unsigned i = 0; i < dim; i++)
	if(v1[i] != v2[i])
	  return false;
      return true;
    }

    // std::ostream& operator<<(std::ostream &os, const interval &i)
    // {
    //   os << "[" << inf(i) << " , " << sup(i) << "]";
    //   return os;
    // }

    ArithTraits<interval>::vec_t sqr(const ArithTraits<interval>::vec_t &v)
    {
      unsigned dim = size(v);
      ArithTraits<interval>::vec_t r(dim);
      for(unsigned i = 0; i < dim; i++)
	r[i] = sqr(v[i]);
      return r;
    }

    interval dot(const ArithTraits<interval>::vec_t &v)
    {
      //assert(!"BUGGED");
      // Benutze langes Skalarprodukt der C-XSC falls möglich
// #ifdef USE_IV_CXSC
      // cxsc::idotprecision buffer;
      // unsigned dim = size(v);
      // for(unsigned i = 0; i < dim; i++) 
      // 	accumulate(buffer, details::get_cxsc(v[i]), details::get_cxsc(v[i]));
      // return details::from_cxsc(rnd(buffer));
// #else
      unsigned dim = size(v);
      interval r(0.0,0.0);
      for(unsigned i = 0; i < dim; i++) 
	r += sqr(v[i]);
      return r;
// #endif
    }
    
    // interval two_norm(const ArithTraits<interval>::vec_t &v)
    // {
    //   unsigned dim = size(v);
    //   interval r(0.0,0.0);
    //   for(unsigned i = 0; i < dim; i++) 
    // 	r += sqr(v[i]);
    //   return r;
    // }

    ArithTraits<interval>::vec_t operator&(ArithTraits<interval>::vec_t v1,
					   const ArithTraits<interval>::vec_t &v2)
    {
      // TODO: Exception
      assert(size(v1) == size(v2) && "Dimensions do not match!");
      for(unsigned i = 0; i < size(v1); i++)
	v1[i] &= v2[i];
      return v1;
    }

    bool operator<=(const interval &x, const interval &y)
    {
      return inf(x) >= inf(y) && sup(x) <= sup(y);
    }

    bool operator<=(const ArithTraits<interval>::vec_t &x, const ArithTraits<interval>::vec_t &y)
    {
      unsigned s = size(x);
      assert(s == size(y) && "Vector size does not match!");
      bool ret = true;
      for(unsigned i = 0; i < s && ret; i++)
	ret = x[i] <= y[i];
      return ret;
    }

    ArithTraits<interval>::vec_t operator|(const ArithTraits<interval>::vec_t &v1,
					   const ArithTraits<interval>::vec_t &v2)
    {
      assert(size(v1) == size(v2));
      unsigned dim = size(v1);
      ivector r(dim);
      for(unsigned i = 0; i < dim; i++) {
	r[i] = v1[i] | v2[i];
      }
      return r;
    }
  }
}

