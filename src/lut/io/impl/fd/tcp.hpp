#pragma once

#include "lut/async/future.hpp"
#include "lut/io/impl/fd/base.hpp"
#include "lut/io/endpoint.hpp"

namespace lut { namespace io
{
    class Stream;
}}

namespace lut { namespace io { namespace impl { namespace fd
{

    class Tcp
        : public Base
    {
    public:
        Tcp();
        ~Tcp() override;

        void event(int typeFlags) override;
        void close() override;

    private:
    };

}}}}
