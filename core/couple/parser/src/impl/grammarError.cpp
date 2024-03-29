#include "grammarError.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    GrammarError::GrammarError(TokIterator pos, const std::string &msg)
        : std::runtime_error(msg)
        , _pos(pos->value().begin())
    {
    }

    GrammarError::GrammarError(CharIterator pos, const std::string &msg)
        : std::runtime_error(msg)
        , _pos(pos)
    {
    }

    const CharIterator &GrammarError::pos() const
    {
        return _pos;
    }

}}}}
