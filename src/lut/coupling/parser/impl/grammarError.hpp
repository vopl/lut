#pragma once

#include "lut/coupling/parser/impl/tokenizer.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
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
