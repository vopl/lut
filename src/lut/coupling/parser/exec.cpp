#include "lut/stable.hpp"
#include "lut/coupling/parser/exec.hpp"
#include "lut/coupling/parser/impl/parse.hpp"
#include "lut/coupling/parser/impl/grammar.hpp"

//TODO ошибки, набивка мета-либы


namespace lut { namespace coupling { namespace parser
{
    bool exec(const std::vector<std::string> &fileNames, const Config &cfg, std::vector<ErrorInfo> &errs, meta::Library &lib)
    {
        std::vector<impl::Decl> parseResult;

        for(const std::string &fileName : fileNames)
        {
            if(!impl::parse(fileName, cfg, errs, parseResult))
            {
                return false;
            }
        }

        //TODO resolve

        //TODO fill lib

        return true;
    }

}}}
