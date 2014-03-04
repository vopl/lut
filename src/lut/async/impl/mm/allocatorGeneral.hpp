#ifndef _LUT_ASYNC_IMPL_MM_ALLOCATORGENERAL_HPP_
#define _LUT_ASYNC_IMPL_MM_ALLOCATORGENERAL_HPP_

#include "lut/async/impl/mm/allocatorThread.hpp"
#include <mutex>
#include <bitset>

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
        static std::size_t vspaceSize();
        static constexpr std::size_t vspaceHeaderSize();
        char *vspaceBegin() const;
        char *vspaceEnd() const;

    private:
        bool vmAccessHandler(void *addr);
        static bool s_vmAccessHandler(void *addr);

    private:
        size_t allocatorThreadIndex(const void *addr);
        AllocatorThread *allocatorThread(const void *addr);
        AllocatorThread *allocatorThread(size_t index);

    private:
        static AllocatorGeneral _instance;
        static const Config &_config;

        static const size_t _badIndex = -1;

        char *_vspaceBegin;
        char *_vspaceEnd;

        struct Header
        {
            std::bitset<4096>   _threadsUseMask;
            static_assert(Config::_maxThreadsAmount <= decltype(_threadsUseMask)().size(), "_threadsUseMask too small");

            char *_threadsArea;
        };

        Header *_header;
        std::mutex _mtx;
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
