#include "lut/coupling/parser/impl/grammarError.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    GrammarError::GrammarError(TokIterator pos, const std::string &msg)
        : std::runtime_error(msg)
        , _pos(pos)
    {
    }

    const TokIterator &GrammarError::pos() const
    {
        return _pos;
    }

}}}}
