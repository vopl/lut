#pragma once

namespace lut { namespace himpl
{
    namespace details
    {

        template <class TFace, class TBaseFaces>
        struct CheckerImpl
        {
            static_assert(sizeof(TFace) == sizeof(typename TFace::Impl), "size of face and impl must be equal");
            static_assert(std::tuple_size<typename TFace::BaseFaces>::value == std::tuple_size<typename TFace::Impl::BaseImpls>::value, "base classes amount for face and for impl must be equal");

            static_assert(
                    std::is_same<
                        typename std::tuple_element<0, typename TFace::Impl::BaseImpls>::type,
                        typename std::tuple_element<0, TBaseFaces>::type::Impl
                    >::value,
                    "first base of impl must be same as impl of first base of face");


            template<typename T>
            struct removeFirst;

            template<typename T, typename... Ts>
            struct removeFirst<std::tuple<T, Ts...>>
            {
                typedef std::tuple<Ts...> type;
            };

            using Payload = typename CheckerImpl<
                typename std::tuple_element<0, TBaseFaces>::type,
                typename removeFirst<TBaseFaces>::type>::Payload;

        };



        template <class TFace>
        struct CheckerImpl<TFace, std::tuple<>>
        {
            static_assert(sizeof(TFace) == sizeof(typename TFace::Impl), "size of face and impl must be equal");
            static_assert(std::tuple_size<typename TFace::BaseFaces>::value == std::tuple_size<typename TFace::Impl::BaseImpls>::value, "base classes amount for face and for impl must be equal");

            struct Payload {};
        };
    }

    template <class TFace>
    struct Checker : details::CheckerImpl<TFace, typename TFace::BaseFaces>::Payload
    {
    };

}}
