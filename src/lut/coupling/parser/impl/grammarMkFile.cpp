#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkFile()
    {
        ////////////////////////////////////////////////////////////////////////////////
        file =
            qi::eps[qi::_val = phx::construct<Scope>(phx::new_<SScope>())] >
            -decls[phx::bind(&SScope::decls, deref(qi::_val)) = qi::_1];
    }

}}}}
