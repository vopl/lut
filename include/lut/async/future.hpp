#pragma once


namespace lut { namespace async
{

    template <typename... T>
    class Future
    {
        Future(const Future &other) = delete;
        Future &operator=(const Future &other) = delete;

        //friend class promice
        Future();

    public:
        Future(Future &&other);
        Future &operator=(Future &&other);

        void wait();

    };

    template <typename... T>
    class Promise
    {

    };

}}
