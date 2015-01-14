#include "lut/stable.hpp"
#include "lut/io/impl/fd/stream/writer.hpp"
#include "lut/io/error.hpp"

#include <sys/uio.h>

namespace lut { namespace io { namespace impl { namespace fd { namespace stream
{

    Writer::Request::Request(std::size_t tailSize)
        : _tailSize {tailSize}
        , _promise {}
        , _next {nullptr}
    {

    }


    Writer::Writer()
        : _descriptorReady {false}
        , _requestsFirst {nullptr}
        , _requestsLast {nullptr}
    {
    }

    Writer::~Writer()
    {
        close();
    }

    void Writer::error(const std::error_code &err)
    {
        assert(0);
    }

    void Writer::ready(int descriptor)
    {
        _descriptorReady = true;
        pump(descriptor);
    }

    async::Future<std::error_code> Writer::write(int descriptor, io::Data &&data)
    {
        assert(!data.empty());
        if(data.empty())
        {
            return async::mkReadyFuture(std::error_code());
        }

        if(!_requestsFirst)
        {
            assert(!_requestsLast);

            Request *request = new Request {data.size()};

            assert(_buffer.empty());
            _buffer = std::forward<io::Data>(data);

            _requestsFirst = _requestsLast = request;
            async::Future<std::error_code> res = request->_promise.future();

            pump(descriptor);

            return res;
        }

        assert(!_descriptorReady);
        assert(_requestsLast);

        Request *request = new Request {data.size()};

        assert(!_buffer.empty());
        _buffer.append(std::forward<io::Data>(data));

        _requestsLast->_next = request;
        _requestsLast = request;

        return request->_promise.future();
    }

    void Writer::close()
    {
        _buffer.clear();
        flushError(make_error_code(error::stream::closed));
    }

    void Writer::pump(int descriptor)
    {
        if(!_descriptorReady)
        {
            return;
        }

        if(_buffer.empty())
        {
            return;
        }

        int iovAmount = _buffer.segmentsAmount();

        if(iovAmount)
        {
            iovec iov[iovAmount];
            _buffer.fillIovec(&iov[0]);

            ssize_t res = ::writev(descriptor, iov, iovAmount);

            if(0 <= res)
            {
                flushProcessed(res);
            }
            else
            {
                flushError(std::error_code(errno, std::system_category()));
            }
        }
    }

    void Writer::flushProcessed(std::size_t size)
    {
        assert(size);

        if(size < _buffer.size())
        {
            _descriptorReady = false;
        }

        _buffer.dropFirst(size);

        for(;;)
        {
            assert(_requestsFirst);

            if(_requestsFirst->_tailSize <= size)
            {
                Request *r = _requestsFirst;

                if(_requestsFirst->_next)
                {
                    _requestsFirst = _requestsFirst->_next;
                }
                else
                {
                    assert(_requestsLast == _requestsFirst);
                    _requestsFirst = _requestsLast = nullptr;
                }

                r->_promise.setValue(std::error_code());
                size -= r->_tailSize;

                delete r;

                if(!size)
                {
                    break;
                }

            }
            else
            {
                _requestsFirst->_tailSize -= size;
                size = 0;
                break;
            }
        }
    }

    void Writer::flushError(const std::error_code &ec)
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
                r->_promise.setValue(std::error_code(ec));
                delete r;
            }
        }
    }

}}}}}
