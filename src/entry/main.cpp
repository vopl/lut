#include <iostream>
#include "lut/async.hpp"
#include "lut/io/tcp.hpp"
#include <netinet/in.h>

int main()
{
    lut::async::spawn([]()
    {
        lut::io::Tcp tcp;
        std::cout<<"try connect "<<std::endl;

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = 330;
        addr.sin_addr.s_addr = INADDR_LOOPBACK;
        lut::async::Future<std::error_code> cr = tcp.connect((sockaddr *)&addr);

        cr.wait();

        lut::io::Data d;
        tcp.write(d);
        tcp.read(0, 220).wait();

        tcp.close().wait();

    });


    lut::async::run();

    return 0;
}
