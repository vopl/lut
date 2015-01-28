#include <boost/spirit/include/lex_lexertl.hpp>
//#include <boost/spirit/include/phoenix_operator.hpp>
//#include <boost/spirit/include/phoenix_statement.hpp>
//#include <boost/spirit/include/phoenix_algorithm.hpp>
//#include <boost/spirit/include/phoenix_core.hpp>

namespace lut { namespace coupling { namespace parser { namespace impl
{

    template <typename TIterator>
    struct Tokens
        : boost::spirit::lex::lexer<boost::spirit::lex::lexertl::actor_lexer<boost::spirit::lex::lexertl::token<TIterator>>>
    {
        using Token = typename Tokens::token_type;

        Tokens()
            : id{"[a-zA-Z_]+\\w*"}
            , ob{'('}
            , cb{')'}
            , ocb {'{'}
            , ccb {'}'}
            , semi{';'}
            , colon{':'}
        {
            using boost::spirit::_pass;

            this->self
                = boost::spirit::lex::token_def<>{"\\s"}                       [_pass = boost::spirit::lex::pass_flags::pass_ignore]
                | boost::spirit::lex::token_def<>{"\\/\\/[^\\r\\n]*\\r?\\n?"}  [_pass = boost::spirit::lex::pass_flags::pass_ignore]
                | boost::spirit::lex::token_def<>{"\\/\\*(.*?)\\*\\/"}         [_pass = boost::spirit::lex::pass_flags::pass_ignore]
                | id
                | ob
                | cb
                | ocb
                | ccb
                | semi
                | colon
            ;
        }

        boost::spirit::lex::token_def<> id;            // identifier
        boost::spirit::lex::token_def<> ob, cb;        // ()
        boost::spirit::lex::token_def<> ocb, ccb;      // {}
        boost::spirit::lex::token_def<> semi;          // ;
        boost::spirit::lex::token_def<> colon;         // :
    };

}}}}
