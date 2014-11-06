#pragma once

namespace lut { namespace mm { namespace vm
{
    typedef bool (*TVmAccessHandler)(void *addr);

    bool init(TVmAccessHandler accessHandler);
    bool deinit(TVmAccessHandler accessHandler);

    void *alloc(std::size_t size);
    bool free(void *addr, std::size_t size);

    bool protect(void *addr, std::size_t size, bool access);
}}}
