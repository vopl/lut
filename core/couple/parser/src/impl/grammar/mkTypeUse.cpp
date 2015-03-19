#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkTypeUse()
    {
        ////////////////////////////////////////////////////////////////////////////////
        typeUse %=
            primitive | list | set | map | ptr | array | scopedName;
    }

}}}}
