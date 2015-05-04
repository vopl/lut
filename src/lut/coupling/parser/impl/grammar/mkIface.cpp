#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkIface()
    {
        ////////////////////////////////////////////////////////////////////////////////
        iface =
            toks.kwiface[qi::_val = phx::construct<Iface>(phx::new_<SIface>())] >>
            (
                name[phx::bind(&SIface::name, deref(qi::_val)) = qi::_1] |
                error(+"interface name expected")
            ) >>
            -baseIfaces[phx::bind(&SIface::bases, deref(qi::_val)) = qi::_1] >>
            (toks.ocb | error(+"'{' expected")) >>
            *(
                //decls[phx::insert(phx::bind(&SIface::decls, deref(qi::_val)), phx::end(phx::bind(&SIface::decls, deref(qi::_val))), phx::begin(qi::_1), phx::end(qi::_1))] |

                decls[
                    phx::for_each(
                        qi::_1,
                        phx::lambda
                            (phx::local_names::_a = phx::bind(&SIface::decls, deref(qi::_val)))
                            [phx::push_back(phx::local_names::_a, phx::arg_names::_1)]
                    )
                ] |

                method[phx::push_back(phx::bind(&SIface::fields, deref(qi::_val)), qi::_1)]
            ) >>
            (toks.ccb | error(+"'}' expected"));
    }

}}}}
