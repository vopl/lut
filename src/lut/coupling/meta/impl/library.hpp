#pragma once

#include "lut/himpl/implLayout.hpp"
#include "lut/coupling/meta/library.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Library
        : public himpl::ImplLayout<Library>
    {
    public:
        Library();
        ~Library();

    public:// changes
        LoadResult load(const std::string &file);

    };

}}}}
