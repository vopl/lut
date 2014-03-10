#ifndef _LUT_MM_IMPL_CONFIG_HPP_
#define _LUT_MM_IMPL_CONFIG_HPP_

#include <cstddef>

namespace lut { namespace mm { namespace impl
{

    struct ConfigMemory
    {
        static const std::size_t    _pageSize {4096};
    };

    struct ConfigStack
    {
        static const std::size_t    _stackPages {256};
        static const std::size_t    _stackGrowsDown {true};
        static const bool           _stackUseGuardPage {true};
        static const std::size_t    _stackAlwaysProtectedBytes {1024};

        static const std::size_t    _stacksAmount {1024*1024};

    };


    struct ConfigHeap
    {
        static const std::size_t    _bufferPages {64};
        static const std::size_t    _buffersAmount {1024*1024};
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
    };

}}}

#endif
