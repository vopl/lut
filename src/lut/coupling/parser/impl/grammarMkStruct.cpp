#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkStruct()
    {
        ////////////////////////////////////////////////////////////////////////////////
        struct_ =
            toks.kwstruct[qi::_val = phx::construct<Struct>(phx::new_<SStruct>())] >>
            (
                typeName[phx::bind(&SStruct::name, deref(qi::_val)) = qi::_1] |
                error(+"structure name expected")
            ) >>
            -bases[phx::bind(&SStruct::bases, deref(qi::_val)) = qi::_1]>>
            (toks.ocb | error(+"'{' expected")) >>
            *(
                decl[phx::push_back(phx::bind(&SStruct::decls, deref(qi::_val)), qi::_1)] |
                structField[phx::push_back(phx::bind(&SStruct::fields, deref(qi::_val)), qi::_1)]
            ) >>
            (toks.ccb | error(+"'}' expected"));
    }

}}}}
