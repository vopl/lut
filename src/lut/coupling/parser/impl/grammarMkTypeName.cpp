#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkTypeName()
    {
        ////////////////////////////////////////////////////////////////////////////////
        typeName =
            toks.word
                [qi::_val = phx::construct<TypeName>(phx::new_<STypeName>())]
                [phx::bind(&STypeName::value, deref(qi::_val)) = phx::construct<std::string>(phx::begin(qi::_1), phx::end(qi::_1))]
                [phx::bind(&STypeName::pos, deref(qi::_val)) = qi::_1];
    }

}}}}
