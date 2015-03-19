#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
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
