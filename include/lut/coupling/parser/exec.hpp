#pragma once

#include "lut/coupling/meta/library.hpp"
#include "lut/coupling/parser/config.hpp"
#include "lut/coupling/parser/errorInfo.hpp"

namespace lut { namespace coupling { namespace parser
{
    bool exec(const std::string &fileName, const Config &cfg, meta::Library &lib, std::vector<ErrorInfo> &errs);
}}}
