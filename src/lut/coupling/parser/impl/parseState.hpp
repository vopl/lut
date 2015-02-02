#pragma once

#include "lut/coupling/parser/config.hpp"
#include "lut/coupling/parser/errorInfo.hpp"

#include <vector>
#include <map>
#include <fstream>
#include <memory>

namespace  lut { namespace coupling { namespace parser { namespace impl
{
    class Grammar;

    struct ParseState
    {
        const Config &          _cfg;
        std::vector<ErrorInfo> &_errors;

        Grammar                                 *_grammar;
        std::string                             _currentFile;
        std::map<std::string, std::shared_ptr<std::ifstream>>    _processedFiles;
    };


}}}}
