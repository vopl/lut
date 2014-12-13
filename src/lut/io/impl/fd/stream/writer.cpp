#include "lut/stable.hpp"
#include "lut/io/impl/fd/stream/writer.hpp"

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
            ::abort();
        }

        if(!_requestsFirst)
        {
            assert(!_requestsLast);

            Request *request = new Request {data.size()};

            assert(_tailData.empty());
            _tailData = std::forward<io::Data>(data);

            _requestsFirst = _requestsLast = request;

            pump(descriptor);

            return request->_promise.future();
        }

        assert(!_descriptorReady);
        assert(_requestsLast);

        Request *request = new Request {data.size()};

        assert(!_tailData.empty());
        _tailData.append(std::forward<io::Data>(data));

        _requestsLast->_next = request;
        _requestsLast = request;

        return request->_promise.future();
    }

    void Writer::close()
    {
        _tailData.clear();

        Request *request = _requestsFirst;
        _requestsFirst = _requestsLast = nullptr;

        while(request)
        {
            request->_promise.setValue(std::make_error_code(std::errc::connection_aborted));
            request = request->_next;
        }

        _descriptorReady = false;
    }

    void Writer::pump(int descriptor)
    {
        if(!_descriptorReady)
        {
            return;
        }

        if(_tailData.empty())
        {
            return;
        }

        int iovAmount = _tailData.segmentsAmount();

        std::size_t processedBytes {0};
        std::error_code error {};

        if(iovAmount)
        {
            iovec iov[iovAmount];
            _tailData.fillIovec(&iov[0]);

            ssize_t res = ::writev(descriptor, iov, iovAmount);

            if(0 <= res)
            {
                processedBytes = res;
            }
            else
            {
                error = std::error_code(errno, std::system_category());
            }
        }

        if(processedBytes)
        {
            _tailData.dropFirst(processedBytes);

            if(processedBytes < _tailData.size())
            {
                _descriptorReady = false;
            }

            for(;;)
            {
                assert(_requestsFirst);

                if(_requestsFirst->_tailSize <= processedBytes)
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
                    processedBytes -= r->_tailSize;

                    delete r;

                }
                else
                {
                    _requestsFirst->_tailSize -= processedBytes;
                    processedBytes = 0;
                    break;
                }
            }
        }


        if(error)
        {

            if(std::error_condition(EAGAIN, std::system_category()) == error)
            {
                assert(!"impossible");
                _descriptorReady = false;
            }
            else
            {
                _descriptorReady = false;
                _tailData.clear();

                Request *r = _requestsFirst;
                _requestsFirst = _requestsLast = nullptr;
                for(; r; r = r->_next)
                {
                    r->_promise.setValue(std::error_code(err, std::system_category()));
                }
            }
        }
    }

}}}}}
