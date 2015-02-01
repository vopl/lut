#include "lut/coupling/parser/impl/parse.hpp"
#include "lut/coupling/parser/impl/grammar.hpp"
#include "lut/coupling/parser/impl/grammarError.hpp"

#include <boost/filesystem.hpp>

#include <fstream>

namespace  lut { namespace coupling { namespace parser { namespace impl
{
    namespace
    {
        ////////////////////////////////////////////////////////////////////////////////
        struct FNameRaii
        {
            std::string _old;
            ParseState &_parseState;

            FNameRaii(const std::string &fname, ParseState &parseState)
                : _old(parseState._currentFile)
                , _parseState(parseState)
            {
                _parseState._currentFile = fname;
            }

            ~FNameRaii()
            {
                _parseState._currentFile = _old;
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        std::string resolveFileName(const std::string &in, const ParseState &parseState, std::string &errorMessage)
        {
            try
            {

                boost::filesystem::path p(in);
                boost::filesystem::path candidate;
                if(p.is_absolute())
                {
                    candidate = p;
                    if(!boost::filesystem::exists(candidate))
                    {
                        errorMessage = "not found";
                        return std::string();
                    }
                }
                else
                {
                    boost::filesystem::path base;

                    if(parseState._currentFile.empty())
                    {
                        base = boost::filesystem::current_path();
                    }
                    else
                    {
                        base = boost::filesystem::absolute(parseState._currentFile).remove_filename();
                    }

                    if(boost::filesystem::exists(base / p))
                    {
                        candidate = base / p;
                    }
                    else
                    {
                        for(const std::string &b : parseState._cfg._includeDirectories)
                        {
                            if(boost::filesystem::exists(b / p))
                            {
                                candidate = b / p;
                                break;
                            }
                        }
                    }
                }

                if(boost::filesystem::is_symlink(candidate))
                {
                    candidate = boost::filesystem::read_symlink(candidate);
                }

                if(boost::filesystem::is_regular_file(candidate))
                {
                    candidate.normalize();
                    return candidate.generic_string();
                }
            }
            catch(const boost::filesystem::filesystem_error &err)
            {
                errorMessage = err.what();
                return std::string();
            }

            errorMessage = "not found";
            return std::string();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    bool parse(const std::string &fileNameUnresolved, ParseState &parseState, std::vector<Decl> &decls)
    {
        //TODO resolve file name
        std::string resolverErrorMessage;
        std::string fileName = resolveFileName(fileNameUnresolved, parseState, resolverErrorMessage);
        if(fileName.empty())
        {
            parseState._errors.emplace_back(ErrorInfo {fileNameUnresolved, -1, -1, resolverErrorMessage});
            return false;
        }

        //check cyclic
        if(parseState._processedFiles.end() != parseState._processedFiles.find(fileName))
        {
            parseState._errors.emplace_back(ErrorInfo {fileNameUnresolved, -1, -1, "error: cyclic inclusion"});
            return false;
        }

        parseState._processedFiles.insert(fileName);
        FNameRaii fnr(fileName, parseState);

        //load content
        std::ifstream reader{fileName};
        if(!reader)
        {
            parseState._errors.emplace_back(ErrorInfo {fileName, -1, -1, strerror(errno)});
            return false;
        }

        CharIterator lexBegin{boost::spirit::multi_pass<std::istreambuf_iterator<char>>{std::istreambuf_iterator<char>{reader}}};
        CharIterator lexEnd{boost::spirit::multi_pass<std::istreambuf_iterator<char>>{std::istreambuf_iterator<char>{}}};

        auto pushError = [&](const CharIterator &pos, const std::string &msg)
        {
            CharIterator bol = get_line_start(lexBegin, pos);
            parseState._errors.emplace_back(ErrorInfo {
                                  fileName,
                                  static_cast<int>(pos.position()),
                                  static_cast<int>(std::distance(bol, pos)),
                                  msg});
        };

        //tokenize
        TokIterator tokBegin;
        TokIterator tokEnd;

        {
            CharIterator lexIter{lexBegin};
            tokBegin = toks.begin(lexIter, lexEnd);
            tokEnd = toks.end();

            TokIterator tokIter = tokBegin;
            for(; tokIter != tokEnd; ++tokIter)
            {
                if(!token_is_valid(*tokIter))
                {
                    pushError(lexIter, "invalid input sequence");
                    return false;
                }
            }
        }

        //parse
        TokIterator tokIter{tokBegin};

        std::vector<Decl> res;

        bool r;
        try
        {
            r = qi::parse(
                tokIter,
                tokEnd,
                *parseState._grammar,
                res);
        }
        catch(const GrammarError &e)
        {
            pushError(e.pos(), std::string("error: ") + e.what());
            return false;
        }

        if(!r)
        {
            pushError(tokIter->value().begin(), std::string("fail"));
            return false;
        }

        if(tokIter != tokEnd)
        {
            pushError(tokIter->value().begin(), std::string("unexpected extra input"));
            return false;
        }

        decls.insert(decls.end(), res.begin(), res.end());
        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////
    bool parse(const std::string &fileName, const Config &cfg, std::vector<ErrorInfo> &errs, std::vector<Decl> &decls)
    {
        ParseState parseState = {cfg, errs, nullptr, {}, {}};
        Grammar g(parseState);
        parseState._grammar = &g;

        return parse(fileName, parseState, decls);
    }

}}}}
