#include <iostream>
#include "lut/stable.hpp"
#include "lut/async/functions.hpp"

#include <future>
#include "lut/mm/sharedInstance.hpp"
#include "lut/async/future.hpp"

#include "lut/io/loop.hpp"
#include "lut/io/client.hpp"
#include "lut/io/server.hpp"

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
//            std::cout<<"3"<<std::endl;
//            p.setException(std::make_exception_ptr(std::runtime_error("tratata")));
//            std::cout<<"4"<<std::endl;
//        });

//        std::cout<<"1"<<std::endl;
//        try
//        {
//            f.value();
//        }
//        catch(std::exception &e)
//        {
//            std::cout<<e.what()<<std::endl;
//        }

//        std::cout<<"2"<<std::endl;
//    });

    if(1)
    {
        lut::async::spawn([]()
        {
            //std::cout<<"try connect "<<std::endl;

            lut::io::Server server;

            auto lr = server.listen("ip://127.0.0.1:8000");
            if(lr)
            {
                std::cout<<"listen: "<<lr.message()<<std::endl;
                return;
            }

            for(;;)
            {
//                std::cout<<"ac0: "<<++ac0<<std::endl;
                auto ar = server.accept();

                if(ar.value<0>())
                {
                    std::cout<<"accept: "<<ar.value<0>().message()<<std::endl;
                    return;
                }
//                std::cout<<"ac1: "<<++ac1<<std::endl;

                lut::io::Stream stream = ar.detachValue<1>();

                lut::async::spawn([s = std::move(stream)] () mutable {

                    {
                        lut::io::Data data;
//                        std::cout<<"rc0: "<<++rc0<<std::endl;
                        data = s.read().detachValue<1>();
//                        std::cout<<"rc1: "<<++rc1<<std::endl;

                        if(data.toString() !=
                            "GET / HTTP/1.0\r\n"
                            "Host: 127.0.0.1:8000\r\n"
                            "User-Agent: ApacheBench/2.3\r\n"
                            "Accept: */*\r\n"
                            "\r\n")
                        {
                            std::cout<<"ahtung"<<std::endl;
                        }
                    }


                    //std::cout<<data.toString()<<std::endl;

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

//                        std::cout<<"wc0: "<<++wc0<<std::endl;
                        s.write(std::move(data)).wait();
//                        std::cout<<"wc1: "<<++wc1<<std::endl;
                    }
                    s.close();
                });
            }
        });


        std::cout << lut::io::loop::run().message() << std::endl;
    }

    if(0)
    {
        lut::async::spawn([]()
        {
            //std::cout<<"try connect "<<std::endl;

            lut::io::Client client;

            //auto errAndStream = client.connect("tcp://[2001:0db8:11a3:09d7:1f34:8a2e:07a0:765d]:1234");
            auto errAndStream = client.connect("ip://127.0.0.1:8000");
            errAndStream.wait();
            if(errAndStream.value<0>())
            {
                std::cout<<"connect: "<<errAndStream.value<0>().message()<<std::endl;
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
                std::cout<<"write: "<<wr.value<0>().message()<<std::endl;
            }

            lut::async::spawn([s = std::move(stream)] () mutable {
                auto rr = s.read(1);
                if(rr.value<0>())
                {
                    std::cout<<"read: "<<rr.value<0>().message()<<std::endl;
                }

                lut::io::Data dr = rr.detachValue<1>();

                std::cout << "response: "<<dr.toString() << std::endl;

                s.close();
            });


            lut::io::loop::stop();

        });


        std::cout << "end: " << lut::io::loop::run().message() << std::endl;
    }

    return 0;
}
