#include "lut/stable.hpp"
#include "lut/couple/meta/impl/method.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
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
