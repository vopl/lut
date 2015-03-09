#include "lut/couple/parser/impl/grammar.hpp"

namespace lut { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkTypeUse()
    {
        ////////////////////////////////////////////////////////////////////////////////
        typeUse %=
            primitive | list | set | map | ptr | array | scopedName;
    }

}}}}
