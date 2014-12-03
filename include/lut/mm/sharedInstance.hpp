#pragma once

#include <type_traits>
#include <cstdint>
#include <cassert>
#include <utility>
#include "lut/mm.hpp"

namespace lut { namespace mm
{

    template <typename T>
    class SharedInstance
    {
    public:
        using Instance = typename std::decay<T>::type;

    public:
        template <typename... Args>
        SharedInstance(Args &&... args);

        SharedInstance(const SharedInstance &other);
        SharedInstance(SharedInstance &&other);

        ~SharedInstance();

        SharedInstance &operator=(const SharedInstance &other);
        SharedInstance &operator=(SharedInstance &&other);

        std::int32_t counter() const;

        Instance &value();
        Instance *ptr();
        Instance *operator->();

        const Instance &value() const;
        const Instance *ptr() const;
        const Instance *operator->() const;

    private:
        void ref();
        void unref();

    private:
        struct State
        {
            Instance        _instance;
            std::int32_t    _counter;
        };

        State *_state;
    };



    template <typename T>
    template <typename... Args>
    SharedInstance<T>::SharedInstance(Args &&... args)
        : _state(static_cast<State *>(mm::alloc<sizeof(State)>()))
    {
        new(&_state->_instance) Instance(std::forward<Args...>(args)...);
        _state->_counter = 1;
    }

    template <typename T>
    SharedInstance<T>::SharedInstance(const SharedInstance &other)
        : _state(other._state)
    {
        ref();
    }

    template <typename T>
    SharedInstance<T>::SharedInstance(SharedInstance &&other)
        : _state(other._state)
    {
        if(other._state)
        {
            other._state = nullptr;
        }
    }

    template <typename T>
    SharedInstance<T>::~SharedInstance()
    {
        unref();
    }

    template <typename T>
    SharedInstance<T> &SharedInstance<T>::operator=(const SharedInstance &other)
    {
        unref();
        _state = other._state;
        ref();
    }

    template <typename T>
    SharedInstance<T> &SharedInstance<T>::operator=(SharedInstance &&other)
    {
        unref();
        _state = other._state;
        if(other._state)
        {
            other._state = nullptr;
        }
    }

    template <typename T>
    std::int32_t SharedInstance<T>::counter() const
    {
        if(_state)
        {
            return _state->_counter;
        }

        return 0;
    }

    template <typename T>
    typename SharedInstance<T>::Instance &SharedInstance<T>::value()
    {
        assert(_state);
        return _state->_instance;
    }

    template <typename T>
    typename SharedInstance<T>::Instance *SharedInstance<T>::ptr()
    {
        assert(_state);
        return &_state->_instance;
    }

    template <typename T>
    typename SharedInstance<T>::Instance *SharedInstance<T>::operator->()
    {
        return ptr();
    }

    template <typename T>
    const typename SharedInstance<T>::Instance &SharedInstance<T>::value() const
    {
        assert(_state);
        return _state->_instance;
    }

    template <typename T>
    const typename SharedInstance<T>::Instance *SharedInstance<T>::ptr() const
    {
        assert(_state);
        return &_state->_instance;
    }

    template <typename T>
    const typename SharedInstance<T>::Instance *SharedInstance<T>::operator->() const
    {
        return ptr();
    }

    template <typename T>
    void SharedInstance<T>::ref()
    {
        if(_state)
        {
            assert(_state->_counter);
            _state->_counter++;
        }
    }

    template <typename T>
    void SharedInstance<T>::unref()
    {
        if(_state)
        {
            if(! --_state->_counter)
            {
                _state->_instance.~Instance();
                mm::free<sizeof(State)>(_state);
                _state = nullptr;
            }
        }
    }

}}
