#include "lut/coupling/parser/errorInfo.hpp"
#include "lut/coupling/parser/impl/ast.hpp"
#include <vector>
#include <algorithm>

namespace  lut { namespace coupling { namespace parser { namespace impl { namespace ast
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
                if(resolveOne(_rootScope, scopedName))
                {
                    return true;
                }

                _errs.emplace_back(ErrorInfo {
                                      scopedName.pos.begin().file(),
                                      static_cast<int>(scopedName.pos.begin().line()),
                                      static_cast<int>(scopedName.pos.begin().column()),
                                      "unable to resolve type name: " + scopedName.toString()});

                return false;
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
                                  "unable to resolve type name: " + scopedName.toString()});

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
                            v->bases->instances.push_back(nullptr);
                            res = false;
                        }
                        else if((typeid(v) != a->asDecl.type()))
                        {
                            _errs.emplace_back(ErrorInfo {
                                                  a->pos.begin().file(),
                                                  static_cast<int>(a->pos.begin().line()),
                                                  static_cast<int>(a->pos.begin().column()),
                                                  "resolved type name is incompatible: " + a->toString()});
                            v->bases->instances.push_back(nullptr);
                            res = false;
                        }
                        else
                        {
                            v->bases->instances.push_back(boost::get<decltype(v->bases->instances[0])>(a->asDecl));
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

}}}}}
