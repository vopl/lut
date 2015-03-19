#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkEnum()
    {
        ////////////////////////////////////////////////////////////////////////////////
        enum_ =
            toks.kwenum[qi::_val = phx::construct<Enum>(phx::new_<SEnum>())] >>
            (
                name[phx::bind(&SEnum::name, deref(qi::_val)) = qi::_1] |
                error(+"enum name expected")
            ) >>
            -baseEnums[phx::bind(&SEnum::bases, deref(qi::_val)) = qi::_1] >>
            (toks.ocb | error(+"'{' expected")) >>
            *(
                enumField[phx::push_back(phx::bind(&SEnum::fields, deref(qi::_val)), qi::_1)]
            ) >>
            (toks.ccb | error(+"'}' expected"));
    }

}}}}
