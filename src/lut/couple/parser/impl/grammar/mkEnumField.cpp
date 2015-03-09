#include "lut/couple/parser/impl/grammar.hpp"

namespace lut { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkEnumField()
    {
        ////////////////////////////////////////////////////////////////////////////////
        enumField =
            name
                [qi::_val = phx::construct<EnumField>(phx::new_<SEnumField>())]
                [phx::bind(&SEnumField::name, deref(qi::_val)) = qi::_1] >>
            (toks.semi | error(+"';' expected"));
    }

}}}}
