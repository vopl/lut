#include "reader.hpp"
#include "../../data.hpp"
#include <error.hpp>

#include <utility>

namespace dci { namespace io { namespace impl { namespace fd { namespace stream
{

    Reader::Request::Request(std::size_t needMinSize, std::size_t needMaxSize)
        : _needMinSize {needMinSize}
        , _needMaxSize {needMaxSize}
        , _promise {}
        , _next {nullptr}
    {

    }

    Reader::Reader()
        : _descriptorReady {false}
        , _buffer {}
        , _requestsFirst {nullptr}
        , _requestsLast {nullptr}
    {
    }

    Reader::~Reader()
    {
        _buffer.clear();
        flushError(make_error_code(error::stream::closed));
    }

    void Reader::error(const std::error_code &err)
    {
        assert(0);
    }

    void Reader::ready(int descriptor)
    {
        _descriptorReady = true;
        pump(descriptor);
    }

    async::Future<std::error_code, io::Data> Reader::read(int descriptor, std::size_t min, std::size_t max)
    {
        if(_buffer.size() >= min)
        {
            return async::mkReadyFuture(std::error_code(), _buffer.detachFirst(std::min(_buffer.size(), max)));
        }

        if(!_requestsFirst)
        {
            assert(!_requestsLast);

            Request *request = new Request {min, max};
            _requestsFirst = _requestsLast = request;

            return request->_promise.future();
        }

        assert(!_descriptorReady);
        assert(_requestsLast);

        Request *request = new Request {min, max};
        _requestsLast->_next = request;
        _requestsLast = request;

        return request->_promise.future();
    }

    void Reader::close()
    {
        _buffer.clear();
        flushError(make_error_code(error::stream::closed));
    }

    void Reader::flushProcessed(impl::Data &&data)
    {
        _buffer.append(std::forward<impl::Data>(data));

        for(;;)
        {
            if(!_requestsFirst)
            {
                return;
            }

            if(_buffer.size() < _requestsFirst->_needMinSize)
            {
                return;
            }

            std::size_t size = std::min(_buffer.size(), _requestsFirst->_needMaxSize);
            Request *r = _requestsFirst;
            _requestsFirst = _requestsFirst->_next;
            if(!_requestsFirst)
            {
                _requestsLast = nullptr;
            }

            r->_promise.setValue(std::error_code{}, _buffer.detachFirst(size));
            delete r;
        }
    }

    void Reader::flushError(const std::error_code &ec)
    {
        assert(ec);

        if(std::error_condition(EAGAIN, std::system_category()) == ec)
        {
            assert(!"impossible");
            _descriptorReady = false;
        }
        else
        {
            _descriptorReady = false;
            _buffer.clear();

            Request *r = _requestsFirst;
            _requestsFirst = _requestsLast = nullptr;
            for(; r; r = r->_next)
            {
                r->_promise.setValue(std::error_code(ec), io::Data {});
                delete r;
            }
        }
    }

    void Reader::pump(int descriptor)
    {
        if(!_descriptorReady)
        {
            return;
        }

        std::pair<iovec *, std::size_t> buf = _preparedBuffer.prepare(1024*64);
        ssize_t res = ::readv(descriptor, buf.first, buf.second);

        if(0 <= res)
        {
            flushProcessed(std::move(_preparedBuffer.flush(res)));
        }
        else
        {
            flushError(std::error_code(errno, std::system_category()));
        }
    }

    Reader::PreparedBuffer::PreparedBuffer()
        : _segmentFirst {new data::Segment(data::Segment::_granula)}
        , _segmentLast {_segmentFirst}
        , _iovecs {1, iovec {_segmentLast->_buffer, data::Segment::_granula}}
    {
    }

    Reader::PreparedBuffer::~PreparedBuffer()
    {
        while(_segmentFirst)
        {
            impl::data::Segment *segment = _segmentFirst;
            _segmentFirst = _segmentFirst->_next;
            delete segment;
        }
    }

    std::pair<iovec *, std::size_t> Reader::PreparedBuffer::prepare(std::size_t size)
    {
        size /= data::Segment::_granula;

        //один или два лишних всегда
        while(size >= _iovecs.size())
        {
            _segmentLast->_next = new data::Segment(data::Segment::_granula);
            _segmentLast = _segmentLast->_next;
            _iovecs.emplace_back(iovec {_segmentLast->_buffer, data::Segment::_granula});
        }

        return std::make_pair(&_iovecs[0], _iovecs.size());
    }

    impl::Data Reader::PreparedBuffer::flush(std::size_t size)
    {
        if(!size)
        {
            return impl::Data{};
        }

        data::Segment *first = _segmentFirst;
        data::Segment *last = first;

        std::size_t segmentsAmount = (size % data::Segment::_granula) ?
                                              size/data::Segment::_granula+1:
                                              size/data::Segment::_granula;

        for(std::size_t cnt{1}; cnt<segmentsAmount; cnt++)
        {
            last = last->_next;
        }
        _segmentFirst = last->_next;

        last->_next = nullptr;
        last->_size = size - (segmentsAmount-1) * data::Segment::_granula;
        _iovecs.erase(_iovecs.begin(), _iovecs.begin()+segmentsAmount);

        return impl::Data {size, first, last};
    }

    Reader::PreparedBuffer Reader::_preparedBuffer = {};

}}}}}
