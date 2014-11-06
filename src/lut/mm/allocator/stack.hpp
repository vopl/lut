#pragma once

#include "lut/mm/allocator/stackLayout.hpp"

namespace lut { namespace mm { namespace allocator
{
    class Stack
        : public StackLayout<Config::_stackGrowsDown, Config::_stackUseGuardPage>
    {
        using Layout = StackLayout<Config::_stackGrowsDown, Config::_stackUseGuardPage>;

    public:
        Stack();
        ~Stack();

        void compact();

        static const lut::mm::Stack *impl2Face(Stack *impl);
        static Stack *face2Impl(const lut::mm::Stack *face);

    public:
        bool vmAccessHandler(std::uintptr_t offset);

    private:
    };
}}}
