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

    namespace
    {
        class ScopeMerger
            : public boost::static_visitor<bool>
        {
            std::map<std::string, Scope> _scopes;
        public:
            void exec(const Scope &s)
            {
                return exec(s->decls);
            }

        private:
            void exec(std::vector<Decl> &decls)
            {
                decls.erase(
                    std::remove_if(
                        decls.begin(),
                        decls.end(),
                        [&](Decl &d)->bool{return boost::apply_visitor(*this, d);}
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
                ScopeMerger().exec(v->decls);
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

            std::map<std::string, Name> _typeNames;
            std::map<std::string, Name> _fieldNames;

        public:
            NamesChecker(std::vector<ErrorInfo> &errs)
                : _errs(errs)
            {}

        public:
            bool exec(const Scope &s)
            {
                return exec(s->decls);
            }

        private:
            bool exec(const std::vector<Decl> &decls)
            {
                return std::accumulate(
                    decls.begin(),
                    decls.end(),
                    true,
                    [&](bool v, const Decl &d)->bool{return boost::apply_visitor(*this, d) && v;}
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
                const Name &cur = v->name;
                auto ires = _typeNames.insert(std::make_pair(cur->value, cur));
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

                    auto ires = _fieldNames.insert(std::make_pair(cur->value, cur));
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
                return NamesChecker(_errs).exec(v->decls);
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
            }

            void exec(const Scope &s)
            {
                _scopes.push(s.get());
                exec(s->decls);
                _scopes.pop();
            }

        private:
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
                _scopes.top()->aliases.insert(std::make_pair(v->name->value, v));
            }

            void operator()(SVariant *v)
            {
                v->owner = _scopes.top();
                _scopes.top()->variants.insert(std::make_pair(v->name->value, v));
                _scopes.top()->scopes.insert(std::make_pair(v->name->value, v));

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
                _scopes.top()->structs.insert(std::make_pair(v->name->value, v));
                _scopes.top()->scopes.insert(std::make_pair(v->name->value, v));

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
                _scopes.top()->enums.insert(std::make_pair(v->name->value, v));

                _enums.push(v);
                exec(v->fields);
                _enums.pop();
            }

            void operator()(SIface *v)
            {
                v->owner = _scopes.top();
                _scopes.top()->ifaces.insert(std::make_pair(v->name->value, v));
                _scopes.top()->scopes.insert(std::make_pair(v->name->value, v));

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
                _scopes.top()->scopes.insert(std::make_pair(v->name->value, v));

                _scopes.push(v);
                exec(v->decls);
                _scopes.pop();
            }
        };

    }

    namespace
    {
        class ScopedNamesResolver
                : public boost::static_visitor<bool>
        {
            std::vector<ErrorInfo> &_errs;
            SScope *_rootScope{nullptr};
        public:
            ScopedNamesResolver(std::vector<ErrorInfo> &errs)
                : _errs(errs)
            {
            }

        public:
            bool exec(const Scope &s)
            {
                assert(!_rootScope);
                _rootScope = s.get();
                bool res = exec(s->decls);
                _rootScope = nullptr;
                return res;
            }

        private:
            template <class V>
            bool exec(std::vector<V> &vs)
            {
                return std::accumulate(
                    vs.begin(),
                    vs.end(),
                    true,
                    [&](bool state, const V &v)->bool {return boost::apply_visitor(*this, v) && state;}
                );
            }

            bool exec(std::vector<EnumField> &)
            {
                return true;
            }

            bool exec(std::vector<VariantField> &vs)
            {
                return std::accumulate(
                    vs.begin(),
                    vs.end(),
                    true,
                    [&](bool state, const VariantField &v)->bool {
                        return resolve(v->owner, v->type) && state;
                    }
                );
            }

            bool exec(std::vector<StructField> &vs)
            {
                return std::accumulate(
                    vs.begin(),
                    vs.end(),
                    true,
                    [&](bool state, const StructField &v)->bool {
                        return resolve(v->owner, v->type) && state;
                    }
                );
            }

            bool exec(std::vector<Method> &vs)
            {
                return std::accumulate(
                    vs.begin(),
                    vs.end(),
                    true,
                    [&](bool state, const Method &v)->bool {
                        state &= resolve(v->owner, v->resultType);
                        state &= exec(v->params);
                        return state;
                    }
                );
            }

            bool exec(std::vector<MethodParam> &vs)
            {
                return std::accumulate(
                    vs.begin(),
                    vs.end(),
                    true,
                    [&](bool state, const MethodParam &v)->bool {
                        return resolve(v->owner->owner, v->type) && state;
                    }
                );
            }

        private:
            template <class T>
            bool resolveOne(const std::map<std::string, T> &map, const std::string &name, SScopedName &scopedName)
            {
                auto target = map.find(name);

                if(map.end() == target)
                {
                    return false;
                }

                scopedName.asDecl = target->second;
                scopedName.asScopedEntry = target->second;
                return true;
            }

            bool resolveOne(const SScope *scope, SScopedName &scopedName)
            {
                auto iter = scopedName.values.begin();
                auto last = --scopedName.values.end();

                for(; iter!=last; ++iter)
                {
                    const Name &name = *iter;
                    auto scopeIter = scope->scopes.find(name->value);

                    if(scope->scopes.end() == scopeIter)
                    {
                        return false;
                    }
                    scope = scopeIter->second;
                }

                const Name &name = *last;
                const std::string &nameValue = name->value;

                if(resolveOne(scope->aliases, nameValue, scopedName))
                {
                    return true;
                }

                if(resolveOne(scope->structs, nameValue, scopedName))
                {
                    return true;
                }

                if(resolveOne(scope->variants, nameValue, scopedName))
                {
                    return true;
                }

                if(resolveOne(scope->enums, nameValue, scopedName))
                {
                    return true;
                }

                if(resolveOne(scope->ifaces, nameValue, scopedName))
                {
                    return true;
                }

                return false;
            }

            bool resolve(const SScope *scope, SScopedName &scopedName)
            {
                assert(!scopedName.values.empty());

                if(scopedName.root)
                {
                    return resolveOne(_rootScope, scopedName);
                }

                for(; scope; scope=scope->owner)
                {
                    if(resolveOne(scope, scopedName))
                    {
                        return true;
                    }
                }

                _errs.emplace_back(ErrorInfo {
                                      scopedName.pos.begin().file(),
                                      static_cast<int>(scopedName.pos.begin().line()),
                                      static_cast<int>(scopedName.pos.begin().column()),
                                      "unable to resolve type name: " + scopedName.values.back()->value});

                return false;
            }

            bool resolve(const SScope *scope, TypeUse &typeUse)
            {
                ScopedName *sn = boost::get<ScopedName>(&typeUse);
                if(!sn)
                {
                    return true;
                }
                assert(*sn);

                return resolve(scope, *sn->get());
            }

        public:
            template <class T>
            bool operator()(T &v)
            {
                bool res = true;

                res &= resolveScope(v.get());
                res &= resolveBases(v.get());
                res &= resolveFields(v.get());
                res &= resolveAlias(v.get());

                return res;
            }

            /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
            bool resolveScope(...)
            {
                return true;
            }

            bool resolveScope(SScope *v)
            {
                return exec(v->decls);
            }

            /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
            bool resolveBases(...)
            {
                return true;
            }

            template <class T>
            typename std::enable_if<sizeof(T::bases)!=0, bool>::type resolveBases(T *v)
            {
                bool res = true;
                if(v->bases)
                {
                    std::for_each(
                        v->bases->scopedNames.begin(),
                        v->bases->scopedNames.end(),
                        [&](const ScopedName &a) {
                            bool localRes = resolve(v->owner, *a);
                            if(!localRes)
                            {
                                res = false;
                            }
                            else if((typeid(v) != a->asDecl.type()))
                            {
                                _errs.emplace_back(ErrorInfo {
                                                      a->pos.begin().file(),
                                                      static_cast<int>(a->pos.begin().line()),
                                                      static_cast<int>(a->pos.begin().column()),
                                                      "resolved type name is incompatible: " + a->values.back()->value});
                                res = false;
                            }
                        }
                    );
                }

                return res;
            }

            /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
            bool resolveFields(...)
            {
                return true;
            }

            template <class T>
            typename std::enable_if<sizeof(T::fields)!=0, bool>::type resolveFields(T *v)
            {
                return exec(v->fields);
            }

            /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
            bool resolveAlias(...)
            {
                return true;
            }

            bool resolveAlias(SAlias *v)
            {
                return resolve(v->owner, v->type);
            }
        };
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
        ScopeMerger().exec(res);

        //check names uniqueness
        if(!NamesChecker(errs).exec(res))
        {
            return Scope();
        }

        //index owners
        OwnerIndexer().exec(res);

        //resolve typeUse.scopedName
        if(!ScopedNamesResolver(errs).exec(res))
        {
            return Scope();
        }

        //check bases

        return res;
    }

}}}}
