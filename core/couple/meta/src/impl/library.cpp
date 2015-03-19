#include "library.hpp"

namespace dci { namespace couple { namespace meta { namespace impl
{
    Library::Library()
    {
    }

    Library::~Library()
    {
    }

    LoadResult Library::load(const std::string &file)
    {
        return LoadResult::malformedFile;
    }

    Library &Library::operator=(LibraryContent &&lc)
    {
        _lc = std::forward<LibraryContent>(lc);
        return *this;
    }

}}}}
