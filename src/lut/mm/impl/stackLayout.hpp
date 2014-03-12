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
        template <bool, bool> friend class StackLayout;

    public:
        StackLayout()
        {
            std::size_t mappedSize = sizeof(_stackStateArea) + Config::_stackKeepProtectedBytes;

            if(mappedSize % Config::_pageSize)
            {
                mappedSize = (mappedSize / Config::_pageSize + 1) * Config::_pageSize;
            }

            char *area = reinterpret_cast<char *>(this);

            vm::protect(
                        area,
                        mappedSize,
                        true);

            new (&stackState()) StackState;

            stackState()._userspaceBegin = area + offsetof(StackLayout, _userArea);
            stackState()._userspaceMapped = area + mappedSize;
            stackState()._userspaceEnd = area + offsetof(StackLayout, _userArea) + sizeof(UserArea);
        }

        ~StackLayout()
        {
            const char *area = reinterpret_cast<const char *>(this);
            std::size_t mappedSize = stackState()._userspaceMapped - area;
            assert(! (mappedSize % Config::_pageSize));

            stackState().~StackState();

            vm::protect(
                        area,
                        mappedSize,
                        false);
        }

        static const lut::mm::Stack *impl2Face(StackLayout *impl)
        {
            return &impl->stackState();
        }

        static StackLayout *face2Impl(const lut::mm::Stack *face)
        {
            char *ss = reinterpret_cast<char *>(static_cast<StackState *>(const_cast<lut::mm::Stack *>(face)));
            return reinterpret_cast<StackLayout *>(ss - offsetof(StackLayout, _stackStateArea));
        }

        void compact()
        {
            std::uintptr_t imappedEnd = reinterpret_cast<std::uintptr_t>(alloca(1)) + Config::_stackKeepProtectedBytes;

            if(imappedEnd % Config::_pageSize)
            {
                imappedEnd = (imappedEnd / Config::_pageSize + 1) * Config::_pageSize;
            }

            char *mappedEnd = reinterpret_cast<char *>(imappedEnd);
            assert(mappedEnd > reinterpret_cast<char *>(this));
            assert(mappedEnd < reinterpret_cast<char *>(this) + sizeof(StackLayout));

            if(mappedEnd >= stackState()._userspaceMapped)
            {
                return;
            }

            vm::protect(
                        mappedEnd,
                        stackState()._userspaceMapped - mappedEnd,
                        false);

            stackState()._userspaceMapped = mappedEnd;
        }

    private:
        StackState &stackState()
        {
            return *reinterpret_cast<StackState *>(&_stackStateArea);
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
        StackLayout()
        {
        }

        ~StackLayout()
        {
        }

        static const lut::mm::Stack *impl2Face(StackLayout *impl)
        {
            return WithoutGuard::impl2Face(reinterpret_cast<WithoutGuard *>(reinterpret_cast<char *>(impl) + offsetof(StackLayout, _withoutGuard)));
        }

        static StackLayout *face2Impl(const lut::mm::Stack *face)
        {
            return reinterpret_cast<StackLayout *>(reinterpret_cast<char *>(WithoutGuard::face2Impl(face)) - offsetof(StackLayout, _withoutGuard));
        }

        void compact()
        {
            return _withoutGuard.compact();
        }

    private:
        StackState &stackState()
        {
            return _withoutGuard.stackState();
        }

    private:
        using GuardArea = std::aligned_storage<Config::_pageSize, Config::_pageSize>::type;
        using WithoutGuard = StackLayout<false, false>;

        WithoutGuard    _withoutGuard;
        GuardArea       _guardArea;
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <>
    class StackLayout<true, false>
    {
        template <bool, bool> friend class StackLayout;

    public:
        StackLayout()
        {
            std::size_t mappedSize = sizeof(_stackStateArea) + Config::_stackKeepProtectedBytes;

            if(mappedSize % Config::_pageSize)
            {
                mappedSize = (mappedSize / Config::_pageSize + 1) * Config::_pageSize;
            }

            char *area = reinterpret_cast<char *>(this);

            vm::protect(
                        area + sizeof(StackLayout) - mappedSize,
                        mappedSize,
                        true);

            new (&stackState()) StackState;

            stackState()._userspaceBegin = area + offsetof(StackLayout, _userArea);
            stackState()._userspaceMapped = area + sizeof(StackLayout) - mappedSize;
            stackState()._userspaceEnd = area + offsetof(StackLayout, _userArea) + sizeof(UserArea);
        }

        ~StackLayout()
        {
            const char *area = reinterpret_cast<const char *>(this);
            const char *mappedBegin = stackState()._userspaceMapped;
            std::size_t mappedSize = area + sizeof(StackLayout) - mappedBegin;
            assert(! (mappedSize % Config::_pageSize));

            stackState().~StackState();

            vm::protect(
                        mappedBegin,
                        mappedSize,
                        false);
        }

        static const lut::mm::Stack *impl2Face(StackLayout *impl)
        {
            return &impl->stackState();
        }

        static StackLayout *face2Impl(const lut::mm::Stack *face)
        {
            char *ss = reinterpret_cast<char *>(static_cast<StackState *>(const_cast<lut::mm::Stack *>(face)));
            return reinterpret_cast<StackLayout *>(ss - offsetof(StackLayout, _stackStateArea));
        }

        void compact()
        {
            std::uintptr_t imappedBegin = reinterpret_cast<std::uintptr_t>(alloca(1)) - Config::_stackKeepProtectedBytes;

            imappedBegin -= imappedBegin % Config::_pageSize;

            char *mappedBegin = reinterpret_cast<char *>(imappedBegin);
            assert(mappedBegin > reinterpret_cast<char *>(this));
            assert(mappedBegin < reinterpret_cast<char *>(this) + sizeof(StackLayout));

            if(mappedBegin >= stackState()._userspaceMapped)
            {
                return;
            }

            vm::protect(
                        mappedBegin,
                        mappedBegin - stackState()._userspaceMapped,
                        false);

            stackState()._userspaceMapped = mappedBegin;
        }

    private:
        StackState &stackState()
        {
            return *reinterpret_cast<StackState *>(&_stackStateArea);
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
        StackLayout()
        {
        }

        ~StackLayout()
        {
        }

        static const lut::mm::Stack *impl2Face(StackLayout *impl)
        {
            return WithoutGuard::impl2Face(reinterpret_cast<WithoutGuard *>(reinterpret_cast<char *>(impl) + offsetof(StackLayout, _withoutGuard)));
        }

        static StackLayout *face2Impl(const lut::mm::Stack *face)
        {
            return reinterpret_cast<StackLayout *>(reinterpret_cast<char *>(WithoutGuard::face2Impl(face)) - offsetof(StackLayout, _withoutGuard));
        }

        void compact()
        {
            return _withoutGuard.compact();
        }

    private:
        StackState &stackState()
        {
            return _withoutGuard.stackState();
        }

    private:
        using GuardArea = std::aligned_storage<Config::_pageSize, Config::_pageSize>::type;
        using WithoutGuard = StackLayout<true, false>;

        GuardArea       _guardArea;
        WithoutGuard    _withoutGuard;
    };
}}}

#endif
