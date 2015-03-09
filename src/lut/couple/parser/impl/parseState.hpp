#pragma once

#include "lut/couple/parser/config.hpp"
#include "lut/couple/parser/errorInfo.hpp"

#include <vector>
#include <map>
#include <fstream>
#include <memory>

namespace  lut { namespace couple { namespace parser { namespace impl
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
