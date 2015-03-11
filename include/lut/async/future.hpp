#pragma once

#include "lut/mm/sharedInstance.hpp"
#include "lut/async/event.hpp"

#include <type_traits>
#include <tuple>
#include <utility>
#include <exception>

namespace lut { namespace async
{
    template <typename... T>
    class Future;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    namespace details
    {
        template <typename... T>
        struct FutureState
        {
            using Value = std::tuple<T...>;

            FutureState();
            ~FutureState();
            Value &valueArea();

            Event       _readyEvent;
            typename std::aligned_storage<sizeof(Value), alignof(Value)>::type   _valueArea;
            std::exception_ptr _exception;
        };

        struct FutureStateAccessos
        {
            template <typename... T>
            static FutureState<T...> &exec(Future<T...> &src)
            {
                return src.instance();
            }
        };
    }

    template <typename... T> class Promise;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename... T>
    class Future
        : private lut::mm::SharedInstance<details::FutureState<T...>>
    {
        friend class Promise<T...>;
        friend class details::FutureStateAccessos;
        using StateInstance = lut::mm::SharedInstance<details::FutureState<T...>>;
        Future(const StateInstance &state);

    public:
        Future(const Future &other);
        Future(Future &&other);

        Future &operator=(const Future &other);
        Future &operator=(Future &&other);

        void wait();
        bool isReady() const;

        const std::tuple<T...> &value();

        template <std::size_t idx>
        const typename std::tuple_element<idx, std::tuple<T...> >::type &value();

        std::tuple<T...> &&detachValue();

        template <std::size_t idx>
        typename std::tuple_element<idx, std::tuple<T...> >::type &&detachValue();
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename... T>
    class Promise
        : private lut::mm::SharedInstance<details::FutureState<T...>>
    {
        using StateInstance = lut::mm::SharedInstance<details::FutureState<T...>>;

        Promise(const Promise &other) = delete;
        Promise &operator=(const Promise &other) = delete;

    public:
        Promise();
        Promise(Promise &&other);
        Promise &operator=(Promise &&other);

        ~Promise();

        Future<T...> future();

        bool isReady() const;
        void setValue(T&&... val);
        void setException(const std::exception_ptr &exception);
    };

    template <typename... T>
    Future<T...> mkReadyFuture(T&&... val)
    {
        Promise<T...> promise;
        promise.setValue(std::forward<T>(val)...);
        return promise.future();
    }

    namespace details
    {
        template <typename... T>
        FutureState<T...>::FutureState()
            : _readyEvent(false)
        {
        }

        template <typename... T>
        FutureState<T...>::~FutureState()
        {
            if(_readyEvent.isSignalled() && !_exception)
            {
                valueArea().~Value();
            }
        }

        template <typename... T>
        typename FutureState<T...>::Value &FutureState<T...>::valueArea()
        {
            return *reinterpret_cast<Value *>(&_valueArea);
        }
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename... T>
    Future<T...>::Future(const StateInstance &state)
        : StateInstance(state)
    {
    }

    template <typename... T>
    Future<T...>::Future(const Future &other)
        : StateInstance(static_cast<const StateInstance &>(other))
    {
    }

    template <typename... T>
    Future<T...>::Future(Future &&other)
        : StateInstance(std::forward<StateInstance>(other))
    {

    }

    template <typename... T>
    Future<T...> &Future<T...>::operator=(const Future &other)
    {
        this->StateInstance::operator=(other);
        return *this;
    }

    template <typename... T>
    Future<T...> &Future<T...>::operator=(Future &&other)
    {
        this->StateInstance::operator=(std::forward<StateInstance>(other));
        return *this;
    }

    template <typename... T>
    void Future<T...>::wait()
    {
        this->instance()._readyEvent.acquire();
    }

    template <typename... T>
    bool Future<T...>::isReady() const
    {
        return this->_readyEvent.isSignalled();
    }

    template <typename... T>
    const std::tuple<T...> &Future<T...>::value()
    {
        wait();
        if(this->instance()._exception)
        {
            std::rethrow_exception(this->instance()._exception);
        }
        return this->instance().valueArea();
    }

    template <typename... T>
    template <std::size_t idx>
    const typename std::tuple_element<idx, std::tuple<T...> >::type &Future<T...>::value()
    {
        wait();
        if(this->instance()._exception)
        {
            std::rethrow_exception(this->instance()._exception);
        }
        return std::get<idx>(this->instance().valueArea());
    }

    template <typename... T>
    std::tuple<T...> &&Future<T...>::detachValue()
    {
        wait();
        if(this->_exception)
        {
            std::rethrow_exception(this->_exception);
        }
        return this->valueArea();
    }


    template <typename... T>
    template <std::size_t idx>
    typename std::tuple_element<idx, std::tuple<T...> >::type &&Future<T...>::detachValue()
    {
        wait();
        if(this->instance()._exception)
        {
            std::rethrow_exception(this->instance()._exception);
        }
        return std::get<idx>(std::forward<typename details::FutureState<T...>::Value>(this->instance().valueArea()));
    }



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename... T>
    Promise<T...>::Promise()
        : StateInstance()
    {
    }

    template <typename... T>
    Promise<T...>::Promise(Promise &&other)
        : StateInstance(std::forward<StateInstance>(other))
    {
    }

    template <typename... T>
    Promise<T...> &Promise<T...>::operator=(Promise &&other)
    {
        this->StateInstance::operator=( std::forward<StateInstance>(other));
    }

    template <typename... T>
    Promise<T...>::~Promise()
    {
        if(this->counter()>1 && !this->instance()._readyEvent.isSignalled())
        {
            assert(!"unsetted promise destroyed while futures exists");
            std::abort();
        }
    }

    template <typename... T>
    Future<T...> Promise<T...>::future()
    {
        return Future<T...>(*this);
    }

    template <typename... T>
    bool Promise<T...>::isReady() const
    {
        return this->instance()._readyEvent.isSignalled();
    }

    template <typename... T>
    void Promise<T...>::setValue(T&&... val)
    {
        if(this->instance()._readyEvent.isSignalled())
        {
            assert(!"promise already has value or exception");
            std::abort();
        }

        new(&this->instance().valueArea()) std::tuple<T...>(std::forward<T>(val)...);
        this->instance()._readyEvent.set();
    }

    template <typename... T>
    void Promise<T...>::setException(const std::exception_ptr &exception)
    {
        if(this->instance()._readyEvent.isSignalled())
        {
            assert(!"promise already has value or exception");
            std::abort();
        }

        this->instance()._exception = exception;
        this->instance()._readyEvent.set();
    }

}}
