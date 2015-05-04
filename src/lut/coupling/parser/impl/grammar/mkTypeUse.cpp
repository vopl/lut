#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkTypeUse()
    {
        ////////////////////////////////////////////////////////////////////////////////
        typeUse %=
            primitive | list | set | map | ptr | array | scopedName;
    }

}}}}
