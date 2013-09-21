#ifndef _LUT_ASYNC_IMPL_CONTEXTENGINE_BOOSTCONTEXT_HPP_
#define _LUT_ASYNC_IMPL_CONTEXTENGINE_BOOSTCONTEXT_HPP_

#include <cstdint>
#include <boost/context/all.hpp>
#include <functional>

namespace lut { namespace async { namespace impl
{
    class ContextEngine
    {
    public:
        ContextEngine();
        ContextEngine(const std::function<void()> &fn, size_t stackSize);
        ~ContextEngine();

    protected:
        void *getStackBegin();
        void *getStackEnd();

    private:
        boost::context::fcontext_t *_ctx;

        size_t _stackSize;
        char *_stack;

        std::function<void()> _fn;

    private:
        static void s_contextProc(intptr_t);
    };
}}}

#endif
