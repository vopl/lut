#ifndef _LUT_ASYNC_IMPL_CTX_ENGINE_BOOSTCONTEXT_HPP_
#define _LUT_ASYNC_IMPL_CTX_ENGINE_BOOSTCONTEXT_HPP_

#include <cstdint>
#include <boost/context/all.hpp>

namespace lut { namespace async { namespace impl { namespace ctx
{
    class Engine
            : private boost::context::fcontext_t
    {
    public:
        Engine();

        void constructRoot();
        void destructRoot();

        void constructCoro(char *stackBegin, void(*f)(intptr_t), intptr_t arg);
        void destructCoro();

        void switchTo(Engine *to);

    private:
        intptr_t _coroArg;
    };
}}}}

#endif
