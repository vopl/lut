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
        ContextEngine(size_t stackSize);
        virtual ~ContextEngine();

    protected:
        bool haveStack();
        void *getStackBegin();
        void *getStackEnd();

        void activateFrom(ContextEngine *current);

    private:
        boost::context::fcontext_t *_ctx;

    private:
        static void s_contextProc(intptr_t callable);
        virtual void contextProc() = 0;
    };
}}}

#endif
