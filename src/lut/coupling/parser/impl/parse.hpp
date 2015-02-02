#pragma once

#include "lut/coupling/parser/config.hpp"
#include "lut/coupling/parser/errorInfo.hpp"
#include "lut/coupling/parser/impl/ast.hpp"
#include "lut/coupling/parser/impl/parseState.hpp"


namespace  lut { namespace coupling { namespace parser { namespace impl
{
    bool parse(const std::string &fileName, ParseState &parseState, std::vector<Decl> &decls);
    bool parse(const std::vector<std::string> &fileNames, const Config &cfg, std::vector<ErrorInfo> &errs, std::vector<Decl> &decls);


}}}}
