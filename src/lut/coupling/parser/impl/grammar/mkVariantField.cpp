#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkVariantField()
    {
        ////////////////////////////////////////////////////////////////////////////////
        variantField =
            typeUse
                [qi::_val = phx::construct<VariantField>(phx::new_<SVariantField>())]
                [phx::bind(&SVariantField::type, deref(qi::_val)) = qi::_1] >>
            -name[phx::bind(&SVariantField::name, deref(qi::_val)) = qi::_1] >>
            (toks.semi | error(+"';' expected"));
    }

}}}}
