#pragma once

#include "lut/couple/meta/alias.hpp"
#include "lut/couple/meta/array.hpp"
#include "lut/couple/meta/enum.hpp"
#include "lut/couple/meta/iface.hpp"
#include "lut/couple/meta/list.hpp"
#include "lut/couple/meta/map.hpp"
#include "lut/couple/meta/ptr.hpp"
#include "lut/couple/meta/primitive.hpp"
#include "lut/couple/meta/scope.hpp"
#include "lut/couple/meta/set.hpp"
#include "lut/couple/meta/struct.hpp"
#include "lut/couple/meta/variant.hpp"

#include <string>
#include <system_error>

namespace lut { namespace couple { namespace meta
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
