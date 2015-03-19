#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkStructField()
    {
        ////////////////////////////////////////////////////////////////////////////////
        structField =
            typeUse
                [qi::_val = phx::construct<StructField>(phx::new_<SStructField>())]
                [phx::bind(&SStructField::type, deref(qi::_val)) = qi::_1] >>
            (
                name[phx::bind(&SStructField::name, deref(qi::_val)) = qi::_1] |
                error(+"field name expected")
            ) >>
            (toks.semi | error(+"';' expected"));
    }

}}}}
