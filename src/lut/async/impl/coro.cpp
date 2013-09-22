#include "lut/async/stable.hpp"
#include "lut/async/impl/coro.hpp"

namespace lut { namespace async { namespace impl
{

    Coro::Coro()
        : Context(1024*32)
    {

    }

    Coro::~Coro()
    {

    }

    void Coro::setCode(const Task &code)
    {
        assert(0);
    }

    void Coro::setCode(Task &&code)
    {
        assert(0);
    }

    void Coro::contextProc()
    {

    }

}}}
