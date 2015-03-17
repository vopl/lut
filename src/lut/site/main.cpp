#include <cstdlib>

#include "lut/site/impl/instance.hpp"
#include "lut/async/functions.hpp"
#include "lut/logger/logger.hpp"

#include <csignal>
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

lut::site::impl::Instance *instance = nullptr;

void signalHandler(int signum)
{
    switch(signum)
    {
    case SIGINT:
    case SIGTERM:
        if(instance)
        {
            lut::async::spawn([](){
                std::error_code ec = instance->stop().value<0>();
                if(ec)
                {
                    LOGE("stop: "<<ec);
                }
            });
        }
        break;

    default:
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char *argv[])
{
    //set current path as bin/..
    {
        boost::system::error_code ec;
        fs:: path p{argv[0]};
        p = fs::system_complete(p, ec);
        if(ec)
        {
            LOGE("unable to determine current directory: "<<ec);
            return EXIT_FAILURE;
        }

        p = p.normalize().parent_path().parent_path();
        fs::current_path(p, ec);
        if(ec)
        {
            LOGE("unable to set current directory to "<<p<<": "<<ec);
            return EXIT_FAILURE;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    po::options_description desc("lut site");
    desc.add_options()
            ("help", "produce help message")
            ("version", "print version info")
            ;

    ////////////////////////////////////////////////////////////////////////////////
    po::variables_map vars;
    try
    {
        po::store(po::command_line_parser(argc, argv).options(desc).run(), vars);
    }
    catch(std::exception &e)
    {
        LOGE("exception: "<<e.what());
        return EXIT_FAILURE;
    }
    catch(...)
    {
        LOGE("unknown exception");
        return EXIT_FAILURE;
    }
    po::notify(vars);

    ////////////////////////////////////////////////////////////////////////////////
    if(vars.count("version"))
    {
        std::cout << "this is a version info" << std::endl;
    }

    if(vars.count("help"))
    {
        std::cout << desc << std::endl;
        return EXIT_SUCCESS;
    }

    ////////////////////////////////////////////////////////////////////////////////
    {
        instance = new lut::site::impl::Instance{};

        signal(SIGINT,  signalHandler);
        signal(SIGTERM, signalHandler);

        std::error_code ec = instance->run();
        if(ec)
        {
            LOGE("run: "<<ec);
        }

        signal(SIGINT,  SIG_DFL);
        signal(SIGTERM, SIG_DFL);

        delete instance;
        instance = nullptr;

        if(ec)
        {
            LOGE(ec);
            return ec.value();
        }
    }

    return EXIT_SUCCESS;
}
