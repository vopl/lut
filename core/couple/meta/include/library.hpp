#pragma once

#include "alias.hpp"
#include "array.hpp"
#include "enum.hpp"
#include "iface.hpp"
#include "list.hpp"
#include "map.hpp"
#include "ptr.hpp"
#include "primitive.hpp"
#include "scope.hpp"
#include "set.hpp"
#include "struct.hpp"
#include "variant.hpp"

#include <string>
#include <system_error>

namespace dci { namespace couple { namespace meta
{
    enum class LoadResult
    {
        ok,
        corruptedFile,
        malformedFile,
    };

    class Library
        : public himpl::FaceLayout<impl::Library>
    {
    public:
        Library();
        ~Library();

    public:// changes
        LoadResult load(const std::string &file);
        //save
    };
}}}
