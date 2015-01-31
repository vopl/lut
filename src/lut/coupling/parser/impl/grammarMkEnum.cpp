#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkEnum()
    {
        ////////////////////////////////////////////////////////////////////////////////
        enum_ =
            toks.kwenum[qi::_val = phx::construct<Enum>(phx::new_<SEnum>())] >>
            (
                typeName[phx::bind(&SEnum::name, deref(qi::_val)) = qi::_1] |
                error(+"enum name expected")
            ) >>
            -bases[phx::bind(&SEnum::bases, deref(qi::_val)) = qi::_1] >>
            (toks.ocb | error(+"'{' expected")) >>
            *(
                enumField[phx::push_back(phx::bind(&SEnum::fields, deref(qi::_val)), qi::_1)]
            ) >>
            (toks.ccb | error(+"'}' expected"));
    }

}}}}
