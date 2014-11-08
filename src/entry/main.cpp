#include <iostream>
#include "lut/async.hpp"

int main()
{
    lut::async::spawn([]()
    {
        std::cout<<"1"<<std::endl;
        lut::async::spawn([]()
        {
            std::cout<<"2"<<std::endl;
            lut::async::stop();
        });
    });


    lut::async::run();

    return 0;
}
