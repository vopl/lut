#pragma once

#include "lut/himpl/faceLayout.hpp"
#if !GENERATE_SIZEPROVIDER
#   include "lut/io/sizeProvider.hpp"
#endif

#include <string>

struct iovec;

namespace lut { namespace io
{
    namespace impl
    {
        class Data;
    }

    class Data
        : public himpl::FaceLayout<impl::Data>
    {
    private:
        Data(const Data &) = delete;
        void operator=(const Data &) = delete;

    public:
        Data();
        Data(Data &&);
        Data(impl::Data &&);
        ~Data();

        Data &operator=(Data &&);

        bool empty() const;
        std::size_t size() const;

        void append(Data &&data);
        void append(const char *str, std::size_t size = -1);

        void clear();

        Data &operator<<(const char *str);

        std::size_t segmentsAmount() const;
        void fillIovec(iovec *iov) const;

        void dropFirst(std::size_t size);
        void dropLast(std::size_t size);

        Data detachFirst(std::size_t size);
        Data detachLast(std::size_t size);

        std::string toString();
    };

}}
