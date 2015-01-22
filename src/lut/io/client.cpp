#include "lut/stable.hpp"
#include "lut/io/client.hpp"
#include "lut/io/impl/client.hpp"

namespace lut { namespace io
{

    Client::Client()
        : Layout<impl::Client>()
    {
    }

    Client::~Client()
    {
    }

    async::Future<std::error_code, Stream> Client::connect(const Endpoint &endpoint)
    {
        return impl().connect(endpoint);
    }
}}
