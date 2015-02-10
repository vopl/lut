#pragma once

#include "lut/coupling/meta/alias.hpp"
#include "lut/coupling/meta/array.hpp"
#include "lut/coupling/meta/enum.hpp"
#include "lut/coupling/meta/iface.hpp"
#include "lut/coupling/meta/list.hpp"
#include "lut/coupling/meta/map.hpp"
#include "lut/coupling/meta/ptr.hpp"
#include "lut/coupling/meta/primitive.hpp"
#include "lut/coupling/meta/scope.hpp"
#include "lut/coupling/meta/set.hpp"
#include "lut/coupling/meta/struct.hpp"
#include "lut/coupling/meta/variant.hpp"

#include <string>
#include <system_error>

namespace lut { namespace coupling { namespace meta
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
