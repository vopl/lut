#ifndef _LUT_ASYNC_IMPL_STACKALLOCATOR_HPP_
#define _LUT_ASYNC_IMPL_STACKALLOCATOR_HPP_

#include "lut/async/impl/sm/allocator.hpp"

namespace lut { namespace async { namespace impl
{
    using StackAllocator = lut::async::impl::sm::Allocator<
        5,//size_t levelBittness,
        4,//size_t deepth,
        4096,//size_t pageSize,
        1024*1024*8/4096,//size_t maxStackPages,
        1,//size_t initialMappedPages=1,
        1024,//size_t unmapBoundBytes=1024,
        true,//bool useGuardPage=true,
        8//size_t bittnessConcurrency=8
    >;

}}}

#endif
