#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkDecls()
    {
        ////////////////////////////////////////////////////////////////////////////////
        decls %=
            *decl;
    }

}}}}
