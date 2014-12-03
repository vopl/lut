#pragma once

#include "lut/mm/sharedInstance.hpp"
#include "lut/async/event.hpp"

#include <type_traits>
#include <tuple>
#include <utility>
#include <exception>

namespace lut { namespace async
{
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
    }

    template <typename... T> class Promise;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename... T>
    class Future
    {
        friend class Promise<T...>;
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

    private:
        StateInstance _state;
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename... T>
    class Promise
    {
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

    private:
        using StateInstance = typename Future<T...>::StateInstance;
        StateInstance _state;
    };



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
        : _state(state)
    {
    }

    template <typename... T>
    Future<T...>::Future(const Future &other)
        : _state(other._state)
    {
    }

    template <typename... T>
    Future<T...>::Future(Future &&other)
        : _state(std::move(other._state))
    {

    }

    template <typename... T>
    Future<T...> &Future<T...>::operator=(const Future &other)
    {
        _state = other._state;
    }

    template <typename... T>
    Future<T...> &Future<T...>::operator=(Future &&other)
    {
        _state = std::move<StateInstance>(other._state);
    }

    template <typename... T>
    void Future<T...>::wait()
    {
        _state->_readyEvent.acquire();
    }

    template <typename... T>
    bool Future<T...>::isReady() const
    {
        return _state->_readyEvent.isSignalled();
    }

    template <typename... T>
    const std::tuple<T...> &Future<T...>::value()
    {
        wait();
        if(_state->_exception)
        {
            std::rethrow_exception(_state->_exception);
        }
        return _state->valueArea();
    }

    template <typename... T>
    template <std::size_t idx>
    const typename std::tuple_element<idx, std::tuple<T...> >::type &Future<T...>::value()
    {
        wait();
        if(_state->_exception)
        {
            std::rethrow_exception(_state->_exception);
        }
        return std::get<idx>(_state->valueArea());
    }

    template <typename... T>
    std::tuple<T...> &&Future<T...>::detachValue()
    {
        wait();
        if(_state->_exception)
        {
            std::rethrow_exception(_state->_exception);
        }
        return _state->valueArea();
    }


    template <typename... T>
    template <std::size_t idx>
    typename std::tuple_element<idx, std::tuple<T...> >::type &&Future<T...>::detachValue()
    {
        wait();
        if(_state->_exception)
        {
            std::rethrow_exception(_state->_exception);
        }
        return std::get<idx>(std::forward<typename details::FutureState<T...>::Value>(_state->valueArea()));
    }



    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <typename... T>
    Promise<T...>::Promise()
        : _state()
    {
    }

    template <typename... T>
    Promise<T...>::Promise(Promise &&other)
        : _state(std::move<StateInstance>(other._state))
    {
    }

    template <typename... T>
    Promise<T...> &Promise<T...>::operator=(Promise &&other)
    {
        _state = std::move<StateInstance>(other._state);
    }

    template <typename... T>
    Promise<T...>::~Promise()
    {
        if(!_state->_readyEvent.isSignalled() && _state.counter()>1)
        {
            assert(!"unsetted promise destroyed while futures exists");
            std::abort();
        }
    }

    template <typename... T>
    Future<T...> Promise<T...>::future()
    {
        return Future<T...>(_state);
    }

    template <typename... T>
    bool Promise<T...>::isReady() const
    {
        return _state->_readyEvent.isSignalled();
    }

    template <typename... T>
    void Promise<T...>::setValue(T&&... val)
    {
        if(_state->_readyEvent.isSignalled())
        {
            assert(!"promise already has value or exception");
            std::abort();
        }

        new(&_state->valueArea()) std::tuple<T...>(std::forward<T>(val)...);
        _state->_readyEvent.set();
    }

    template <typename... T>
    void Promise<T...>::setException(const std::exception_ptr &exception)
    {
        if(_state->_readyEvent.isSignalled())
        {
            assert(!"promise already has value or exception");
            std::abort();
        }

        _state->_exception = exception;
        _state->_readyEvent.set();
    }

}}
