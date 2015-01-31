#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkEnumField()
    {
        ////////////////////////////////////////////////////////////////////////////////
        enumField =
            typeName
                [qi::_val = phx::construct<EnumField>(phx::new_<SEnumField>())]
                [phx::bind(&SEnumField::name, deref(qi::_val)) = qi::_1] >>
            (toks.semi | error(+"';' expected"));
    }

}}}}
