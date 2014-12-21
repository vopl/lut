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

    Data::Data(std::size_t size, data::Segment *first, data::Segment *last)
        : _size {size}
        , _first {first}
        , _last {last}
    {
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
            _first = _last = new data::Segment;
        }

        data::Segment *cur = _last;
        for(;;)
        {
            assert(cur->_offset + cur->_size <= data::Segment::_granula);
            std::size_t space = data::Segment::_granula - (cur->_offset + cur->_size);

            if(space > size)
            {
                space = size;
            }

            memcpy(&cur->_buffer[cur->_offset + cur->_size], str, space);

            cur->_size += space;
            size -= space;

            if(size)
            {
                data::Segment *next = new data::Segment;
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
        data::Segment *segment = _first;
        while(segment)
        {
            data::Segment *cur = segment;
            segment = segment->_next;

            delete cur;
        }

        _first = _last = nullptr;
        _size = 0;
    }

    std::size_t Data::segmentsAmount() const
    {
        std::size_t res {0};
        for(data::Segment *iter = _first; iter; iter=iter->_next)
        {
            res++;
        }

        return res;
    }

    void Data::fillIovec(iovec *iov) const
    {
        for(data::Segment *iter = _first; iter; iter=iter->_next)
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

        data::Segment *iter = _first;

        for(;;)
        {
            data::Segment *cur = iter;

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

    Data Data::detachFirst(std::size_t size)
    {
        if(!size)
        {
            return Data {};
        }

        if(size >= _size)
        {
            return Data {std::move(*this)};
        }

        data::Segment *detachBound = _first;
        std::size_t detachSize = detachBound->_size;

        for(;;)
        {
            if(detachSize < size)
            {
                detachBound = detachBound->_next;
                detachSize += detachBound->_size;
            }
            else if(detachSize > size)
            {
                //посеридине
                data::Segment *detachFirst = _first;

                std::size_t boundTailSize = detachSize - size;

                _first = new data::Segment {static_cast<std::uint32_t>(boundTailSize), 0, detachBound->_next};
                ::memcpy(_first->_buffer, &detachBound->_buffer[detachBound->_offset + detachBound->_size - boundTailSize], boundTailSize);
                _size -= size;

                detachBound->_size -= boundTailSize;
                detachBound->_next = nullptr;
                return impl::Data {size, detachFirst, detachBound};
            }
            else
            {
                //точно по границе
                data::Segment *detachFirst = _first;
                _size -= size;
                _first = detachBound->_next;
                return impl::Data {size, detachFirst, detachBound};
            }
        }

        assert(!"never here");
    }

    Data Data::detachLast(std::size_t size)
    {
        assert(0);
    }

    std::string Data::toString()
    {
        std::string res;
        res.reserve(_size);

        data::Segment *segment = _first;
        while(segment)
        {
            res.append(&segment->_buffer[segment->_offset], &segment->_buffer[segment->_offset + segment->_size]);
            segment = segment->_next;
        }

        assert(res.size() == _size);

        return res;
    }

}}}
