#include "lut/stable.hpp"

#include "lut/coupling/meta/library.hpp"
#include "lut/coupling/parser/config.hpp"
#include "lut/coupling/parser/errorInfo.hpp"
#include "lut/coupling/parser/exec.hpp"


#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

namespace po = boost::program_options;

int main(int argc, const char **argv)
{
    ////////////////////////////////////////////////////////////////////////////////
    po::options_description desc("general options");
    desc.add_options()
            ("help", "produce help message")
            ("version", "print version info")
            ;

    ////////////////////////////////////////////////////////////////////////////////
    po::options_description descInput("input options");
    descInput.add_options()
            (
                "in,i",
                boost::program_options::value<std::vector<std::string>>(),
                "input file name"
            );

    desc.add(descInput);

    ////////////////////////////////////////////////////////////////////////////////
    po::options_description descOutput("output options");
    descOutput.add_options()
            (
                "out,o",
                boost::program_options::value<std::string>(),
                "output file name"
            );
    desc.add(descOutput);

    ////////////////////////////////////////////////////////////////////////////////
    po::positional_options_description p;
    p.add("in", -1);

    ////////////////////////////////////////////////////////////////////////////////
    po::variables_map vars;
    try
    {
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vars);
    }
    catch(std::exception &e)
    {
        std::cout<<e.what()<<std::endl;
        return EXIT_FAILURE;
    }
    catch(...)
    {
        std::cout<<"unknown exception"<<std::endl;
        return EXIT_FAILURE;
    }
    po::notify(vars);

    ////////////////////////////////////////////////////////////////////////////////
    if(vars.empty() || vars.count("version"))
    {
        std::cout << "this is a version info" << std::endl;
    }

    if(vars.count("help"))
    {
        std::cout << desc << std::endl;
        return EXIT_SUCCESS;
    }

    lut::coupling::meta::Library lib;

    if(vars.count("in"))
    {

        lut::coupling::parser::Config cfg;
        //fill cfg from program options

        for(const std::string &in: vars["in"].as<std::vector<std::string>>())
        {
            switch(lib.load(in))
            {
            case lut::coupling::meta::LoadResult::ok:
                break;
            case lut::coupling::meta::LoadResult::corruptedFile:
                std::cerr << "corrupted file: " << in << std::endl;
                return EXIT_FAILURE;
            case lut::coupling::meta::LoadResult::malformedFile:
                {
                    std::vector<lut::coupling::parser::ErrorInfo> errs;
                    if(!lut::coupling::parser::exec(in, cfg, lib, errs))
                    {
                        for(const lut::coupling::parser::ErrorInfo &err : errs)
                        {
                            std::cerr << err.toString() << std::endl;
                        }
                        return EXIT_FAILURE;
                    }

                    break;
                }
            }
        }
    }

    if(!lib.resolve())
    {
        //TODO
        //std::cerr << "incomplete types: " << lib.incomplete() << std::endl;
        return EXIT_FAILURE;
    }

    if(vars.count("out"))
    {
        std::cout << "output: " << vars["out"].as<std::string>() << std::endl;
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}
