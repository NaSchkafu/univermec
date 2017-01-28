#include <algorithm/epsdistance/nodeprox.hpp>

#include <fenv.h>

namespace distance
{
  using namespace core::arith;
    // Berechnet eine obere und untere Grenze für den Abstand zwischen zwei Boxen
  // gemäß dem Verfahren von Dyllong/Luther
  interval d_cubes_aligned(const ivector &X, const ivector &Y)
  {
    unsigned dim = size(X);
    mreal ub = 0.0, lb = 0.0;
    int round = fegetround();
    fesetround(FE_UPWARD);
    for(unsigned i = 0; i < dim; i++)
      if(inf(X[i]) > sup(Y[i]))
        ub += sqr(inf(X[i]) - sup(Y[i]));
      else if(inf(Y[i]) > sup(X[i]))
        ub += sqr(inf(Y[i]) - sup(X[i]));

    for(unsigned i = 0; i < dim; i++)
      if(inf(X[i]) > sup(Y[i]))
	lb += sqr(inf(X[i]) - sup(Y[i]));
      else if(inf(Y[i]) > sup(X[i]))
	lb += sqr(inf(Y[i]) - sup(X[i]));
    fesetround(round);
    return interval(lb, ub);
  }

  // Modifiziertes Verfahren von Dyllong/Luther
  // Abstand zwischen zwei Boxen wobei bei Y der gesamte Bereich betrachtet wird
  // Währen für X nur der minimale Abstand herangezogen wird
  interval d_cubes_aligned2(const ivector &X, const ivector &Y)
  {
    unsigned dim = size(X);
    interval bound(0.0);
    int round = fegetround();
    for(unsigned i = 0; i < dim; i++)
      if(inf(X[i]) > sup(Y[i]))
        bound += sqr(inf(X[i]) - Y[i]);
      else if(inf(Y[i]) > sup(X[i]))
        bound += sqr(Y[i] - sup(X[i]));    
      else {
	fesetround(FE_UPWARD);
	mreal ub(0.0);
	if(sup(Y[i]) > sup(X[i]))
	  ub = sqr(sup(Y[i]) - sup(X[i]));
	if(inf(X[i]) > inf(Y[i]))
	  ub = std::max(ub, sqr(inf(Y[i]) - inf(X[i])));
	bound += interval(0, ub);
      }
    fesetround(round);
    return bound;
  }
  

  interval node_distance(const decomp::IIVTreeNode &r1, const decomp::IIVTreeNode &r2)
  {
    const decomp::IIVTreeNode *rp1, *rp2;
    rp1 = &(r1.color() == decomp::BLACK ? r1 : r2);
    rp2 = &(r1.color() == decomp::BLACK ? r2 : r1);

    if(rp1->color() == decomp::BLACK) {
      // Beide Schwarz
      if(rp2->color() == decomp::BLACK)
	return d_cubes_aligned(rp1->X(), rp2->X());
      // Ein Knoten schwarz
      return d_cubes_aligned2(rp1->X(), rp2->X());
    }

    //return distance_cube_cube(r1.X(), r2.X(), rm1t, rm2t, im1t, im2t);
    return dot(static_cast<ArithTraits<interval>::vec_t>(r1.X() - r2.X()));
  }
}
