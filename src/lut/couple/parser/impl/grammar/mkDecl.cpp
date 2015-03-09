#include "lut/couple/parser/impl/grammar.hpp"

namespace lut { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkDecl()
    {
        ////////////////////////////////////////////////////////////////////////////////
        decl %=
            alias | variant | struct_ | enum_ | iface | scope;
    }

}}}}
