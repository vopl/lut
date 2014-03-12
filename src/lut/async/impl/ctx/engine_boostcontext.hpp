#ifndef _LUT_ASYNC_IMPL_CTX_ENGINE_BOOSTCONTEXT_HPP_
#define _LUT_ASYNC_IMPL_CTX_ENGINE_BOOSTCONTEXT_HPP_

#include <cstdint>
#include <boost/context/all.hpp>

namespace lut { namespace async { namespace impl { namespace ctx
{
    class Engine
    {
    public:
        Engine();

        void constructRoot();
        void destructRoot();

        void constructCoro(char *stackPtr, std::size_t stackSize, void(*f)(intptr_t), intptr_t arg);
        void destructCoro();

        void switchTo(Engine *to);

    private:
        boost::context::fcontext_t *_ctx;
        bool _ctxIsRoot;
        intptr_t _coroArg;
    };
}}}}

#endif
