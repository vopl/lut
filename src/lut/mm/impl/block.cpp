#include "lut/mm/stable.hpp"
#include "lut/mm/impl/block.hpp"

namespace lut { namespace mm { namespace impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Block::Block()
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Block::~Block()
    {

    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Block::vmAccessHandler(std::uintptr_t offset)
    {
        assert(!"not impl yet");
    }
}}}
