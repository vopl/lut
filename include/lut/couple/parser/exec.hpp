#pragma once

#include "lut/couple/meta/libraryBuilder.hpp"
#include "lut/couple/parser/config.hpp"
#include "lut/couple/parser/errorInfo.hpp"

#include <vector>
#include <string>

namespace lut { namespace couple { namespace parser
{
    bool exec(const std::vector<std::string> &fileNames, const Config &cfg, std::vector<ErrorInfo> &errs, meta::LibraryBuilder &lb);
}}}
