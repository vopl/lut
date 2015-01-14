#pragma once

#include "lut/mm/newDelete.hpp"

namespace lut { namespace io { namespace impl { namespace data
{
    struct Segment
        : public mm::NewDelete<Segment>
    {
        Segment(std::uint32_t size = 0, std::uint32_t offset = 0, Segment *next=nullptr);

        static constexpr std::size_t _granula = 1024;

        char _buffer[_granula];

        std::uint32_t _size;
        std::uint32_t _offset;
        Segment *_next;
    };

}}}}
