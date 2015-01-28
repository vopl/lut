#pragma once

namespace lut { namespace coupling { namespace meta
{
    enum class LoadResult
    {
        ok,
        corruptedFile,
        malformedFile,
    };

    class Library
    {
    public:

        LoadResult load(const std::string &file);

        bool resolve();

    };
}}}
