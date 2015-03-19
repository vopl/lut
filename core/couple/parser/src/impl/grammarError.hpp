#pragma once

#include "tokenizer.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    class GrammarError
        : public std::runtime_error
    {
    public:
        GrammarError(TokIterator pos, const std::string &msg);
        GrammarError(CharIterator pos, const std::string &msg);

        const CharIterator &pos() const;

    private:
        CharIterator _pos;
    };

}}}}
