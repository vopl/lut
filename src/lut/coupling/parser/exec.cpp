#include "lut/stable.hpp"
#include "lut/coupling/parser/exec.hpp"
#include "lut/coupling/parser/impl/tokens.hpp"

#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>
#include <fstream>

#include <boost/spirit/include/qi.hpp>


using namespace boost::spirit;
using namespace boost::spirit::ascii;


//TODO ошибки, потом граматики


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
            errs.emplace_back(ErrorInfo {fileName, -1, -1, strerror(errno)});
            return false;
        }

        using CharIterator = boost::spirit::line_pos_iterator<boost::spirit::multi_pass<std::istreambuf_iterator<char>>>;
        CharIterator lexBegin{boost::spirit::multi_pass<std::istreambuf_iterator<char>>{std::istreambuf_iterator<char>{reader}}};
        CharIterator lexEnd{boost::spirit::multi_pass<std::istreambuf_iterator<char>>{std::istreambuf_iterator<char>{}}};


        using Toks = impl::Tokens<CharIterator>;
        using TokIterator = Toks::iterator_type;
        using Token = Toks::Token;


        //tokenize
        Toks toks;
        CharIterator lexIter{lexBegin};
        TokIterator tokBegin = toks.begin(lexIter, lexEnd);
        TokIterator tokEnd = toks.end();

        {
            TokIterator tokIter = tokBegin;
            for(; tokIter != tokEnd; ++tokIter)
            {
                if(!token_is_valid(*tokIter))
                {
                    CharIterator bol = get_line_start(lexBegin, lexIter);
                    errs.emplace_back(ErrorInfo {
                                          fileName,
                                          static_cast<int>(lexIter.position()),
                                          static_cast<int>(std::distance(bol, lexIter)),
                                          "invalid input sequence"});
                    return false;
                }
            }
        }

        //parse
        TokIterator tokIter{tokBegin};
        bool r = qi::parse(tokIter, tokEnd, *(toks.id));

        if(r)
        {
            if(tokIter == tokEnd)
            {
                std::cout<<"success: " << std::string(tokIter, tokEnd)<<std::endl;
            }
            else
            {
                const Token &t = (*tokIter);
                CharIterator pos = t.value().begin();
                std::cout<<"partial success, tail: " << std::string(tokIter, tokEnd)<<std::endl;

                CharIterator bol = get_line_start(lexBegin, pos);
                std::cout << " at "<< pos.position() << ", "<< std::distance(bol, pos)<<std::endl;
            }
        }
        else
        {
            std::cout<<"fail: " << std::string(tokIter, tokEnd) << std::endl;
        }

        assert(0);
    }

}}}
