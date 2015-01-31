#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkMethodParam()
    {
        ////////////////////////////////////////////////////////////////////////////////
        methodParam =
            typeUse
                [qi::_val = phx::construct<MethodParam>(phx::new_<SMethodParam>())]
                [phx::bind(&SMethodParam::type, deref(qi::_val)) = qi::_1] >>
            -typeName[phx::bind(&SMethodParam::name, deref(qi::_val)) = qi::_1];
    }

}}}}
