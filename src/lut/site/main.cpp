#include <cstdlib>

#include "lut/site/impl/instance.hpp"

#include <csignal>
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

namespace po = boost::program_options;


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
            instance->stop(lut::site::impl::StopMode::immediately);
        }
        break;

    case SIGINT:
    case SIGTERM:
        if(instance)
        {
            instance->stop(lut::site::impl::StopMode::graceful);
        }
        break;

    default:
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{

    ////////////////////////////////////////////////////////////////////////////////
    po::options_description desc("lut site");
    desc.add_options()
            ("help", "produce help message")
            ("version", "print version info")
            ("mode", po::value<std::string>()->default_value("master"), "mode: master, slave")
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
        std::string smode {vars["mode"].as<std::string>()};

        lut::site::impl::Mode mode;

        if("master" == smode)
        {
            mode = lut::site::impl::Mode::Master;
        }
        else if("slave" == smode)
        {
            mode = lut::site::impl::Mode::Master;
        }
        else
        {
            std::cerr<<"unknown mode: "<<smode<<std::endl;
            return EXIT_FAILURE;
        }

        instance = new lut::site::impl::Instance{mode};

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
