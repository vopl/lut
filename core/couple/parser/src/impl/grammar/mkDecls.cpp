#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkDecls()
    {
        ////////////////////////////////////////////////////////////////////////////////
        decls %=
            +(+decl | include);
    }

}}}}
