#include "lut/stable.hpp"
#include "lut/coupling/parser/exec.hpp"
#include "lut/coupling/parser/impl/parse.hpp"
#include "lut/coupling/parser/impl/grammar.hpp"
#include "lut/coupling/parser/impl/ast/metaCreator.hpp"
#include "lut/coupling/parser/impl/ast/metaLinker.hpp"

namespace lut { namespace coupling { namespace parser
{
    bool exec(const std::vector<std::string> &fileNames, const Config &cfg, std::vector<ErrorInfo> &errs, meta::LibraryBuilder &lb)
    {
        //parse
        impl::Scope parseResult = impl::parse(fileNames, cfg, errs);
        if(!parseResult)
        {
            return false;
        }

        //fill lib
        impl::ast::MetaCreator(lb).exec(parseResult);
        impl::ast::MetaLinker(lb).exec(parseResult);

        return true;
    }

}}}
