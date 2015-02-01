#pragma once

#include "lut/coupling/meta/library.hpp"
#include "lut/coupling/parser/config.hpp"
#include "lut/coupling/parser/errorInfo.hpp"

#include <vector>
#include <string>

namespace lut { namespace coupling { namespace parser
{
    bool exec(const std::vector<std::string> &fileNames, const Config &cfg, std::vector<ErrorInfo> &errs, meta::Library &lib);
}}}
