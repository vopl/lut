#include "lut/couple/parser/impl/grammar.hpp"

namespace lut { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkDecls()
    {
        ////////////////////////////////////////////////////////////////////////////////
        decls %=
            +(+decl | include);
    }

}}}}
