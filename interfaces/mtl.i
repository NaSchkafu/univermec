%{
#include "stable.h"
  %}

namespace mtl
{
  namespace vector
  {
// TODO: Es fehlen noch die weiteren Operatoren
    template <class Value>// , typename Parameters = parameters<> >
    class dense_vector
    //   : public vec_expr<dense_vector<Value, Parameters> >,
    //   public ::mtl::detail::contiguous_memory_block< Value, Parameters::on_stack, Parameters::dimension::value >,
    //     public crtp_base_vector< dense_vector<Value, Parameters>, Value, std::size_t >
    {
      // typedef dense_vector                                                             self;
      // typedef ::mtl::detail::contiguous_memory_block< Value, Parameters::on_stack,
      //                                                 Parameters::dimension::value >   memory_base;
      // typedef crtp_base_vector< self, Value, std::size_t >                             crtp_base;
      // typedef crtp_vector_assign< self, Value, std::size_t >                           assign_base;
      // typedef vec_expr<dense_vector<Value, Parameters> >                               expr_base;
    public:
      typedef Value             value_type ;
      typedef std::size_t       size_type ;
      typedef value_type&       reference ;
      typedef value_type const& const_reference ;
      typedef Value*            pointer ;
      typedef Value const*      const_pointer ;
      typedef typename Parameters::orientation  orientation;
      typedef const_pointer     key_type;

      // ctors
      dense_vector();
      explicit dense_vector(size_type n);
      explicit dense_vector( size_type n, value_type value );
      dense_vector( const dense_vector<Value>& src );

      //size_type size() const;

      // access
      reference operator()( size_type i );
      const_reference operator()( size_type i ) const;

      // iterators
      const_pointer begin() const;
      const_pointer end() const;
      pointer begin();
      pointer end();

      %extend {
        reference at(size_type i)
        {
          return $self->operator()(i);
        }

        void set(size_type i, const_reference e)
        {
          $self->operator[](i) = e;
        }

        char* __str__()
        {
          std::stringstream str;
          str << "[";
          for(mtl::vector::dense_vector<Value>::pointer it = $self->begin();
              it != $self->end(); ++it) {
            if(it != $self->begin())
              str << ", ";
            str << *it;
          }
          str << "]";
          // leak!
          const char *s = str.str().c_str();
          static char tmp[1024];
          strncpy(tmp, s, 1024);
          return tmp;
        }
      };
    };

    %template(rvector) dense_vector<core::arith::mreal>;
    %template(ivector) dense_vector<core::arith::interval>;

  }
}

namespace mtl
{
  namespace matrix
  {
    template <typename Value>
    class dense2D
    {

    public:
      typedef Value                             value_type;
      typedef const value_type&                 const_reference;
      typedef value_type&                       reference;
      typedef const value_type*                 const_pointer_type;
      typedef const_pointer_type                key_type;
      typedef std::size_t                       size_type;
      typedef dense_el_cursor<Value>            el_cursor_type;
      typedef dense2D_indexer                   indexer_type;

    public:
      // ctors
      dense2D();
      dense2D(size_type num_rows, size_type num_cols);

      %extend {
        Value at(size_type i, size_type j)
        {
          return (*$self)[i][j];
        }

        void set(size_type i, size_type j, const_reference e)
        {
          (*$self)[i][j] = e;
        }

	char* __str__()
	{
	  std::stringstream str;
	  str << *$self;
	  static char tmp[1024];
	  strncpy(tmp, str.str().c_str(), 1024);
	  return tmp;
	}
      }
    };
    %template(imatrix) dense2D<core::arith::interval>;
    %template(rmatrix) dense2D<core::arith::mreal>;
  }
}

%inline
%{
  namespace core
  {
    namespace arith
    {
      typedef mtl::matrix::dense2D<core::arith::mreal> rmatrix;
      typedef mtl::matrix::dense2D<core::arith::interval> imatrix;
      typedef mtl::vector::dense_vector<core::arith::mreal> rvector;
      typedef mtl::vector::dense_vector<core::arith::interval> ivector;
     }
  }
%}




