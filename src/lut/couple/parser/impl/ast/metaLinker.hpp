#include "lut/couple/parser/errorInfo.hpp"
#include "lut/couple/parser/impl/ast.hpp"
#include "lut/couple/meta/libraryBuilder.hpp"
#include <algorithm>

namespace  lut { namespace couple { namespace parser { namespace impl { namespace ast
{
    class MetaLinker
        : public boost::static_visitor<>
    {
        meta::LibraryBuilder &_lb;

    public:
        MetaLinker(meta::LibraryBuilder &lb)
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
                    boost::apply_visitor(*this, v->type);
                }
            );
        }

        void exec(std::vector<EnumField> &vs)
        {
//            std::for_each(
//                vs.begin(),
//                vs.end(),
//                [&](const EnumField &v) {
//                }
//            );
        }

        void exec(std::vector<Method> &vs)
        {
            std::for_each(
                vs.begin(),
                vs.end(),
                [&](const Method &v) {
                    _lb.setMethodDirection(v->meta, MethodDirection::in==v->direction ? meta::CallDirection::in : meta::CallDirection::out);
                    _lb.setMethodNowait(v->meta, v->nowait);
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
            boost::apply_visitor(*this, v->type);
        }

        void operator()(SVariant *v)
        {
            if(v->bases)
            {
                std::for_each(
                    v->bases->instances.begin(),
                    v->bases->instances.end(),
                    [&](SVariant *b) {
                        _lb.addBase(v->meta, b->meta);
                    }
                );
            }

            exec(v->decls);
            exec(v->fields);
        }

        void operator()(SStruct *v)
        {
            if(v->bases)
            {
                std::for_each(
                    v->bases->instances.begin(),
                    v->bases->instances.end(),
                    [&](SStruct *b) {
                        _lb.addBase(v->meta, b->meta);
                    }
                );
            }

            exec(v->decls);
            exec(v->fields);
        }

        void operator()(SEnum *v)
        {
            if(v->bases)
            {
                std::for_each(
                    v->bases->instances.begin(),
                    v->bases->instances.end(),
                    [&](SEnum *b) {
                        _lb.addBase(v->meta, b->meta);
                    }
                );
            }

            exec(v->fields);
        }

        void operator()(SIface *v)
        {
            if(v->bases)
            {
                std::for_each(
                    v->bases->instances.begin(),
                    v->bases->instances.end(),
                    [&](SIface *b) {
                        _lb.addBase(v->meta, b->meta);
                    }
                );
            }

            exec(v->decls);
            exec(v->fields);
        }

        void operator()(SScope *v)
        {
            exec(v->decls);
        }

        struct TypeUseMetaFetcher
            : public boost::static_visitor<meta::Type *>
        {
            template <class T>
            meta::Type *operator()(const T &v) const
            {
                return v->meta;
            }

            struct ScopedNameMetaFetcher
                : public boost::static_visitor<meta::Type *>
            {
                template <class T>
                meta::Type *operator()(const T &v) const
                {
                    return v->meta;
                }
            };

            meta::Type *operator()(const ScopedName &v) const
            {
                return boost::apply_visitor(ScopedNameMetaFetcher(), v->asDecl);
            }
        };

        meta::Type *typeUse2Meta(TypeUse &tu)
        {
            return boost::apply_visitor(TypeUseMetaFetcher(), tu);
        }

        void operator()(SList *v)
        {
            _lb.setElementType(v->meta, typeUse2Meta(v->elementType));
        }

        void operator()(SSet *v)
        {
            _lb.setElementType(v->meta, typeUse2Meta(v->elementType));
        }

        void operator()(SMap *v)
        {
            _lb.setElementType(v->meta, typeUse2Meta(v->keyType), typeUse2Meta(v->valueType));
        }

        void operator()(SPtr *v)
        {
            _lb.setElementType(v->meta, typeUse2Meta(v->valueType));
        }

        void operator()(SArray *v)
        {
            _lb.setElementType(v->meta, typeUse2Meta(v->elementType));

            //TODO
            try
            {
                _lb.setArraySize(v->meta, static_cast<std::uint32_t>(std::stoull(v->size)));
            }
            catch(const std::runtime_error &e)
            {
                std::cerr<<e.what()<<std::endl;
                _lb.setArraySize(v->meta, 0);
            }
        }

        void operator()(SPrimitive *v)
        {
        }

        void operator()(SScopedName *v)
        {
        }

    };
}}}}}
