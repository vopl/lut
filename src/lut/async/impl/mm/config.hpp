#ifndef _LUT_ASYNC_IMPL_MM_CONFIG_HPP_
#define _LUT_ASYNC_IMPL_MM_CONFIG_HPP_

#include <cstddef>

namespace lut { namespace async { namespace impl { namespace mm
{

    struct ConfigMemory
    {
        std::size_t pageSize() const
        {
            return 4096;
        }
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

        std::size_t stackIndexBittness() const
        {
            return 5;
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

        std::size_t blockIndexBittness() const
        {
            return 5;
        }
    };

    struct ConfigThreads
    {
        std::size_t maxThreadsAmount() const
        {
            return 16;
        }
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
