#include "lut/coupling/parser/impl/parse.hpp"
#include "lut/coupling/parser/impl/grammar.hpp"
#include "lut/coupling/parser/impl/grammarError.hpp"

#include <boost/filesystem.hpp>

#include <fstream>
#include <algorithm>
#include <functional>
#include <type_traits>

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

    namespace
    {
        class ScopeMerger
            : public boost::static_visitor<bool>
        {
            std::map<std::string, Scope> _scopes;
        public:
            static void exec(std::vector<Decl> &decls)
            {
                ScopeMerger instance;

                decls.erase(
                    std::remove_if(
                        decls.begin(),
                        decls.end(),
                        [&](Decl &d)->bool{return boost::apply_visitor(instance, d);}
                    )
                    , decls.end()
                );
            }

        public:
            bool operator()(const Scope &v)
            {
                auto ires = _scopes.insert(std::make_pair(v->name->value, v));
                if(ires.second)
                {
                    return false;
                }

                Scope old = ires.first->second;
                old->decls.insert(old->decls.end(), v->decls.begin(), v->decls.end());

                return true;
            }

            bool operator()(const Alias &v)
            {
                return false;
            }
            bool operator()(const Enum &v)
            {
                return false;
            }

            template <class TScope>
            bool operator()(const TScope &v)
            {
                exec(v->decls);
                return false;
            }
        };
    }

    namespace
    {
        class NamesChecker
            : public boost::static_visitor<bool>
        {
            std::vector<ErrorInfo> &_errs;

            std::map<std::string, TypeName> _typeNames;
            std::map<std::string, Name> _names;

            NamesChecker(std::vector<ErrorInfo> &errs)
                : _errs(errs)
            {}

        public:
            static bool exec(const std::vector<Decl> &decls, std::vector<ErrorInfo> &errs)
            {
                NamesChecker instance(errs);

                return std::accumulate(
                    decls.begin(),
                    decls.end(),
                    true,
                    [&](bool v, const Decl &d)->bool{return boost::apply_visitor(instance, d) && v;}
                );
            }

        public:
            template <class T>
            bool operator()(const T &v)
            {
                bool res = true;
                res &= checkTypeName(v.get());
                res &= checkFields(v.get());
                res &= checkChildren(v.get());
                return res;
            }

        private:
            bool checkTypeName(...)
            {
                return true;
            }

            template <class T>
            typename std::enable_if<sizeof(T::name)!=0, bool>::type checkTypeName(const T *v)
            {
                const TypeName &cur = v->name;
                auto ires = _typeNames.insert(std::make_pair(cur->value, cur));
                if(!ires.second)
                {
                    _errs.emplace_back(ErrorInfo {
                                          cur->pos.begin().file(),
                                          static_cast<int>(cur->pos.begin().line()),
                                          static_cast<int>(cur->pos.begin().column()),
                                          "duplicate name: "+cur->value});

                    const TypeName &prev = ires.first->second;

                    _errs.emplace_back(ErrorInfo {
                                          prev->pos.begin().file(),
                                          static_cast<int>(prev->pos.begin().line()),
                                          static_cast<int>(prev->pos.begin().column()),
                                          "previous declaration of "+prev->value});

                    return false;
                }

                return true;
            }

        private:
            bool checkFields(...)
            {
                return true;
            }

            template <class T>
            typename std::enable_if<sizeof(T::fields)!=0, bool>::type checkFields(const T *v)
            {
                bool res = true;
                for(const auto &f : v->fields)
                {
                    const Name &cur = f->name;

                    auto ires = _names.insert(std::make_pair(cur->value, cur));
                    if(!ires.second)
                    {
                        _errs.emplace_back(ErrorInfo {
                                              cur->pos.begin().file(),
                                              static_cast<int>(cur->pos.begin().line()),
                                              static_cast<int>(cur->pos.begin().column()),
                                              "duplicate name: "+cur->value});

                        const Name &prev = ires.first->second;

                        _errs.emplace_back(ErrorInfo {
                                              prev->pos.begin().file(),
                                              static_cast<int>(prev->pos.begin().line()),
                                              static_cast<int>(prev->pos.begin().column()),
                                              "previous declaration of "+prev->value});
                        res = false;
                    }
                }

                return res;
            }

        private:
            bool checkChildren(...)
            {
                return true;
            }

            template <class T>
            typename std::enable_if<std::is_base_of<SScope, T>::value, bool>::type checkChildren(const T *v)
            {
                return exec(v->decls, _errs);
            }
        };
    }

    namespace
    {
        class OwnerIndexer
            : public boost::static_visitor<>
        {
            std::stack<SScope*>     _scopes;
            std::stack<SStruct*>    _structs;
            std::stack<SVariant*>   _variants;
            std::stack<SIface*>     _ifaces;
            std::stack<SMethod*>    _methods;
            std::stack<SEnum*>      _enums;

        public:
            OwnerIndexer()
            {
                _scopes.push(nullptr);// simulate global scope as null value
            }

            template <class V>
            void exec(std::vector<V> &vs)
            {
                std::for_each(
                    vs.begin(),
                    vs.end(),
                    [&](const V &v)->void {boost::apply_visitor(*this, v);}
                );
            }

            void exec(std::vector<VariantField> &vs)
            {
                std::for_each(
                    vs.begin(),
                    vs.end(),
                    [&](const VariantField &v)->void {
                        v->owner = _variants.top();
                    }
                );
            }

            void exec(std::vector<StructField> &vs)
            {
                std::for_each(
                    vs.begin(),
                    vs.end(),
                    [&](const StructField &v)->void {
                        v->owner = _structs.top();
                    }
                );
            }

            void exec(std::vector<EnumField> &vs)
            {
                std::for_each(
                    vs.begin(),
                    vs.end(),
                    [&](const EnumField &v)->void {
                        v->owner = _enums.top();
                    }
                );
            }

            void exec(std::vector<Method> &vs)
            {
                std::for_each(
                    vs.begin(),
                    vs.end(),
                    [&](const Method &v)->void {
                        v->owner = _ifaces.top();

                        _methods.push(v.get());
                        exec(v->params);
                        _methods.pop();
                    }
                );
            }

            void exec(std::vector<MethodParam> &vs)
            {
                std::for_each(
                    vs.begin(),
                    vs.end(),
                    [&](const MethodParam &v)->void {
                        v->owner = _methods.top();
                    }
                );
            }

        public:
            template <class T>
            void operator()(T &v)
            {
                (*this)(v.get());
            }

            void operator()(SAlias *v)
            {
                v->owner = _scopes.top();
            }

            void operator()(SVariant *v)
            {
                v->owner = _scopes.top();

                _variants.push(v);
                _scopes.push(v);
                exec(v->decls);
                exec(v->fields);
                _scopes.pop();
                _variants.pop();
            }

            void operator()(SStruct *v)
            {
                v->owner = _scopes.top();

                _structs.push(v);
                _scopes.push(v);
                exec(v->decls);
                exec(v->fields);
                _scopes.pop();
                _structs.pop();
            }

            void operator()(SEnum *v)
            {
                v->owner = _scopes.top();

                _enums.push(v);
                exec(v->fields);
                _enums.pop();
            }

            void operator()(SIface *v)
            {
                v->owner = _scopes.top();

                _ifaces.push(v);
                _scopes.push(v);
                exec(v->decls);
                exec(v->fields);
                _scopes.pop();
                _ifaces.pop();
            }

            void operator()(SScope *v)
            {
                v->owner = _scopes.top();

                _scopes.push(v);
                exec(v->decls);
                _scopes.pop();
            }
        };

    }

    ////////////////////////////////////////////////////////////////////////////////
    bool parse(const std::vector<std::string> &fileNames, const Config &cfg, std::vector<ErrorInfo> &errs, std::vector<Decl> &decls)
    {
        {
            ParseState parseState = {cfg, errs, nullptr, {}, {}};
            Grammar g(parseState);
            parseState._grammar = &g;

            for(const std::string &fileName : fileNames)
            {
                if(!parse(fileName, parseState, decls))
                {
                    return false;
                }
            }
        }

        //merge scopes
        ScopeMerger::exec(decls);

        //check names uniqueness
        if(!NamesChecker::exec(decls, errs))
        {
            return false;
        }

        //index owners
        OwnerIndexer().exec(decls);

        //resolve typeUse.typeName

        //resolve bases

        return true;
    }

}}}}
