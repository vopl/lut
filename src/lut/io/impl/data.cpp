#include "lut/stable.hpp"
#include "lut/io/impl/data.hpp"

namespace lut { namespace io { namespace impl
{
    Data::Data()
        : _size {0}
        , _first {nullptr}
        , _last {nullptr}
    {
    }

    Data::Data(Data &&from)
        : _size {from._size}
        , _first {from._first}
        , _last {from._last}
    {
        from._size = 0;
        from._first = nullptr;
        from._last = nullptr;
    }

    Data::~Data()
    {
        clear();
    }

    Data &Data::operator=(Data &&from)
    {
        _size = from._size;
        _first = from._first;
        _last = from._last;

        from._size = 0;
        from._first = nullptr;
        from._last = nullptr;

        return *this;
    }

    void Data::append(Data &&data)
    {
        if(!_size)
        {
            clear();

            _size = data._size;
            _first = data._first;
            _last = data._last;

            data._size = 0;
            data._first = nullptr;
            data._last = nullptr;
        }
        else
        {
            assert(_first && _last);
            assert(!_last->_next);

            if(data._size)
            {
                assert(data._first && data._last);
                assert(!data._last->_next);

                _last->_next = data._first;
                _last = data._last;
                _size += data._size;

                data._size = 0;
                data._first = nullptr;
                data._last = nullptr;
            }
            else
            {
                assert(!data._first && !data._last);
            }
        }
    }

    void Data::append(const char *str, std::size_t size)
    {
        if(!size)
        {
            return;
        }

        _size += size;

        if(!_last)
        {
            assert(!_first);
            _first = _last = new Segment;
            _last->_next = nullptr;
            _last->_offset = 0;
            _last->_size = 0;
        }

        Segment *cur = _last;
        for(;;)
        {
            assert(cur->_offset + cur->_size <= _granula);
            std::size_t space = _granula - (cur->_offset + cur->_size);

            if(space > size)
            {
                space = size;
            }

            memcpy(&cur->_buffer[cur->_offset + cur->_size], str, space);

            cur->_size += space;
            size -= space;

            if(size)
            {
                Segment *next = new Segment;
                next->_next = nullptr;
                next->_offset = 0;
                next->_size = 0;
                cur->_next = next;
                cur = next;
            }
            else
            {
                break;
            }
        }

        if(_last != cur)
        {
            _last = cur;
        }
    }

    bool Data::empty() const
    {
        return _size ? false : true;
    }

    std::size_t Data::size() const
    {
        return _size;
    }

    void Data::clear()
    {
        Segment *segment = _first;
        while(segment)
        {
            Segment *cur = segment;
            segment = segment->_next;

            delete cur;
        }

        _first = _last = nullptr;
        _size = 0;
    }

    std::size_t Data::segmentsAmount() const
    {
        std::size_t res {0};
        for(Segment *iter = _first; iter; iter=iter->_next)
        {
            res++;
        }

        return res;
    }

    void Data::fillIovec(iovec *iov) const
    {
        for(Segment *iter = _first; iter; iter=iter->_next)
        {
            iov->iov_base = &iter->_buffer[iter->_offset];
            iov->iov_len = iter->_size;
            iov++;
        }
    }

    void Data::dropFirst(std::size_t size)
    {
        if(size >= _size)
        {
            clear();
            return;
        }

        Segment *iter = _first;

        for(;;)
        {
            Segment *cur = iter;

            if(size > cur->_size)
            {
                iter = iter->_next;
                size -= cur->_size;
                delete cur;
            }
            else if(size < cur->_size)
            {
                cur->_offset += size;
                cur->_size -= size;
                size = 0;
                break;
            }
            else
            {
                iter = iter->_next;
                delete cur;
                break;
            }
        }

        _first = iter;
    }

    void Data::dropLast(std::size_t size)
    {
        assert(0);
    }


}}}
