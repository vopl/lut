#pragma once

#include "lut/io/handle.hpp"
#include <system_error>

namespace lut { namespace io
{
    namespace impl
    {
        class Stream;
    }

    //incomplete
    class Data {};

    class Stream
        : public Handle
        , private HiddenImpl<impl::Stream>
    {
        Stream(const Stream &from) = delete;
        Stream &operator=(const Stream &from) = delete;

    public:
        Stream();
        ~Stream();

    public:

        //int uv_shutdown(uv_shutdown_t* req, uv_stream_t* handle, uv_shutdown_cb cb);
        async::Future<std::error_code> shutdown();

        //int uv_listen(uv_stream_t* stream, int backlog, uv_connection_cb cb);
        //int uv_accept(uv_stream_t* server, uv_stream_t* client);
        async::Future<std::error_code> listen(int backlog);
        std::error_code accept(Stream &client);


        //int uv_read_start(uv_stream_t*, uv_alloc_cb alloc_cb, uv_read_cb read_cb);
        //int uv_read_stop(uv_stream_t*);
        async::Future<std::error_code, Data> read(int min, int max);

        //int uv_write(uv_write_t* req, uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbufs, uv_write_cb cb);
        //int uv_write2(uv_write_t* req, uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbufs, uv_stream_t* send_handle, uv_write_cb cb);
        //int uv_try_write(uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbufs);
        std::error_code write(const Data &data);


        //int uv_is_readable(const uv_stream_t* handle);
        bool isReadable();

        //int uv_is_writable(const uv_stream_t* handle);
        bool isWritable();
    };
}}
