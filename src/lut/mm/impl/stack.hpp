#ifndef _LUT_MM_IMPL_STACK_HPP_
#define _LUT_MM_IMPL_STACK_HPP_

#include "lut/mm/impl/stackLayout.hpp"

namespace lut { namespace mm { namespace impl
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

#endif
