#include "lut/stable.hpp"
#include "lut/coupling/parser/exec.hpp"

#include <boost/spirit/include/support_multi_pass.hpp>
#include <fstream>

#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_algorithm.hpp>
#include <boost/spirit/include/phoenix_core.hpp>


namespace lex = boost::spirit::lex;

template <typename Lexer>
struct word_count_tokens : lex::lexer<Lexer>
{
    word_count_tokens()
      : c(0), w(0), l(0)
      , word("[^ \t\n]+")     // define tokens
      , eol("\n")
      , any(".")
    {
        using lex::_start;
        using lex::_end;
        using boost::phoenix::ref;

        // associate tokens with the lexer
        this->self
            =   word
            |   eol
            |   any
            ;
    }

    std::size_t c, w, l;
    lex::token_def<> word, eol, any;
};

namespace lut { namespace coupling { namespace parser
{
    bool exec(const std::string &fileName, const Config &cfg, meta::Library &lib, std::vector<ErrorInfo> &errs)
    {
        /*
         * load content
         * tokenize, remove comments
         * parse (determine start, parse body), recursive. Make entities, execute includes
         * fill lib
         */

        std::ifstream reader{fileName};
        if(!reader)
        {
            errs.emplace_back(ErrorInfo {fileName, -1, std::error_code(errno, std::system_category())});
            return false;
        }

        typedef std::istreambuf_iterator<char> rawIterator;
        typedef boost::spirit::multi_pass<std::istreambuf_iterator<char>> iterator;

        iterator lexFirst = boost::spirit::make_default_multi_pass(rawIterator{reader});
        iterator lexEnd = boost::spirit::make_default_multi_pass(rawIterator());


        typedef lex::lexertl::token<iterator> token;
        typedef lex::lexertl::actor_lexer<token> lexer;
        word_count_tokens<lexer> word_count_lexer;


        lexer::iterator_type iter = word_count_lexer.begin(lexFirst, lexEnd);
        lexer::iterator_type end = word_count_lexer.end();

        while (iter != end && token_is_valid(*iter))
        {
            std::cout<<"token: "<<iter->value()<<", id: "<<iter->id()<<std::endl;
            ++iter;
        }
        assert(0);
    }

}}}
