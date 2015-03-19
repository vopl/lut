#include <exec.hpp>
#include "impl/parse.hpp"
#include "impl/grammar.hpp"
#include "impl/ast/metaCreator.hpp"
#include "impl/ast/metaLinker.hpp"

namespace dci { namespace couple { namespace parser
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
