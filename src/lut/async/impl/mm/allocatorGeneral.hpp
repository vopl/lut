#ifndef _LUT_ASYNC_IMPL_MM_ALLOCATORGENERAL_HPP_
#define _LUT_ASYNC_IMPL_MM_ALLOCATORGENERAL_HPP_

#include "lut/async/impl/mm/allocatorThread.hpp"
#include "lut/async/impl/mm/config.hpp"

namespace lut { namespace async { namespace impl { namespace mm
{
    class AllocatorGeneral
    {
        AllocatorGeneral(const AllocatorGeneral &) = delete;
        void operator=(const AllocatorGeneral &) = delete;

    private:
        AllocatorGeneral();

    public:
        static AllocatorGeneral &instance();
        ~AllocatorGeneral();

    public:
        bool threadInit();
        bool threadDeinit();

    public:
        const Stack *stackAlloc();
        bool stackFree(const Stack *stack);
        bool stackCompact(const Stack *stack);

    public:
        template <size_t size>
        void *bufferAlloc();

        template <size_t size>
        bool bufferFree(void *buffer);

    private:
        bool vmAccessHandler(void *addr);
        static bool s_vmAccessHandler(void *addr);

    private:
        static AllocatorGeneral _instance;

        char *_vspaceBegin;
        char *_vspaceEnd;

        struct Header
        {
            std::size_t _threadsAmount;
            bool *_threadsIndex;
            AllocatorThread *_threadsArea;
        };

        Header *_header;
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <size_t size>
    void *AllocatorGeneral::bufferAlloc()
    {
        assert(0);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <size_t size>
    bool AllocatorGeneral::bufferFree(void *buffer)
    {
        assert(0);
    }


}}}}

#endif
