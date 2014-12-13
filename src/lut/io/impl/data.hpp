#pragma once

#include <sys/uio.h>

namespace lut { namespace io { namespace impl
{

    class Data
    {
        Data(const Data &) = delete;
        void operator=(const Data &) = delete;

    public:
        static constexpr std::size_t _granula = 1024;

    public:
        Data();
        Data(Data &&);
        ~Data();

        Data &operator=(Data &&);

        void append(Data &&data);
        void append(const char *str, std::size_t size = -1);

        bool empty() const;
        std::size_t size() const;

        void clear();

        std::size_t segmentsAmount() const;
        void fillIovec(iovec *iov) const;

        void dropFirst(std::size_t size);
        void dropLast(std::size_t size);

    private:
        struct Segment
        {
            char _buffer[Data::_granula];

            std::uint32_t _offset;
            std::uint32_t _size;
            Segment *_next;
        };
        static constexpr std::size_t _segmentSize = sizeof(Segment);

        std::size_t _size;
        Segment *_first;
        Segment *_last;


    };
}}}
