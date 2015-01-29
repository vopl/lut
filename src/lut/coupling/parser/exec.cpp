#include "lut/stable.hpp"
#include "lut/coupling/parser/exec.hpp"
#include "lut/coupling/parser/impl/tokens.hpp"
#include "lut/coupling/parser/impl/method.hpp"

#include <fstream>


//TODO ошибки, потом граматики


namespace lut { namespace coupling { namespace parser
{
    bool exec(const std::string &fileName, const Config &cfg, meta::Library &lib, std::vector<ErrorInfo> &errs)
    {
        using namespace impl;

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

        CharIterator lexBegin{boost::spirit::multi_pass<std::istreambuf_iterator<char>>{std::istreambuf_iterator<char>{reader}}};
        CharIterator lexEnd{boost::spirit::multi_pass<std::istreambuf_iterator<char>>{std::istreambuf_iterator<char>{}}};




        //tokenize
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

        std::vector<MethodPtr> res;

        bool r;
        try
        {
            r = qi::parse(
                tokIter,
                tokEnd,
                method,
                res);
        }
        catch(qi::expectation_failure<TokIterator> const& x)
        {
            //std::cout << "expected: "; print_info(x.what_);
            //std::cout << "got: \"" << std::string(x.first, x.last) << '"' << std::endl;
        }
        catch(const std::runtime_error &re)
        {
            std::cout << "re: " << re.what() << std::endl;
        }

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
                std::cout<<"partial success, token: " << std::string(t.value().begin(), t.value().end())<<std::endl;

                CharIterator bol = get_line_start(lexBegin, pos);
                std::cout << " at "<< pos.position() << ", "<< std::distance(bol, pos)<<std::endl;
            }
        }
        else
        {
            const Token &t = (*tokIter);
            CharIterator pos = t.value().begin();
            std::cout<<"fail, token: " << std::string(t.value().begin(), t.value().end())<<std::endl;

            CharIterator bol = get_line_start(lexBegin, pos);
            std::cout << " at "<< pos.position() << ", "<< std::distance(bol, pos)<<std::endl;
        }

        assert(0);
    }

}}}
