#include "lut/stable.hpp"
#include "lut/coupling/parser/exec.hpp"
#include "lut/coupling/parser/impl/parse.hpp"
#include "lut/coupling/parser/impl/grammar.hpp"

//TODO ошибки, набивка мета-либы


namespace lut { namespace coupling { namespace parser
{
    bool exec(const std::vector<std::string> &fileNames, const Config &cfg, std::vector<ErrorInfo> &errs, meta::Library &lib)
    {
        impl::Scope parseResult = impl::parse(fileNames, cfg, errs);

        if(!parseResult)
        {
            return false;
        }

        //TODO fill lib

        return true;
    }

}}}
