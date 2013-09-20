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
        ContextEngine(void(* fn)(intptr_t), intptr_t arg, size_t stackSize);
        ~ContextEngine();

    protected:
        void *getStackBegin();
        void *getStackEnd();

    private:
        boost::context::fcontext_t *_ctx;
    };
}}}

#endif
