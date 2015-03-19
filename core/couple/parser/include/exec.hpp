#pragma once

#include <dci/couple/meta/libraryBuilder.hpp>
#include "config.hpp"
#include "errorInfo.hpp"

#include <vector>
#include <string>

namespace dci { namespace couple { namespace parser
{
    bool exec(const std::vector<std::string> &fileNames, const Config &cfg, std::vector<ErrorInfo> &errs, meta::LibraryBuilder &lb);
}}}
