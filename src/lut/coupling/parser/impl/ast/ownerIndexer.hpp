#include "lut/coupling/parser/errorInfo.hpp"
#include "lut/coupling/parser/impl/ast.hpp"
#include <algorithm>

namespace  lut { namespace coupling { namespace parser { namespace impl { namespace ast
{
    class OwnerIndexer
        : public boost::static_visitor<>
    {
        SScope      *_scope;
        SStruct     *_struct;
        SVariant    *_variant;
        SIface      *_iface;
        SMethod     *_method;
        SEnum       *_enum;

        template <class T>
        class CurrentSetter
        {
            T *&_storage;
            T *_originalValue;

        public:
            CurrentSetter(T *&storage, T *value)
                : _storage(storage)
                , _originalValue(storage)
            {
                _storage = value;
            }

            ~CurrentSetter()
            {
                _storage = _originalValue;
            }

        };

    public:
        OwnerIndexer()
        {
        }

        void exec(const Scope &s)
        {
            CurrentSetter<SScope> css(_scope, s.get());
            exec(s->decls);
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
                    v->owner = _variant;
                }
            );
        }

        void exec(std::vector<StructField> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const StructField &v)->void {
                    v->owner = _struct;
                }
            );
        }

        void exec(std::vector<EnumField> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const EnumField &v)->void {
                    v->owner = _enum;
                }
            );
        }

        void exec(std::vector<Method> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const Method &v)->void {
                    v->owner = _iface;

                    CurrentSetter<SMethod> csm(_method, v.get());
                    exec(v->params);
                }
            );
        }

        void exec(std::vector<MethodParam> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const MethodParam &v)->void {
                    v->owner = _method;
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
            v->owner = _scope;
            _scope->aliases.insert(std::make_pair(v->name->value, v));
        }

        void operator()(SVariant *v)
        {
            v->owner = _scope;
            _scope->variants.insert(std::make_pair(v->name->value, v));
            _scope->scopes.insert(std::make_pair(v->name->value, v));

            CurrentSetter<SVariant> csv(_variant, v);
            CurrentSetter<SScope> css(_scope, v);
            exec(v->decls);
            exec(v->fields);
        }

        void operator()(SStruct *v)
        {
            v->owner = _scope;
            _scope->structs.insert(std::make_pair(v->name->value, v));
            _scope->scopes.insert(std::make_pair(v->name->value, v));

            CurrentSetter<SStruct> csst(_struct, v);
            CurrentSetter<SScope> css(_scope, v);
            exec(v->decls);
            exec(v->fields);
        }

        void operator()(SEnum *v)
        {
            v->owner = _scope;
            _scope->enums.insert(std::make_pair(v->name->value, v));

            CurrentSetter<SEnum> cse(_enum, v);
            exec(v->fields);
        }

        void operator()(SIface *v)
        {
            v->owner = _scope;
            _scope->ifaces.insert(std::make_pair(v->name->value, v));
            _scope->scopes.insert(std::make_pair(v->name->value, v));

            CurrentSetter<SIface> csi(_iface, v);
            CurrentSetter<SScope> css(_scope, v);
            exec(v->decls);
            exec(v->fields);
        }

        void operator()(SScope *v)
        {
            v->owner = _scope;
            _scope->scopes.insert(std::make_pair(v->name->value, v));

            CurrentSetter<SScope> css(_scope, v);
            exec(v->decls);
        }
    };
}}}}}
