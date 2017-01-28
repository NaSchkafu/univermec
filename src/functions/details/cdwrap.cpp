#include "functions/details/cdwrap.hpp"

#include <memory>

#include "functions/ifunction.hpp"
#include "functions/iffeatures.hpp"
#include "functions/iderivative.hpp"
#include "functions/itaylorcoeff.hpp"


namespace functions
{
  namespace details
  {

    class TaylorCoeffWrapper : public ITaylorCoeff
    {
    public:
      TaylorCoeffWrapper(const IVFunction &f, unsigned member)
        :m_f(f), m_member(member)
        {}

      virtual ~TaylorCoeffWrapper()
        {}

    private:
      virtual std::vector<core::arith::rvector> taylor_coeff_ode_(const core::arith::rvector &args, unsigned order) const
        {
          std::vector<core::arith::rvector> v;
          v.push_back(m_f.features().taylor_coeff()->taylor_coeff_ode(args, order)[m_member]);
          return v;
        }

      virtual std::vector<core::arith::ivector> taylor_coeff_ode_(const core::arith::ivector &args, unsigned order) const
        {
          std::vector<core::arith::ivector> v;
          v.push_back(m_f.features().taylor_coeff()->taylor_coeff_ode(args, order)[m_member]);
          return v;
        }

      virtual std::vector<core::arith::avector> taylor_coeff_ode_(const core::arith::avector &args, unsigned order) const
        {
          std::vector<core::arith::avector> v;
          v.push_back(m_f.features().taylor_coeff()->taylor_coeff_ode(args, order)[m_member]);
          return v;
        }

      virtual std::vector<core::arith::tvector> taylor_coeff_ode_(const core::arith::tvector &args, unsigned order) const
        {
          std::vector<core::arith::tvector> v;
          v.push_back(m_f.features().taylor_coeff()->taylor_coeff_ode(args, order)[m_member]);
          return v;
        }

      virtual std::vector<std::vector<core::arith::rvector> > taylor_coeff_ode_df_(const core::arith::rvector &args, unsigned order) const
        {
          std::vector<std::vector<core::arith::rvector> > v(1);
          v[0] = m_f.features().taylor_coeff()->taylor_coeff_ode_df(args, order)[m_member];
          return v;
        }

      virtual std::vector<std::vector<core::arith::ivector> > taylor_coeff_ode_df_(const core::arith::ivector &args, unsigned order) const
        {
          std::vector<std::vector<core::arith::ivector> > v(1);
          v[0] = m_f.features().taylor_coeff()->taylor_coeff_ode_df(args, order)[m_member];
          return v;
        }

      virtual std::vector<std::vector<core::arith::avector> > taylor_coeff_ode_df_(const core::arith::avector &args, unsigned order) const
        {
          std::vector<std::vector<core::arith::avector> > v(1);
          v[0] = m_f.features().taylor_coeff()->taylor_coeff_ode_df(args, order)[m_member];
          return v;
        }

      virtual std::vector<std::vector<core::arith::tvector> > taylor_coeff_ode_df_(const core::arith::tvector &args, unsigned order) const
        {
          std::vector<std::vector<core::arith::tvector> > v(1);
          v[0] = m_f.features().taylor_coeff()->taylor_coeff_ode_df(args, order)[m_member];
          return v;
        }

    private:
      const IVFunction& m_f;
      unsigned m_member;
    };

    class DerivativeWrapper : public IDerivative
    {
    public:
      DerivativeWrapper(const IVFunction &f, unsigned member)
        :m_f(f), m_member(member)
        {}

      virtual ~DerivativeWrapper()
        {}

    private:
      virtual const IFunction& grad_(unsigned direction, unsigned member) const
        {
          assert(!member && "Invalid member requested!");
          return m_f.features().derivative()->grad(direction, m_member);
        }

      virtual core::arith::ivector grad_(const core::arith::ivector &args, unsigned member) const
        {
          assert(!member && "Invalid member requested!");
          return m_f.features().derivative()->grad(args, m_member);
        }

      virtual core::arith::avector grad_(const core::arith::avector &args, unsigned member) const
        {
          assert(!member && "Invalid member requested!");
          return m_f.features().derivative()->grad(args, m_member);
        }

      virtual core::arith::tvector grad_(const core::arith::tvector &args, unsigned member) const
        {
          assert(!member && "Invalid member requested!");
          return m_f.features().derivative()->grad(args, m_member);
        }

      virtual core::arith::rvector grad_(const core::arith::rvector &args, unsigned member) const
        {
          assert(!member && "Invalid member requested!");
          return m_f.features().derivative()->grad(args, m_member);
        }

      virtual core::arith::rmatrix jacobian_(const core::arith::rvector &args) const
        {
          core::arith::rmatrix v(1, m_f.dim()+1);
          v[0][mtl::iall] = m_f.features().derivative()->jacobian(args)[m_member][mtl::iall];
          return v;
        }

      virtual core::arith::imatrix jacobian_(const core::arith::ivector &args) const
        {
          core::arith::imatrix v(1, m_f.dim()+1);
          v[0][mtl::iall] = m_f.features().derivative()->jacobian(args)[m_member][mtl::iall];
          return v;
        }

      virtual core::arith::amatrix jacobian_(const core::arith::avector &args) const
        {
          core::arith::amatrix v(1, m_f.dim()+1);
          v[0][mtl::iall] = m_f.features().derivative()->jacobian(args)[m_member][mtl::iall];
          return v;
        }
      virtual core::arith::tmatrix jacobian_(const core::arith::tvector &args) const
        {
          core::arith::tmatrix v(1, m_f.dim()+1);
          v[0][mtl::iall] = m_f.features().derivative()->jacobian(args)[m_member][mtl::iall];
          return v;
        }

      virtual const IVFunction& grad_fun_(unsigned member) const
        {
          assert(!member && "Invalid grad_fun_ requested!");
          return m_f.features().derivative()->grad_fun(m_member);
        }

      unsigned m_member;
      const IVFunction & m_f;
    };

    class CDWrapper : public IFunction, public IFFeatures
    {
    public:
      CDWrapper(const IVFunction &f, unsigned member)
        :m_f(f), m_member(member)
        {
          if(m_f.features().derivative())
            m_df = std::make_unique<DerivativeWrapper>(f, m_member);
          if(m_f.features().taylor_coeff())
            m_tf = std::make_unique<TaylorCoeffWrapper>(f, m_member);
        }

      virtual ~CDWrapper() {}

    private:
      virtual core::arith::ivector calc_v_(const core::arith::ivector &args) const
        {
          return core::arith::ivector(1, m_f(args)[m_member]);
        }

      virtual core::arith::avector calc_v_(const core::arith::avector &args) const
        {
          return core::arith::avector(1, m_f(args)[m_member]);
        }

      virtual core::arith::rvector calc_v_(const core::arith::rvector &args) const
        {
          return core::arith::rvector(1, m_f(args)[m_member]);
        }

      virtual core::arith::tvector calc_v_(const core::arith::tvector &args) const
        {
          return core::arith::tvector(1, m_f(args)[m_member]);
        }

      virtual const IFunction& fun_(unsigned i) const
        {
          assert(!i && "Invalid member requested");
          return *this;
        }

      virtual unsigned dim_() const
        {
          return m_f.dim();
        }

      virtual unsigned cdim_() const
        {
          return 1;
        }

      virtual IFunction* clone_() const
        {
          // FIXME: Die derzeitige Struktur ist unguenstig
          // IVFunction <-> CDWrapper haben sich gegenseitig als
          // member, dies verhindert ein DeepCopy... :(
          assert(!"clone_() not implemented!");
          return nullptr;
        }

      virtual std::string text_() const
        {
          std::stringstream sstr;
          sstr << "CDWrapper of " << m_member << "-to codomain element of "
               << m_f.text();
          return sstr.str();
        }

      virtual const IFFeatures& features_() const
        {
          return *this;
        }

      // ****************************************************************
      // Features
      virtual const IDerivative* derivative_() const
        {
          return m_df.get();
        }

      virtual const ICustomConsistency* custom_consistency_() const
        {
          // TODO: Impl
          return nullptr;
        }

      virtual const ICustomEncl* custom_encl_() const
        {
          // TODO: Impl
          return nullptr;
        }

      virtual const ITaylorCoeff* taylor_coeff_() const
        {
          return m_tf.get();
        }

      virtual IGPUEval* gpu_eval_() const
        {
          //TODO IMPL
          return nullptr;
        }

      virtual core::arith::interval calc_(const core::arith::ivector &args) const
        {
          return m_f(args)[m_member];
        }

      virtual core::arith::aaf calc_(const core::arith::avector &args) const
        {
          return m_f(args)[m_member];
        }

      virtual core::arith::mreal calc_(const core::arith::rvector &args) const
        {
          return m_f(args)[m_member];
        }

      virtual core::arith::taylormodel calc_(const core::arith::tvector &args) const
        {
          return m_f(args)[m_member];
        }



    private:
      unsigned m_member;
      const IVFunction &m_f;
      std::unique_ptr<IDerivative> m_df;
      std::unique_ptr<ITaylorCoeff> m_tf;
    };

    IFunction* cd_wrap(const IVFunction &iv, unsigned member)
    {
      return new CDWrapper(iv, member);
    }

  }
}
