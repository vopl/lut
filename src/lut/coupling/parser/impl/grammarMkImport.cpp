#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkImport()
    {
        ////////////////////////////////////////////////////////////////////////////////
        import =
            toks.kwimport[qi::_val = phx::construct<Import>(phx::new_<SImport>())] >>
            (
                toks.quotedString[phx::bind(&SImport::target, deref(qi::_val)) = phx::construct<std::string>(phx::begin(qi::_1), phx::end(qi::_1))] |
                error(+"file name expected")
            ) >>
            (toks.semi | error(+"';' expected"));
    }

}}}}
