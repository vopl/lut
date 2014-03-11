#ifndef _LUT_MM_IMPL_STACKLAYOUT_HPP_
#define _LUT_MM_IMPL_STACKLAYOUT_HPP_

#include "lut/mm/config.hpp"
#include "lut/mm/impl/stackState.hpp"
#include "lut/mm/impl/vm.hpp"

#include <type_traits>

namespace lut { namespace mm { namespace impl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <bool stackGrowsDown, bool stackUseGuardPage>
    class StackLayout;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <>
    class StackLayout<false, false>
    {
    public:
        StackLayout()
        {
            std::size_t mappedSize =
                    (sizeof(_stackStateArea) % Config::_pageSize) ?
                        (sizeof(_stackStateArea) / Config::_pageSize + 1) * Config::_pageSize :
                        sizeof(_stackStateArea);

            vm::protect(
                        this,
                        mappedSize,
                        true);

            new (&stackState()) StackState;

            stackState()._userspaceBegin = (char *)this + sizeof(_stackStateArea);
            stackState()._mappedEnd = (char *)this + mappedSize;
            stackState()._guardBegin = (char *)this + sizeof(StackLayout);
        }

        ~StackLayout();

        void protectTo(const void *addr);

        static const lut::mm::Stack *impl2Face(StackLayout *impl);
        static StackLayout *face2Impl(const lut::mm::Stack *face);

    private:
        StackState &stackState()
        {
            union
            {
                StackStateArea *_ssa;
                StackState *_ss;
            } u;
            u._ssa = &_stackStateArea;
            return *u._ss;
        }

    private:
        using StackStateArea = std::aligned_storage<sizeof(StackState)>::type;
        using UserArea = std::aligned_storage<Config::_stackPages * Config::_pageSize - sizeof(StackStateArea), 1>::type;

        StackStateArea  _stackStateArea;
        UserArea        _userArea;
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <>
    class StackLayout<false, true>
    {
    public:
        StackLayout();
        ~StackLayout();

        void protectTo(const void *addr);

        static const lut::mm::Stack *impl2Face(StackLayout *impl);
        static StackLayout *face2Impl(const lut::mm::Stack *face);

    private:
        StackState &stackState()
        {
            union
            {
                StackStateArea *_ssa;
                StackState *_ss;
            } u;
            u._ssa = &_stackStateArea;
            return *u._ss;
        }

    private:
        using GuardArea = std::aligned_storage<Config::_pageSize, Config::_pageSize>::type;
        using StackStateArea = std::aligned_storage<sizeof(StackState)>::type;
        using UserArea = std::aligned_storage<Config::_stackPages * Config::_pageSize - sizeof(StackStateArea) - sizeof(GuardArea), 1>::type;

        StackStateArea  _stackStateArea;
        UserArea        _userArea;
        GuardArea       _guardArea;
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <>
    class StackLayout<true, false>
    {
    public:
        StackLayout();
        ~StackLayout();

        void protectTo(const void *addr);

        static const lut::mm::Stack *impl2Face(StackLayout *impl);
        static StackLayout *face2Impl(const lut::mm::Stack *face);

    private:
        StackState &stackState()
        {
            union
            {
                StackStateArea *_ssa;
                StackState *_ss;
            } u;
            u._ssa = &_stackStateArea;
            return *u._ss;
        }

    private:
        using StackStateArea = std::aligned_storage<sizeof(StackState)>::type;
        using UserArea = std::aligned_storage<Config::_stackPages * Config::_pageSize - sizeof(StackStateArea), 1>::type;

        UserArea        _userArea;
        StackStateArea  _stackStateArea;
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <>
    class StackLayout<true, true>
    {
    public:
        StackLayout();
        ~StackLayout();

        void protectTo(const void *addr);

        static const lut::mm::Stack *impl2Face(StackLayout *impl);
        static StackLayout *face2Impl(const lut::mm::Stack *face);

    private:
        StackState &stackState()
        {
            union
            {
                StackStateArea *_ssa;
                StackState *_ss;
            } u;
            u._ssa = &_stackStateArea;
            return *u._ss;
        }

    private:
        using GuardArea = std::aligned_storage<Config::_pageSize, Config::_pageSize>::type;
        using StackStateArea = std::aligned_storage<sizeof(StackState)>::type;
        using UserArea = std::aligned_storage<Config::_stackPages * Config::_pageSize - sizeof(StackStateArea) - sizeof(GuardArea), 1>::type;

        GuardArea       _guardArea;
        UserArea        _userArea;
        StackStateArea  _stackStateArea;
    };
}}}

#endif
