#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkMethod()
    {
        ////////////////////////////////////////////////////////////////////////////////
        method =
            (
                toks.kwin [qi::_a = MethodDirection::in ] |
                toks.kwout[qi::_a = MethodDirection::out]
            )
                [qi::_val = phx::construct<Method>(phx::new_<SMethod>())]
                [phx::bind(&SMethod::direction, deref(qi::_val)) = qi::_a] >>
            (
                -toks.kwnowait[phx::bind(&SMethod::nowait, deref(qi::_val)) = true]
            ) >>
            (
                typeUse[phx::bind(&SMethod::resultType, deref(qi::_val)) = qi::_1] |
                error(+"method result type expected")
            ) >>
            (
                name[phx::bind(&SMethod::name, deref(qi::_val)) = qi::_1] |
                error(+"method name expected")
            ) >>
            (toks.ob | error(+"'(' expected")) >>
            (
                -(methodParam[phx::push_back(phx::bind(&SMethod::params, deref(qi::_val)), qi::_1)] % toks.comma)
            ) >>
            (toks.cb | error(+"')' expected")) >>
            (toks.semi | error(+"';' expected"));
    }

}}}}
