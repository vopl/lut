#include "method.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    Method::Method()
    {

    }

    Method::~Method()
    {

    }


    void Method::setResultType(Type *v)
    {
        _resultType = v;
    }

    void Method::setDirection(CallDirection v)
    {
        _direction = v;
    }

    void Method::setNowait(bool v)
    {
        _nowait = v;
    }

}}}}
