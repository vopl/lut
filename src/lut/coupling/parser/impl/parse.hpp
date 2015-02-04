#pragma once

#include "lut/coupling/parser/config.hpp"
#include "lut/coupling/parser/errorInfo.hpp"
#include "lut/coupling/parser/impl/ast.hpp"
#include "lut/coupling/parser/impl/parseState.hpp"


namespace  lut { namespace coupling { namespace parser { namespace impl
{
    Scope parse(const std::string &fileName, ParseState &parseState);
    Scope parse(const std::vector<std::string> &fileNames, const Config &cfg, std::vector<ErrorInfo> &errs);


}}}}
