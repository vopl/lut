#include <library.hpp>
#include "impl/library.hpp"

namespace dci { namespace couple { namespace meta
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
