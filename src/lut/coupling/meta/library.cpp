#include "lut/stable.hpp"
#include "lut/coupling/meta/library.hpp"

namespace lut { namespace coupling { namespace meta
{
    LoadResult Library::load(const std::string &file)
    {
        //TODO serialize to internal binary form
        return LoadResult::malformedFile;
    }

    bool Library::resolve()
    {
        assert(0);
    }

}}}
