#include "lut/mm/stable.hpp"
#include "lut/mm/impl/buffer.hpp"

namespace lut { namespace mm { namespace impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Buffer::Buffer()
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Buffer::~Buffer()
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Buffer::vmAccessHandler(std::uintptr_t offset)
    {
        assert(!"not impl yet");
    }
}}}
