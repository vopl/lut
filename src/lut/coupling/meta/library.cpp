#include "lut/stable.hpp"
#include "lut/coupling/meta/library.hpp"
#include "lut/coupling/meta/impl/library.hpp"

namespace lut { namespace coupling { namespace meta
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
