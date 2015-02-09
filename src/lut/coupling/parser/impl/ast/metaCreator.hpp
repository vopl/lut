#include "lut/coupling/parser/errorInfo.hpp"
#include "lut/coupling/parser/impl/ast.hpp"
#include "lut/coupling/meta/library.hpp"
#include <algorithm>

namespace  lut { namespace coupling { namespace parser { namespace impl { namespace ast
{
    class MetaCreator
        : public boost::static_visitor<>
    {
        meta::Library &_lib;

    public:
        MetaCreator(meta::Library &lib)
            : _lib(lib)
        {
        }

        void exec(const Scope &s)
        {
            this->operator()(s.get());
        }

    private:
        template <class V>
        void exec(std::vector<V> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const V &v) {boost::apply_visitor(*this, v);}
            );
        }

        void exec(std::vector<VariantField> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const VariantField &v) {
                    v->meta = _lib.addAttribute(v->owner->meta, v->name->value);
                    boost::apply_visitor(*this, v->type);
                }
            );
        }

        void exec(std::vector<StructField> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const StructField &v) {
                    v->meta = _lib.addAttribute(v->owner->meta, v->name->value);
                    boost::apply_visitor(*this, v->type);
                }
            );
        }

        void exec(std::vector<EnumField> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const EnumField &v) {
                    v->meta = _lib.addEnumValue(v->owner->meta, v->name->value);
                }
            );
        }

        void exec(std::vector<Method> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const Method &v) {
                    v->meta = _lib.addMethod(v->owner->meta, v->name->value);
                    exec(v->params);
                    boost::apply_visitor(*this, v->resultType);
                }
            );
        }

        void exec(std::vector<MethodParam> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const MethodParam &v) {
                    v->meta = _lib.addAttribute(v->owner->meta, v->name->value);
                    boost::apply_visitor(*this, v->type);
                }
            );
        }

    public:
        template <class T>
        void operator()(const std::shared_ptr<T> v)
        {
            (*this)(v.get());
        }

        template <class T>
        void operator()(T &v)
        {
            (*this)(&v);
        }

        void operator()(SAlias *v)
        {
            v->meta = _lib.addAlias(v->owner->meta, v->name->value);
            boost::apply_visitor(*this, v->type);
        }

        void operator()(SVariant *v)
        {
            v->meta = _lib.addVariant(v->owner->meta, v->name->value);

            exec(v->decls);
            exec(v->fields);
        }

        void operator()(SStruct *v)
        {
            v->meta = _lib.addStruct(v->owner->meta, v->name->value);

            exec(v->decls);
            exec(v->fields);
        }

        void operator()(SEnum *v)
        {
            v->meta = _lib.addEnum(v->owner->meta, v->name->value);

            exec(v->fields);
        }

        void operator()(SIface *v)
        {
            v->meta = _lib.addIface(v->owner->meta, v->name->value);

            exec(v->decls);
            exec(v->fields);
        }

        void operator()(SScope *v)
        {
            v->meta = _lib.addScope(v->owner ? v->owner->meta : nullptr, v->name->value);

            exec(v->decls);
        }

        void operator()(SList *v)
        {
            v->meta = _lib.addList();
            boost::apply_visitor(*this, v->elementType);
        }

        void operator()(SSet *v)
        {
            v->meta = _lib.addSet();
            boost::apply_visitor(*this, v->elementType);
        }

        void operator()(SMap *v)
        {
            v->meta = _lib.addMap();
            boost::apply_visitor(*this, v->keyType);
            boost::apply_visitor(*this, v->valueType);
        }

        void operator()(SPtr *v)
        {
            v->meta = _lib.addPtr();
            boost::apply_visitor(*this, v->valueType);
        }

        void operator()(SArray *v)
        {
            v->meta = _lib.addArray();
            boost::apply_visitor(*this, v->elementType);
        }

        void operator()(SPrimitive *v)
        {
            v->meta = _lib.addPrimitive();
        }

        void operator()(SScopedName *v)
        {
        }

    };
}}}}}
