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
        template <typename... TBaseFaces>
        struct FacesSizeEvaluator
        {
            class Probe : public TBaseFaces... {};
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
    template <class TImpl, class... TBaseFaces>
    class FaceLayout
        : public TBaseFaces...
        , public details::Area<
            FaceLayout<TImpl, TBaseFaces...>,
            sizeProvider<TImpl>::_value,
            details::FacesSizeEvaluator<TBaseFaces...>::_value
        >
    {
    public:
        using Impl = TImpl;
        using BaseFaces = std::tuple<TBaseFaces...>;

    protected:
        friend class Accessor;

        FaceLayout();

        template <typename... Arg>
        FaceLayout(const Arg &... args);

        template <typename... Arg>
        FaceLayout(Arg &&... args);

        FaceLayout(const FaceLayout &other);
        FaceLayout(FaceLayout &&other);

        FaceLayout(const Impl &other);
        FaceLayout(Impl &&other);

        ~FaceLayout();

        FaceLayout &operator=(const FaceLayout &other);
        FaceLayout &operator=(FaceLayout &&other);

        FaceLayout &operator=(const Impl &other);
        FaceLayout &operator=(Impl &&other);

        Impl *pimpl();
        Impl &impl();

        const Impl *pimpl() const;
        const Impl &impl() const;
    };




    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    namespace details
    {
        template <typename TImpl, typename... TBaseFaces, std::size_t s1=sizeof(TImpl), std::size_t s2 = sizeof(FaceLayout<TImpl, TBaseFaces...>)>
        void sizeChecker()
        {
            static_assert(sizeof(TImpl)==sizeof(FaceLayout<TImpl, TBaseFaces...>), "inconsistent sizeProvider");
        }
    }



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...>::FaceLayout()
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    template <typename... Arg>
    FaceLayout<TImpl, TBaseFaces...>::FaceLayout(const Arg &... args)
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(args...);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    template <typename... Arg>
    FaceLayout<TImpl, TBaseFaces...>::FaceLayout(Arg &&... args)
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(std::forward<Arg>(args)...);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...>::FaceLayout(const FaceLayout &other)
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(other.impl());
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...>::FaceLayout(FaceLayout &&other)
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(std::move(other.impl()));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...>::FaceLayout(const Impl &other)
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(other);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...>::FaceLayout(Impl &&other)
    {
        details::sizeChecker<Impl>();
        new (pimpl()) Impl(std::move(other));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...>::~FaceLayout()
    {
        pimpl()->~Impl();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...> &FaceLayout<TImpl, TBaseFaces...>::operator=(const FaceLayout &other)
    {
        impl() = other.impl();
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...> &FaceLayout<TImpl, TBaseFaces...>::operator=(FaceLayout &&other)
    {
        impl() = std::forward<Impl>(other.impl());
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...> &FaceLayout<TImpl, TBaseFaces...>::operator=(const Impl &other)
    {
        impl() = other;
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    FaceLayout<TImpl, TBaseFaces...> &FaceLayout<TImpl, TBaseFaces...>::operator=(Impl &&other)
    {
        impl() = std::forward<Impl>(other);
        return *this;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    TImpl *FaceLayout<TImpl, TBaseFaces...>::pimpl()
    {
        return reinterpret_cast<Impl*>(this);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    TImpl &FaceLayout<TImpl, TBaseFaces...>::impl()
    {
        return *pimpl();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    const TImpl *FaceLayout<TImpl, TBaseFaces...>::pimpl() const
    {
        return reinterpret_cast<const Impl*>(this);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class TImpl, class... TBaseFaces>
    const TImpl &FaceLayout<TImpl, TBaseFaces...>::impl() const
    {
        return *pimpl();
    }
}}
