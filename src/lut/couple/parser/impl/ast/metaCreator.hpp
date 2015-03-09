#include "lut/couple/parser/errorInfo.hpp"
#include "lut/couple/parser/impl/ast.hpp"
#include "lut/couple/meta/libraryBuilder.hpp"
#include <algorithm>

namespace  lut { namespace couple { namespace parser { namespace impl { namespace ast
{
    class MetaCreator
        : public boost::static_visitor<>
    {
        meta::LibraryBuilder &_lb;

    public:
        MetaCreator(meta::LibraryBuilder &lb)
            : _lb(lb)
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
                    v->meta = _lb.addAttribute(v->owner->meta, v->name ? v->name->value : std::string());
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
                    v->meta = _lb.addAttribute(v->owner->meta, v->name->value);
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
                    v->meta = _lb.addEnumValue(v->owner->meta, v->name->value);
                }
            );
        }

        void exec(std::vector<Method> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const Method &v) {
                    v->meta = _lb.addMethod(v->owner->meta, v->name->value);
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
                    v->meta = _lb.addAttribute(v->owner->meta, v->name ? v->name->value : std::string());
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
            v->meta = _lb.addAlias(v->owner->meta, v->name->value);
            boost::apply_visitor(*this, v->type);
        }

        void operator()(SVariant *v)
        {
            v->meta = _lb.addVariant(v->owner->meta, v->name->value);
            v->SScope::meta = v->meta;

            exec(v->decls);
            exec(v->fields);
        }

        void operator()(SStruct *v)
        {
            v->meta = _lb.addStruct(v->owner->meta, v->name->value);
            v->SScope::meta = v->meta;

            exec(v->decls);
            exec(v->fields);
        }

        void operator()(SEnum *v)
        {
            v->meta = _lb.addEnum(v->owner->meta, v->name->value);

            exec(v->fields);
        }

        void operator()(SIface *v)
        {
            v->meta = _lb.addIface(v->owner->meta, v->name->value);
            v->SScope::meta = v->meta;

            exec(v->decls);
            exec(v->fields);
        }

        void operator()(SScope *v)
        {
            v->meta = _lb.addScope(v->owner ? v->owner->meta : nullptr, v->name ? v->name->value : std::string());

            exec(v->decls);
        }

        void operator()(SList *v)
        {
            v->meta = _lb.addList();
            boost::apply_visitor(*this, v->elementType);
        }

        void operator()(SSet *v)
        {
            v->meta = _lb.addSet();
            boost::apply_visitor(*this, v->elementType);
        }

        void operator()(SMap *v)
        {
            v->meta = _lb.addMap();
            boost::apply_visitor(*this, v->keyType);
            boost::apply_visitor(*this, v->valueType);
        }

        void operator()(SPtr *v)
        {
            v->meta = _lb.addPtr();
            boost::apply_visitor(*this, v->valueType);
        }

        void operator()(SArray *v)
        {
            v->meta = _lb.addArray();
            boost::apply_visitor(*this, v->elementType);
        }

        void operator()(SPrimitive *v)
        {
            v->meta = _lb.addPrimitive();
        }

        void operator()(SScopedName *v)
        {
        }

    };
}}}}}
