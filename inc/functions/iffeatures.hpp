#ifndef __IFFEATURES_HPP__
#define __IFFEATURES_HPP__

namespace functions
{

  class IDerivative;
  class ICustomConsistency;
  class ICustomEncl;
  class ITaylorCoeff;
  class IGPUEval;

/// Interface for accessing features of a function
/**
 * According to our function model a basic function inherits the interface
 * IFunction. Function may have features like derivatives, custom consistencies or
 * enclosures or even be representable by elementary function graph.
 *
 * You must not delete a pointer returned by the features
 * interfaces. Ownership does NOT transfer to the caller.
 */
  class IFFeatures
  {
  public:
    inline const IDerivative* derivative() const
      {
        return derivative_();
      }

    inline const ICustomConsistency* custom_consistency() const
      {
        return custom_consistency_();
      }

    inline const ICustomEncl* custom_encl() const
      {
        return custom_encl_();
      }

    inline const ITaylorCoeff* taylor_coeff() const
      {
	return taylor_coeff_();
      }

    inline IGPUEval* gpu_eval() const
      {
	return gpu_eval_();
      }

    // TODO: ISlope/IEleGraph

    virtual ~IFFeatures() {}

  private:
    virtual const IDerivative* derivative_() const = 0;
    virtual const ICustomConsistency* custom_consistency_() const = 0;
    virtual const ICustomEncl* custom_encl_() const = 0;
    virtual const ITaylorCoeff* taylor_coeff_() const = 0;
    virtual IGPUEval* gpu_eval_() const = 0;
  };

}


#endif /*__IFFEATURES_HPP__ */

