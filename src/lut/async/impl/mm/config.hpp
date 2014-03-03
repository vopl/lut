#ifndef _LUT_ASYNC_IMPL_MM_CONFIG_HPP_
#define _LUT_ASYNC_IMPL_MM_CONFIG_HPP_

#include <cstddef>

namespace lut { namespace async { namespace impl { namespace mm
{

    struct ConfigMemory
    {
        std::size_t pageSize() const;
        std::size_t hugePageSize() const;
    };

    struct ConfigStack
    {
        std::size_t stacksAmount() const;

        std::size_t stackPages() const;
        bool        stackUseGuardPage() const;
        std::size_t stackAlwaysProtectedBytes() const;
    };


    struct ConfigHeap
    {
        std::size_t superblocksAmount() const;
        std::size_t superblockPages() const;
    };

    struct ConfigThreads
    {
        std::size_t maxThreadsAmount() const;
    };

    struct Config
    {
        const ConfigMemory&   memory() const;
        const ConfigStack&    stack() const;
        const ConfigHeap&     heap() const;
        const ConfigThreads&  threads() const;
    };

}}}}

#endif
