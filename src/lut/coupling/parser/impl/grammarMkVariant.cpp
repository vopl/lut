#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkVariant()
    {
        ////////////////////////////////////////////////////////////////////////////////
        variant =
            toks.kwvariant[qi::_val = phx::construct<Variant>(phx::new_<SVariant>())] >>
            (
                typeName[phx::bind(&SVariant::name, deref(qi::_val)) = qi::_1] |
                error(+"variant name expected")
            ) >>
            -bases[phx::bind(&SVariant::bases, deref(qi::_val)) = qi::_1] >>
            (toks.ocb | error(+"'{' expected")) >>
            *(
                decl[phx::push_back(phx::bind(&SVariant::decls, deref(qi::_val)), qi::_1)] |
                variantField[phx::push_back(phx::bind(&SVariant::fields, deref(qi::_val)), qi::_1)]
            ) >>
            (toks.ccb | error(+"'}' expected"));
    }

}}}}
