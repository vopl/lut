#include <client.hpp>
#include "impl/client.hpp"

namespace dci { namespace io
{

    Client::Client()
        : FaceLayout<impl::Client>()
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
