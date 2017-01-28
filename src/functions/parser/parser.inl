// -*-c++-*-
#include <functions/parser/functionparser.hpp>
#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/classic_ast.hpp>
#include <boost/lexical_cast.hpp>

#include <core/arithcore.hpp>
#include "functions/functionexceptions.hpp"


using namespace BOOST_SPIRIT_CLASSIC_NS;
using namespace std;
using namespace core::arith;

namespace functions
{
  namespace details
  {
    struct function_grammar : public grammar<function_grammar>
    {
      static const int GROUP_ID = 1;
      static const int BINARY_GROUP_ID = 2;
      static const int UNARY_FUNCTION_ID = 3;
      static const int POW_FUNCTION_ID = 4;
      static const int INTERVAL_ID = 5;
      static const int VARIABLE_ID = 6;
      static const int NUMBERS_ID = 7;
      static const int UNARY_MINUS_ID = 8;
      static const int FACTOR_ID = 9;
      static const int EXPRESSION_ID = 10;
      static const int TERM_ID = 11;
      static const int INT_ID = 12;
      static const int UNARY_FUNCTION_NAMES_ID = 13;
      static const int UREAL_ID = 14;
      static const int UREAL_POW_FUNCTION_ID = 15;

      template<typename ScannerT>
      struct definition
      {
        // ctor
        definition(const function_grammar &)
          {
	    // TODO: überarbeiten am besten direkt auf Qi/Karma umstellen
            group       = inner_node_d['(' >> expression >> ')'];
	    unary_function_names = str_p("sqrt") | str_p("sqr") |  str_p("sin") | str_p("cos") 
	      | str_p("exp");
            unary_function = root_node_d[leaf_node_d[unary_function_names]] >> group;
	    pow_function = root_node_d[str_p("pow")] >> inner_node_d[ch_p('(') >> expression >> ch_p(',')
							  >> my_int >> ch_p(')')];
	    ureal_pow_function = root_node_d[str_p("pow")] >> inner_node_d[ch_p('(') >> expression >> ch_p(',')
							 >> my_ureal >> ch_p(')')];
            interval    = root_node_d[inner_node_d[(ch_p('[') >> real_p
                                                    >> no_node_d[*blank_p >> ch_p(',') >> *blank_p]
                                                    >> real_p >> ch_p(']'))]];
            variable    = no_node_d[ch_p('x')] >> leaf_node_d[uint_p];
            numbers     = leaf_node_d[real_p] | interval | variable;
            unary_minus = root_node_d[ch_p('-')] >> factor;
            factor      = numbers | group | unary_function | pow_function | ureal_pow_function | unary_minus;
            term        = factor >> *( (root_node_d[ch_p('*')] | root_node_d[ch_p('/')]) >> factor);
//                                       | (root_node_d[ch_p('/')] >> factor));
            expression  = term >> *( (root_node_d[ch_p('+')] >> term)
                                     | (root_node_d[ch_p('-')] >> term));
	    my_int      = leaf_node_d[int_p];
	    my_ureal    = leaf_node_d[ureal_p];
          }

        // members
        rule<ScannerT, parser_context<>, parser_tag<function_grammar::GROUP_ID> > group;
	rule<ScannerT, parser_context<>, parser_tag<function_grammar::UNARY_FUNCTION_NAMES_ID> > unary_function_names;
        rule<ScannerT, parser_context<>, parser_tag<function_grammar::UNARY_FUNCTION_ID> > unary_function;
	rule<ScannerT, parser_context<>, parser_tag<function_grammar::POW_FUNCTION_ID> > pow_function;
        rule<ScannerT, parser_context<>, parser_tag<function_grammar::INTERVAL_ID> > interval;
        rule<ScannerT, parser_context<>, parser_tag<function_grammar::VARIABLE_ID> > variable;
        rule<ScannerT, parser_context<>, parser_tag<function_grammar::NUMBERS_ID> > numbers;
        rule<ScannerT, parser_context<>, parser_tag<function_grammar::UNARY_MINUS_ID> > unary_minus;
        rule<ScannerT, parser_context<>, parser_tag<function_grammar::FACTOR_ID> > factor;
        rule<ScannerT, parser_context<>, parser_tag<function_grammar::TERM_ID> > term;
        rule<ScannerT, parser_context<>, parser_tag<function_grammar::EXPRESSION_ID> > expression;
	rule<ScannerT, parser_context<>, parser_tag<function_grammar::INT_ID> > my_int;
	rule<ScannerT, parser_context<>, parser_tag<function_grammar::UREAL_ID> > my_ureal;
	rule<ScannerT, parser_context<>, parser_tag<function_grammar::UREAL_POW_FUNCTION_ID> > ureal_pow_function;


        const rule<ScannerT, parser_context<>, parser_tag<EXPRESSION_ID> >& start() const
          {
            return expression;
          }
      };
    };

    struct FParser
    {
      explicit FParser(const std::string &fstr, unsigned dim)
        :m_fstr(fstr)
        {
          m_ast = ast_parse(m_fstr.c_str(), m_grammar);
          if(!m_ast.full) {
            parser_exception e;
            if(m_ast.match)
              e << parser_err_pos(m_ast.length);
            throw e;
          }
	  if(!dim)
	    m_dim = find_dim(m_ast.trees.begin()) + 1;
	  else
	    m_dim = dim;
        }


      template<typename T>
      typename T::value_type operator()(const T& args) const
        {
          return eval(args, m_ast.trees.begin());
        }

      unsigned dim() const
        {
          return m_dim;
        }

      std::string text() const
        {
          return m_fstr;
        }

    private:
      /// function string
      std::string m_fstr;
      /// dimension
      unsigned m_dim;
      ///  grammar
      function_grammar m_grammar;
      /// abstract syntax tree
      tree_parse_info<> m_ast;

    private:
      typedef const char* iterator_t;
      typedef tree_match<iterator_t> parse_tree_match_t;
      typedef parse_tree_match_t::const_tree_iterator iter_t;

      template<typename T>
      typename T::value_type eval(const T& args, const iter_t &i) const
        {
          if(i->value.id() == parser_id(function_grammar::UNARY_FUNCTION_ID)) {
            string f(i->value.begin(), i->value.end());
            if(f == "sqr")
              return sqr(eval(args, i->children.begin()));
	    else if(f == "sqrt")
	      return sqrt(eval(args, i->children.begin()));
	    else if(f == "sin")
	      return sin(eval(args, i->children.begin()));
	    else if(f == "cos")
	      return cos(eval(args, i->children.begin()));
	    else if(f == "exp")
	      return exp(eval(args, i->children.begin()));
          }
	  else if(i->value.id() == parser_id(function_grammar::POW_FUNCTION_ID)) {
	    return pow(static_cast<typename T::value_type>(eval(args, i->children.begin())), 
		       static_cast<int>(to_double(forced_cast<mreal>(eval(args, i->children.begin()+2)))));
	  }
	  else if(i->value.id() == parser_id(function_grammar::UREAL_POW_FUNCTION_ID)) {
// 	    assert(!"IMPL!");
	    return pow(static_cast<typename T::value_type>(eval(args, i->children.begin())), 
		       forced_cast<mreal>(eval(args, i->children.begin()+2)));
	  }
          else if(i->value.id() == parser_id(function_grammar::INTERVAL_ID)) {
            std::string s1(i->children.begin()->value.begin(), i->children.begin()->value.end());
            std::string s2((i->children.begin()+1)->value.begin(), 
			   (i->children.begin()+1)->value.end());
            return core::arith::ArithTraits<typename T::value_type>::convert
	      (core::arith::interval(boost::lexical_cast<double>(s1), 
				     boost::lexical_cast<double>(s2)));
          }
          else if(i->value.id() == parser_id(function_grammar::VARIABLE_ID)) {
            unsigned var = boost::lexical_cast<unsigned>
	      (std::string(i->value.begin(), i->value.end()));
            if(var > m_dim) {
              throw parser_exception() << parser_unknown_variable(var);
            }
            return args[var];
          }
          else if(i->value.id() == parser_id(function_grammar::NUMBERS_ID))
          {
            return typename T::value_type
	      (boost::lexical_cast<double>(std::string(i->value.begin(), i->value.end())));

          }
          else if(i->value.id() == parser_id(function_grammar::UNARY_MINUS_ID)) {
            return -eval(args, i->children.begin());
          }
          else if(i->value.id() == parser_id(function_grammar::EXPRESSION_ID)) {
            if(*i->value.begin() == '+'){
              return eval(args, i->children.begin()) + eval(args, i->children.begin()+1);
	    }
	    
            else if(*i->value.begin() == '-')
	      return eval(args, i->children.begin()) - eval(args, i->children.begin()+1);
          }
          else if(i->value.id() == parser_id(function_grammar::TERM_ID)) {
            if(*i->value.begin() == '*') {
              return eval(args, i->children.begin()) * eval(args, i->children.begin() + 1);
            }
            else if(*i->value.begin() == '/') {
              return eval(args, i->children.begin()) / eval(args, i->children.begin() + 1);
            }
          }
	  else if(i->value.id() == parser_id(function_grammar::INT_ID) ||
		  i->value.id() == parser_id(function_grammar::UREAL_ID)) {
	    return typename T::value_type
	      (boost::lexical_cast<double>(std::string(i->value.begin(), i->value.end())));
	  }
	  
	  assert(!"Unknown execution path!");
        }

      unsigned find_dim(const iter_t &i) const
	{
	  unsigned dim = 0;
	  if(i->value.id() == parser_id(function_grammar::VARIABLE_ID)) {
	    dim = max(dim, boost::lexical_cast<unsigned>(std::string(i->value.begin(), i->value.end())));
	  }
	  else {
	    iter_t ch_end(i->children.end());
	    for(iter_t it(i->children.begin()); it != ch_end; ++it)
	      dim = max(find_dim(it), dim);
	  }
	  return dim;
	}
      
    };
  }
}
