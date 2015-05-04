#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkName()
    {
        ////////////////////////////////////////////////////////////////////////////////
        name =
            toks.word
                [qi::_val = phx::construct<Name>(phx::new_<SName>())]
                [phx::bind(&SName::value, deref(qi::_val)) = phx::construct<std::string>(phx::begin(qi::_1), phx::end(qi::_1))]
                [phx::bind(&SName::pos, deref(qi::_val)) = qi::_1];
    }

}}}}
