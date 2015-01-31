#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkBases()
    {
        ////////////////////////////////////////////////////////////////////////////////
        bases =
            toks.colon[qi::_val = phx::construct<Bases>(phx::new_<SBases>())] >>
            (
                (
                    typeName[phx::push_back(phx::bind(&SBases::names, deref(qi::_val)), qi::_1)] |
                    error(+"type name expected")
                ) % toks.comma
            );
    }

}}}}
