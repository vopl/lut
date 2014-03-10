#ifndef _LUT_MM_IMPL_ALLOCATOR_HPP_
#define _LUT_MM_IMPL_ALLOCATOR_HPP_

#include "lut/mm/impl/thread.hpp"
#include <mutex>
#include <bitset>

namespace lut { namespace mm { namespace impl
{
    class Allocator
    {
        Allocator(const Allocator &) = delete;
        void operator=(const Allocator &) = delete;

    private:
        Allocator();
        ~Allocator();

    public:
        static Allocator &instance();

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

        struct Header
        {
            std::bitset<4096>   _threadsUseMask;
            static_assert(Config::_maxThreadsAmount <= decltype(_threadsUseMask)().size(), "_threadsUseMask too small");

            std::mutex _mtx;
        };

        Header &header();

        size_t threadIndex(const void *addr);
        Thread *thread(const void *addr);
        Thread *thread(size_t index);

    private:
        class Instantiator
        {
        public:
            Instantiator();
            ~Instantiator();
        };
        friend class Instantiator;

    private:
        static Allocator *_instance;
        static Instantiator _instantiator;
        static const Config &_config;

        static const size_t _badIndex = -1;


        using HeaderArea = std::aligned_storage<sizeof(Header), Config::_pageSize>::type;
        HeaderArea _headerArea;

        static_assert(sizeof(Thread) == (sizeof(Thread) & (~(Config::_pageSize-1))), "Thread must be page mutipled");
        using ThreadsArea = std::aligned_storage<sizeof(Thread)*Config::_maxThreadsAmount, Config::_pageSize>::type;
        ThreadsArea _threadsArea;
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <size_t size>
    void *Allocator::bufferAlloc()
    {
        return Thread::instance().bufferAlloc<size>();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <size_t size>
    bool Allocator::bufferFree(void *buffer)
    {
        Thread *at = thread(buffer);
        assert(at);
        if(!at)
        {
            return false;
        }

        if(at == &Thread::instance())
        {
            return at->bufferFree<size>(buffer);
        }
        return at->bufferFreeFromOtherThread<size>(buffer);
    }


}}}

#endif
