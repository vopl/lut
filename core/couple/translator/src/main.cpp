#include <dci/couple/meta/libraryBuilder.hpp>
#include <dci/couple/meta/library.hpp>
#include <dci/couple/parser/config.hpp>
#include <dci/couple/parser/errorInfo.hpp>
#include <dci/couple/parser/exec.hpp>

#include <dci/logger/logger.hpp>

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
                po::value<std::vector<std::string>>(),
                "input file name"
            )
            (
                "include,I",
                po::value<std::vector<std::string>>(),
                "include directories"
            );


    desc.add(descInput);

    ////////////////////////////////////////////////////////////////////////////////
    po::options_description descOutput("output options");
    descOutput.add_options()
            (
                "out,o",
                po::value<std::string>(),
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
    if(vars.empty() || vars.count("version"))
    {
        std::cout << "this is a version info" << std::endl;
    }

    if(vars.count("help"))
    {
        std::cout << desc << std::endl;
        return EXIT_SUCCESS;
    }

    dci::couple::meta::Library lib;

    if(vars.count("in"))
    {

        dci::couple::parser::Config cfg;
        if(vars.count("include"))
        {
            cfg._includeDirectories = vars["include"].as<std::vector<std::string>>();
        }

        std::vector<std::string> idlFiles;

        dci::couple::meta::LibraryBuilder lb;

        for(const std::string &in: vars["in"].as<std::vector<std::string>>())
        {
            dci::couple::meta::Library lib;
            switch(lib.load(in))
            {
            case dci::couple::meta::LoadResult::ok:
                lb.merge(lib);
                std::cout << "file loaded: " << in << std::endl;
                break;
            case dci::couple::meta::LoadResult::corruptedFile:
                std::cerr << "corrupted file: " << in << std::endl;
                return EXIT_FAILURE;
            case dci::couple::meta::LoadResult::malformedFile:
                idlFiles.push_back(in);
            }
        }

        {
            std::vector<dci::couple::parser::ErrorInfo> errs;
            if(!dci::couple::parser::exec(idlFiles, cfg, errs, lb))
            {
                std::cerr << "parse failed" << std::endl;
                for(const dci::couple::parser::ErrorInfo &err : errs)
                {
                    std::cerr << err.toString() << std::endl;
                }
                return EXIT_FAILURE;
            }

            for(const std::string &idlFile : idlFiles)
            {
                std::cout << "parsed: " << idlFile << std::endl;
            }
        }

        std::vector<dci::couple::meta::CommitError> errs;
        if(!lb.commitChanges(lib, errs))
        {
            for(const dci::couple::meta::CommitError &err : errs)
            {
                std::cerr << err.toString() << std::endl;
            }
            return EXIT_FAILURE;
        }
    }

    if(vars.count("out"))
    {
        std::cout << "output: " << vars["out"].as<std::string>() << std::endl;
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}
