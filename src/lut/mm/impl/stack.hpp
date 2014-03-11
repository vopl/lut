#ifndef _LUT_MM_IMPL_STACKINSTANCE_HPP_
#define _LUT_MM_IMPL_STACKINSTANCE_HPP_

#include "lut/mm/impl/stack.hpp"
#include "lut/mm/impl/config.hpp"
#include "lut/mm/stack.hpp"

namespace lut { namespace mm { namespace impl
{
    class Stack
    {
    public:
        Stack();
        ~Stack();

        void compact();

        static const lut::mm::Stack *impl2Face(Stack *impl);
        static Stack *face2Impl(const lut::mm::Stack *face);

    private:

        char _area[Config::_stackPages * Config::_pageSize];

    };
}}}

#endif
