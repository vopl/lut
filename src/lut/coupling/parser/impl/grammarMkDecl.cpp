#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkDecl()
    {
        ////////////////////////////////////////////////////////////////////////////////
        decl %=
            alias | variant | struct_ | enum_ | iface | scope | include;
    }

}}}}
