#include "lut/async/threadState.hpp"

namespace lut { namespace async
{
    ThreadState::ThreadState()
        : _value(ThreadStateValue::init)
    {
    }

    ThreadState::~ThreadState()
    {
    }

    ThreadStateValue ThreadState::get()
    {
        std::lock_guard<std::mutex> l(_mtx);
        return _value;
    }

    void ThreadState::wait(ThreadStateValue v)
    {
        std::unique_lock<std::mutex> l(_mtx);
        while(_value != v)
        {
            _cv.wait(l);
        }
    }

    void ThreadState::waitNot(ThreadStateValue v)
    {
        std::unique_lock<std::mutex> l(_mtx);
        while(_value == v)
        {
            _cv.wait(l);
        }
    }

    void ThreadState::set(ThreadStateValue v)
    {
        std::lock_guard<std::mutex> l(_mtx);
        if(_value != v)
        {
            _value = v;
            _cv.notify_all();
        }
    }
}}
