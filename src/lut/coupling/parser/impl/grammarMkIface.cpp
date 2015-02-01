#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkIface()
    {
        ////////////////////////////////////////////////////////////////////////////////
        iface =
            toks.kwiface[qi::_val = phx::construct<Iface>(phx::new_<SIface>())] >>
            (
                typeName[phx::bind(&SIface::name, deref(qi::_val)) = qi::_1] |
                error(+"interface name expected")
            ) >>
            -bases[phx::bind(&SIface::bases, deref(qi::_val)) = qi::_1] >>
            (toks.ocb | error(+"'{' expected")) >>
            *(
                decls[phx::insert(phx::bind(&SIface::decls, deref(qi::_val)), phx::end(phx::bind(&SIface::decls, deref(qi::_val))), phx::begin(qi::_1), phx::end(qi::_1))] |
                method[phx::push_back(phx::bind(&SIface::methods, deref(qi::_val)), qi::_1)]
            ) >>
            (toks.ccb | error(+"'}' expected"));
    }

}}}}
