#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkInclude()
    {
        ////////////////////////////////////////////////////////////////////////////////
        include =
            toks.kwinclude[qi::_val = phx::construct<Include>(phx::new_<SInclude>())] >>
            (
                toks.quotedString[phx::bind(&SInclude::target, deref(qi::_val)) = phx::construct<std::string>(phx::begin(qi::_1), phx::end(qi::_1))] |
                error(+"file name expected")
            ) >>
            (toks.semi | error(+"';' expected"));
    }

}}}}
