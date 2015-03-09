#include "lut/stable.hpp"
#include "lut/couple/meta/library.hpp"
#include "lut/couple/meta/impl/library.hpp"

namespace lut { namespace couple { namespace meta
{
    using namespace himpl;

    Library::Library()
        : FaceLayout<impl::Library>()
    {
    }

    Library::~Library()
    {
    }

    LoadResult Library::load(const std::string &file)
    {
        return impl().load(file);
    }

}}}
