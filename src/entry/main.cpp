#include <iostream>
#include "lut/async.hpp"

#include <future>
#include "lut/mm/sharedInstance.hpp"
#include "lut/async/future.hpp"

#include "lut/io/loop.hpp"
#include "lut/io/client.hpp"

#include <system_error>

lut::async::Promise<> p;

lut::async::Future<> f(p.future());

int main()
{

    lut::async::spawn([]()
    {
        lut::async::spawn([]()
        {
            std::cout<<"3"<<std::endl;
            p.setException(std::make_exception_ptr(std::runtime_error("tratata")));
            std::cout<<"4"<<std::endl;
        });

        std::cout<<"1"<<std::endl;
        try
        {
            f.value();
        }
        catch(std::exception &e)
        {
            std::cout<<e.what()<<std::endl;
        }

        std::cout<<"2"<<std::endl;
    });

    {
        lut::async::spawn([]()
        {
            std::cout<<"try connect "<<std::endl;

            lut::io::Client client;

            //auto errAndStream = client.connect("tcp://[2001:0db8:11a3:09d7:1f34:8a2e:07a0:765d]:1234");
            auto errAndStream = client.connect("ip://127.0.0.1:80");
            errAndStream.wait();
            lut::io::Stream stream = errAndStream.detachValue<1>();

            int k = 110;

            //lut::io::loop::stop();

            lut::io::Data dw;
            dw << "HTTP 1.0 GET \r\n\r\n";
            stream.write(std::move(dw)).wait();

            lut::io::Data dr = stream.read(0, 220).detachValue<1>();

            //stream.close();

        });


        std::cout << lut::io::loop::run().message() << std::endl;
    }

    return 0;
}
