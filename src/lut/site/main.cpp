#include <cstdlib>

#include "lut/site/impl/instance.hpp"
#include "lut/async/functions.hpp"

#include <csignal>
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;


/*
 *
 * master:          start, stop
 * master/slave:    control
 * site/site        interact
 *
 *
 *
*/


lut::site::impl::Instance *instance = nullptr;

void signalHandler(int signum)
{
    switch(signum)
    {
    case SIGKILL:
        if(instance)
        {
            lut::async::spawn([](){
                std::error_code ec = instance->stop(false).value<0>();
                if(ec)
                {
                    std::cerr<<"stop: "<<ec.message()<<std::endl;
                }
            });
        }
        break;

    case SIGINT:
    case SIGTERM:
        if(instance)
        {
            lut::async::spawn([](){
                std::error_code ec = instance->stop(true).value<0>();
                if(ec)
                {
                    std::cerr<<"graceful stop: "<<ec.message()<<std::endl;
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
    fs::current_path(fs::system_complete(argv[0]).normalize().parent_path().parent_path());

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
        std::cerr<<e.what()<<std::endl;
        return EXIT_FAILURE;
    }
    catch(...)
    {
        std::cerr<<"unknown exception"<<std::endl;
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
        signal(SIGKILL, signalHandler);

        std::error_code ec = instance->run();

        signal(SIGINT,  SIG_DFL);
        signal(SIGTERM, SIG_DFL);
        signal(SIGKILL, SIG_DFL);

        delete instance;
        instance = nullptr;

        if(ec)
        {
            std::cerr<<ec.message()<<std::endl;
            return ec.value();
        }
    }

    return EXIT_SUCCESS;
}
