#pragma once

#include "stackLayout.hpp"

namespace dci { namespace mm { namespace allocator
{
    class Stack
        : public StackLayout<Config::_stackGrowsDown, Config::_stackUseGuardPage>
    {
        using Layout = StackLayout<Config::_stackGrowsDown, Config::_stackUseGuardPage>;

    public:
        Stack();
        ~Stack();

        void compact();

        static const dci::mm::Stack *impl2Face(Stack *impl);
        static Stack *face2Impl(const dci::mm::Stack *face);

    public:
        bool vmAccessHandler(std::uintptr_t offset);

    private:
    };
}}}
