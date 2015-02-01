#include "lut/coupling/parser/impl/grammar.hpp"
#include "lut/coupling/parser/impl/parse.hpp"
#include "lut/coupling/parser/impl/grammarError.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    std::vector<Decl> Grammar::doInclude(const Token::token_value_type &str)
    {
        std::string fileName(std::begin(str), std::end(str));

        assert(fileName.size() >= 2);
        assert('"' == fileName[0] && '"' == fileName[fileName.size()-1]);

        fileName.erase(fileName.begin());
        fileName.erase(fileName.end()-1);

        std::size_t pos{0};
        while(std::string::npos != (pos = fileName.find("\\\"", pos, 2)))
        {
            fileName.erase(fileName.begin() + pos);
        }

        std::vector<Decl> res;

        if(!impl::parse(fileName, _parseState, res))
        {
            throw GrammarError(std::begin(str), "unable to include file");
        }

        return res;
    }

    void Grammar::mkInclude()
    {
        ////////////////////////////////////////////////////////////////////////////////
        include =
            toks.kwinclude[qi::_val = phx::construct<std::vector<Decl>>()] >>
            (
                toks.quotedString[qi::_val = phx::bind(&Grammar::doInclude, this, qi::_1)] |
                error(+"file name expected")
            ) >>
            (toks.semi | error(+"';' expected"));
    }

}}}}
