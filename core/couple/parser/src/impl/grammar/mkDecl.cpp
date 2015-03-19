#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkDecl()
    {
        ////////////////////////////////////////////////////////////////////////////////
        decl %=
            alias | variant | struct_ | enum_ | iface | scope;
    }

}}}}
