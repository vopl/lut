#pragma once

#include <cstddef>

namespace lut { namespace mm
{

    struct ConfigMemory
    {
        static const std::size_t    _pageSize {4096};
        static const std::size_t    _cacheLineSize = 64;
    };

    struct ConfigStack
    {
        static const std::size_t    _stackPages {256};
        static const bool           _stackGrowsDown {true};
        static const bool           _stackUseGuardPage {true};
        static const std::size_t    _stackKeepProtectedBytes {8192};

        static const std::size_t    _stacksAmount {1024};
    };

    struct ConfigHeap
    {
        static constexpr std::size_t _blockSize = 1 << 15;
        static constexpr std::size_t _blocksAmount = 1ULL << 15;

        ////////////////////////////////////////////////////////////////
        static constexpr std::size_t _bigSizeClass = static_cast<std::size_t>(-1);

        ////////////////////////////////////////////////////////////////
        static constexpr std::size_t _sizeGranula = 8;
        static constexpr std::size_t _minSize = _sizeGranula;
        static constexpr std::size_t _maxSize = 1024;

        static constexpr std::size_t _maxSizeClassIndex = (_maxSize-1) / _sizeGranula;

        ////////////////////////////////////////////////////////////////
        static constexpr std::size_t alignUp(std::size_t size, std::size_t alignment)
        {
            return ((size / alignment * alignment) + (size % alignment ? alignment : 0));
        }

        ////////////////////////////////////////////////////////////////
        static constexpr std::size_t alignDown(std::size_t size, std::size_t alignment)
        {
            return (size / alignment * alignment);
        }

        ////////////////////////////////////////////////////////////////
        static constexpr std::size_t evalSizeClassIndex(std::size_t size)
        {
            return !size ?
                        _sizeGranula :
                        size >= _maxSize ?
                            _maxSizeClassIndex :
                            (size-1)/_sizeGranula;
        }

        ////////////////////////////////////////////////////////////////
        static constexpr std::size_t evalSizeClass(std::size_t size)
        {
            return (evalSizeClassIndex(size)+1) * _sizeGranula;
        }


        ////////////////////////////////////////////////////////////////
        static constexpr std::size_t evalPrevSizeClass(std::size_t size)
        {
            return evalSizeClassIndex(size) * _sizeGranula;
        }
    };

    struct Config
        : public ConfigMemory
        , public ConfigStack
        , public ConfigHeap
    {
    };

}}
