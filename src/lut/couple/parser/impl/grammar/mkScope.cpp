#include "lut/couple/parser/impl/grammar.hpp"

namespace lut { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkScope()
    {
        ////////////////////////////////////////////////////////////////////////////////
        scope =
            toks.kwscope[qi::_val = phx::construct<Scope>(phx::new_<SScope>())] >>
            (
                name[phx::bind(&SScope::name, deref(qi::_val)) = qi::_1] |
                error(+"scope name expected")
            ) >>
            (toks.ocb | error(+"'{' expected")) >>
            -decls[phx::bind(&SScope::decls, deref(qi::_val)) = qi::_1] >>
            (toks.ccb | error(+"'}' expected"));
    }

}}}}