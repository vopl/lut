#pragma once

#include "lut/hiddenImpl/sizeProvider.hpp"
#include "lut/hiddenImpl/sizeFetcher.hpp"
#include <utility>

namespace lut { namespace hiddenImpl
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    class Accessor;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    namespace details
    {
        template <typename... TFaces>
        struct FacesSizeEvaluator
        {
            class Probe : public TFaces... {};
            static const std::size_t _value = SizeFetcher<Probe>::_value;
        };

        template <class Tag, std::size_t implSize, std::size_t facesSize, bool isEqual = implSize==facesSize>
        class Area
        {
        protected:
            static_assert(implSize > facesSize, "impl size must be greater than all faces, possible impl inheritance is different from faces");
            char _space[implSize - facesSize];
        };

        template <class Tag, std::size_t implSize, std::size_t facesSize>
        class Area<Tag, implSize, facesSize, true>
        {
        };
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    class Layout
        : public TFaces...
        , public details::Area<
            Layout<TImpl, TFaces...>,
            sizeProvider<TImpl>::_value,
            details::FacesSizeEvaluator<TFaces...>::_value>
    {
    public:
        using Impl = TImpl;

    protected:
        friend class Accessor;

        Layout();

        template <typename... Arg>
        Layout(const Arg &... args);

        template <typename... Arg>
        Layout(Arg &&... args);

        Layout(const Layout &other);
        Layout(Layout &&other);

        Layout(const Impl &other);
        Layout(Impl &&other);

        ~Layout();

        Layout &operator=(const Layout &other);
        Layout &operator=(Layout &&other);

        Layout &operator=(const Impl &other);
        Layout &operator=(Impl &&other);

        Impl *pimpl();
        Impl &impl();

        const Impl *pimpl() const;
        const Impl &impl() const;
    };




    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    namespace details
    {
        template <typename TImpl, typename... TFaces, std::size_t s1=sizeof(TImpl), std::size_t s2 = sizeof(Layout<TImpl, TFaces...>)>
        void sizeChecker()
        {
            static_assert(sizeof(TImpl)==sizeof(Layout<TImpl, TFaces...>), "inconsistent sizeProvider");
        }
    }



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    Layout<TImpl, TFaces...>::Layout()
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    template <typename... Arg>
    Layout<TImpl, TFaces...>::Layout(const Arg &... args)
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(args...);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    template <typename... Arg>
    Layout<TImpl, TFaces...>::Layout(Arg &&... args)
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(std::forward<Arg>(args)...);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    Layout<TImpl, TFaces...>::Layout(const Layout &other)
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(other.impl());
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    Layout<TImpl, TFaces...>::Layout(Layout &&other)
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(std::move(other.impl()));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    Layout<TImpl, TFaces...>::Layout(const Impl &other)
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(other);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    Layout<TImpl, TFaces...>::Layout(Impl &&other)
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(std::move(other));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    Layout<TImpl, TFaces...>::~Layout()
    {
        pimpl()->~Impl();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    Layout<TImpl, TFaces...> &Layout<TImpl, TFaces...>::operator=(const Layout &other)
    {
        impl() = other.impl();
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    Layout<TImpl, TFaces...> &Layout<TImpl, TFaces...>::operator=(Layout &&other)
    {
        impl() = std::forward<Impl>(other.impl());
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    Layout<TImpl, TFaces...> &Layout<TImpl, TFaces...>::operator=(const Impl &other)
    {
        impl() = other;
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    Layout<TImpl, TFaces...> &Layout<TImpl, TFaces...>::operator=(Impl &&other)
    {
        impl() = std::forward<Impl>(other);
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    TImpl *Layout<TImpl, TFaces...>::pimpl()
    {
        return reinterpret_cast<Impl*>(this);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    TImpl &Layout<TImpl, TFaces...>::impl()
    {
        return *pimpl();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    const TImpl *Layout<TImpl, TFaces...>::pimpl() const
    {
        return reinterpret_cast<const Impl*>(this);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TFaces>
    const TImpl &Layout<TImpl, TFaces...>::impl() const
    {
        return *pimpl();
    }
}}
