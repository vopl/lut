#include "../grammar.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkMethodParam()
    {
        ////////////////////////////////////////////////////////////////////////////////
        methodParam =
            typeUse
                [qi::_val = phx::construct<MethodParam>(phx::new_<SMethodParam>())]
                [phx::bind(&SMethodParam::type, deref(qi::_val)) = qi::_1] >>
            -name[phx::bind(&SMethodParam::name, deref(qi::_val)) = qi::_1];
    }

}}}}
