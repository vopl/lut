#ifndef _LUT_ASYNC_IMPL_MM_CONFIG_HPP_
#define _LUT_ASYNC_IMPL_MM_CONFIG_HPP_

#include <cstddef>

namespace lut { namespace async { namespace impl { namespace mm
{

    struct ConfigMemory
    {
        static const std::size_t _pageSize {4096};
    };

    struct ConfigStack
    {
        std::size_t stacksAmount() const
        {
            return 1024*1024;
        }

        std::size_t stackPages() const
        {
            return 256;
        }
        bool        stackUseGuardPage() const
        {
            return true;
        }

        std::size_t stackAlwaysProtectedBytes() const
        {
            return 1024;
        }
    };


    struct ConfigHeap
    {
        std::size_t blocksAmount() const
        {
            return 1024*1024;
        }

        std::size_t blockPages() const
        {
            return 64;
        }
    };

    struct ConfigThreads
    {
        static const std::size_t _maxThreadsAmount {16};
    };

    struct Config
        : public ConfigMemory
        , public ConfigStack
        , public ConfigHeap
        , public ConfigThreads
    {
        static const Config &instance();

    private:
        static const Config _instance;
    };

}}}}

#endif
