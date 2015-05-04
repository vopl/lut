#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkScopedName()
    {
        ////////////////////////////////////////////////////////////////////////////////
        scopedName =
            (
                (
                    toks.dcolon
                        [qi::_val = phx::construct<ScopedName>(phx::new_<SScopedName>())]
                        [phx::bind(&SScopedName::root, deref(qi::_val)) = true]
                        [phx::bind(&SScopedName::pos, deref(qi::_val)) = qi::_1] >>
                        (
                            name[phx::push_back(phx::bind(&SScopedName::values, deref(qi::_val)), qi::_1)] |
                            error(+"type name expected")
                        )
                ) |
                (
                    name
                        [qi::_val = phx::construct<ScopedName>(phx::new_<SScopedName>())]
                        [phx::push_back(phx::bind(&SScopedName::values, deref(qi::_val)), qi::_1)]
                        [phx::bind(&SScopedName::pos, deref(qi::_val)) = phx::bind(&SName::pos, deref(qi::_1))]
                )
            ) >>
            *(
                toks.dcolon >>
                (
                    name[phx::push_back(phx::bind(&SScopedName::values, deref(qi::_val)), qi::_1)] |
                    error(+"type name expected")
                )
            );
    }

}}}}
