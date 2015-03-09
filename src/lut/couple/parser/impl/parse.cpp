#include "lut/couple/parser/impl/ast/namesChecker.hpp"
#include "lut/couple/parser/impl/ast/ownerIndexer.hpp"
#include "lut/couple/parser/impl/ast/scopedNamesResolver.hpp"
#include "lut/couple/parser/impl/ast/scopeMerger.hpp"
#include "lut/couple/parser/impl/ast/basesChecker.hpp"

#include "lut/couple/parser/impl/parse.hpp"
#include "lut/couple/parser/impl/grammar.hpp"
#include "lut/couple/parser/impl/grammarError.hpp"

#include <boost/filesystem.hpp>

#include <fstream>

namespace  lut { namespace couple { namespace parser { namespace impl
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
    Scope parse(const std::string &fileNameUnresolved, ParseState &parseState)
    {
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

        //load content
        std::shared_ptr<std::ifstream> reader{new std::ifstream{fileName}};
        if(!*reader)
        {
            parseState._errors.emplace_back(ErrorInfo {fileName, -1, -1, strerror(errno)});
            return false;
        }

        parseState._processedFiles.emplace(std::make_pair(fileName, reader));
        FNameRaii fnr(fileName, parseState);

        CharIterator lexBegin{boost::spirit::multi_pass<std::istreambuf_iterator<char>>{std::istreambuf_iterator<char>{*reader}}, fileName};
        CharIterator lexEnd{boost::spirit::multi_pass<std::istreambuf_iterator<char>>{std::istreambuf_iterator<char>{}}, fileName};

        auto pushError = [&](const CharIterator &pos, const std::string &msg)
        {
            parseState._errors.emplace_back(ErrorInfo {
                                  pos.file(),
                                  static_cast<int>(pos.line()),
                                  static_cast<int>(pos.column()),
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

        Scope res;

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
            return Scope();
        }

        if(!r)
        {
            pushError(tokIter->value().begin(), std::string("fail"));
            return Scope();
        }

        if(tokIter != tokEnd)
        {
            pushError(tokIter->value().begin(), std::string("unexpected extra input"));
            return Scope();
        }

        return res;
    }

    ////////////////////////////////////////////////////////////////////////////////
    Scope parse(const std::vector<std::string> &fileNames, const Config &cfg, std::vector<ErrorInfo> &errs)
    {
        Scope res;
        {
            ParseState parseState = {cfg, errs, nullptr, {}, {}};
            Grammar g(parseState);
            parseState._grammar = &g;

            for(const std::string &fileName : fileNames)
            {
                Scope localRes = parse(fileName, parseState);
                if(!localRes)
                {
                    return Scope();
                }

                if(!res)
                {
                    res = localRes;
                }
                else
                {
                    res->decls.insert(res->decls.end(), localRes->decls.begin(), localRes->decls.end());
                }
            }
        }

        //merge scopes
        ast::ScopeMerger().exec(res);

        //check names uniqueness
        if(!ast::NamesChecker(errs).exec(res))
        {
            return Scope();
        }

        //index owners
        ast::OwnerIndexer().exec(res);

        //resolve typeUse.scopedName
        if(!ast::ScopedNamesResolver(errs).exec(res))
        {
            return Scope();
        }

        //check bases for cycles
        if(!ast::BasesChecker(errs).exec(res))
        {
            return Scope();
        }

        return res;
    }

}}}}
