#pragma once

#include <sys/uio.h>
#include "lut/io/impl/data/segment.hpp"

namespace lut { namespace io { namespace impl
{

    class Data
    {
        Data(const Data &) = delete;
        void operator=(const Data &) = delete;

    public:
        Data();
        Data(Data &&from);
        Data(std::size_t size, data::Segment *first, data::Segment *last);
        ~Data();

        Data &operator=(Data &&);

        void append(Data &&data);
        void append(const char *str, std::size_t size);

        bool empty() const;
        std::size_t size() const;

        void clear();

        std::size_t segmentsAmount() const;
        void fillIovec(iovec *iov) const;

        void dropFirst(std::size_t size);
        void dropLast(std::size_t size);

        Data detachFirst(std::size_t size);
        Data detachLast(std::size_t size);

        std::string toString();

    private:
        static constexpr std::size_t _segmentSize = sizeof(data::Segment);

        std::size_t _size;
        data::Segment *_first;
        data::Segment *_last;
    };
}}}
