#ifndef _LUT_ASYNC_IMPL_CONTEXTENGINE_BOOSTCONTEXT_HPP_
#define _LUT_ASYNC_IMPL_CONTEXTENGINE_BOOSTCONTEXT_HPP_

#include <cstdint>
#include <boost/context/all.hpp>

namespace lut { namespace async { namespace impl
{
    class ContextEngine
    {
    public:
        ContextEngine();

        void constructRoot();
        void destructRoot();

        void constructCoro(size_t sizeWithStack, void(*f)(intptr_t), intptr_t arg);
        void destructCoro();

        void switchTo(ContextEngine *to);

    private:
        boost::context::fcontext_t *_ctx;
        intptr_t _coroArg;

        char _buffer[sizeof(boost::context::fcontext_t)];
    };
}}}

#endif
