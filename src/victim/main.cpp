#include <iostream>
#include "lut/stable.hpp"
#include "lut/async/functions.hpp"

#include <future>
#include "lut/mm/sharedInstance.hpp"
#include "lut/async/future.hpp"

#include "lut/io/loop.hpp"
#include "lut/io/client.hpp"
#include "lut/io/server.hpp"

#include "lut/logger/logger.hpp"

#include <system_error>

lut::async::Promise<> p;

lut::async::Future<> f(p.future());

//std::size_t ac0 {};
//std::size_t rc0 {};
//std::size_t wc0 {};

//std::size_t ac1 {};
//std::size_t rc1 {};
//std::size_t wc1 {};

int main()
{
//    lut::async::spawn([]()
//    {
//        lut::async::spawn([]()
//        {
//            LOGT("3");
//            p.setException(std::make_exception_ptr(std::runtime_error("tratata")));
//            LOGT("4");
//        });

//        LOGT("1");
//        try
//        {
//            f.value();
//        }
//        catch(std::exception &e)
//        {
//            LOGT(e.what());
//        }

//        LOGT("2");
//    });

    if(1)
    {
        lut::async::spawn([]()
        {
            //LOGT("try connect ");

            lut::io::Server server;

            auto lr = server.listen("ip://127.0.0.1:8000");
            if(lr)
            {
                LOGT("listen: "<<lr);
                return;
            }

            for(;;)
            {
//                LOGT("ac0: "<<++ac0);
                auto ar = server.accept();

                if(ar.value<0>())
                {
                    LOGT("accept: "<<ar.value<0>());
                    return;
                }
//                LOGT("ac1: "<<++ac1);

                lut::io::Stream stream = ar.detachValue<1>();

                lut::async::spawn([s = std::move(stream)] () mutable {

                    {
                        lut::io::Data data;
//                        LOGT("rc0: "<<++rc0);
                        data = s.read().detachValue<1>();
//                        LOGT("rc1: "<<++rc1);

                        if(data.toString() !=
                            "GET / HTTP/1.0\r\n"
                            "Host: 127.0.0.1:8000\r\n"
                            "User-Agent: ApacheBench/2.3\r\n"
                            "Accept: */*\r\n"
                            "\r\n")
                        {
                            LOGT("ahtung");
                        }
                    }


                    //LOGT(data.toString());

                    {
                        lut::io::Data data;

                        data <<
                                "HTTP/1.1 200 OK\r\n"
                                "Server: nginx/1.7.6\r\n"
                                "Date: Wed, 14 Jan 2015 14:08:39 GMT\r\n"
                                "Content-Type: text/html\r\n"
                                "Content-Length: 45\r\n"
                                "Last-Modified: Tue, 11 Nov 2014 10:31:06 GMT\r\n"
                                "Connection: keep-alive\r\n"
                                "Keep-Alive: timeout=20\r\n"
                                "ETag: \"5461e56a-2d\"\r\n"
                                "Accept-Ranges: bytes\r\n"
                                "\r\n";

//                        LOGT("wc0: "<<++wc0);
                        s.write(std::move(data)).wait();
//                        LOGT("wc1: "<<++wc1);
                    }
                    s.close();
                });
            }
        });


        std::cout << lut::io::loop::run() << std::endl;
    }

    if(0)
    {
        lut::async::spawn([]()
        {
            //LOGT("try connect ");

            lut::io::Client client;

            //auto errAndStream = client.connect("tcp://[2001:0db8:11a3:09d7:1f34:8a2e:07a0:765d]:1234");
            auto errAndStream = client.connect("ip://127.0.0.1:8000");
            errAndStream.wait();
            if(errAndStream.value<0>())
            {
                LOGT("connect: "<<errAndStream.value<0>());
            }
            lut::io::Stream stream = errAndStream.detachValue<1>();


            //lut::io::loop::stop();

            lut::io::Data dw;
            dw << "GET / HTTP/1.0\r\n"
                  "Host: 127.0.0.1:81\r\n"
                  "\r\n";
            auto wr = stream.write(std::move(dw));
            if(wr.value<0>())
            {
                LOGT("write: "<<wr.value<0>());
            }

            lut::async::spawn([s = std::move(stream)] () mutable {
                auto rr = s.read(1);
                if(rr.value<0>())
                {
                    LOGT("read: "<<rr.value<0>());
                }

                lut::io::Data dr = rr.detachValue<1>();

                LOGT("response: "<<dr.toString());

                s.close();
            });


            lut::io::loop::stop();

        });


        std::cout << "end: " << lut::io::loop::run() << std::endl;
    }

    return 0;
}
