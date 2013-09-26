#include "lut/async/stable.hpp"
#include "lut/async/impl/task.hpp"

namespace lut { namespace async { namespace impl
{
    ////////////////////////////////////////////////////////////////////////////////
    Task::Task()
        : _size()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    Task::~Task()
    {
        clear();
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Task::moveTo(Task &&task)
    {
        assert(!empty());
        assert(task.empty());
        task._size = _size;
        callHolderBase()->moveTo(task.callHolderBase());
        _size = 0;
    }

    ////////////////////////////////////////////////////////////////////////////////
    bool Task::empty() const
    {
        return _size ? false : true;
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Task::clear()
    {
        if(!empty())
        {
            if(isInternalPlaced())
            {
                callHolderBase()->~CallHolderBase();
            }
            else
            {
                delete callHolderBase();
            }
            _size = 0;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    void Task::exec()
    {
        assert(!empty());
        callHolderBase()->call();
    }

    ////////////////////////////////////////////////////////////////////////////////
    bool Task::isInternalPlaced()
    {
        return _size <= sizeof(_data._buffer4CallHolder);
    }

    ////////////////////////////////////////////////////////////////////////////////
    Task::CallHolderBase *Task::callHolderBase()
    {
        assert(!empty());
        if(isInternalPlaced())
        {
            return (CallHolderBase *)&_data._buffer4CallHolder;
        }
        else
        {
            return _data._callHolderPointer;
        }
    }

}}}
