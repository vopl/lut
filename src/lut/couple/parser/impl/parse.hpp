#pragma once

#include "lut/couple/parser/config.hpp"
#include "lut/couple/parser/errorInfo.hpp"
#include "lut/couple/parser/impl/ast.hpp"
#include "lut/couple/parser/impl/parseState.hpp"


namespace  lut { namespace couple { namespace parser { namespace impl
{
    Scope parse(const std::string &fileName, ParseState &parseState);
    Scope parse(const std::vector<std::string> &fileNames, const Config &cfg, std::vector<ErrorInfo> &errs);


}}}}
