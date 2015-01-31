#pragma once

#include "lut/coupling/parser/impl/tokenizer.hpp"
#include "lut/coupling/parser/impl/ast.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{

    class Grammar
        : public qi::grammar<TokIterator, std::vector<Decl>()>
    {
    public:
        Grammar(const Tokenizer &toks);

    private:
        const Tokenizer &toks;
        qi::rule<TokIterator, void(std::string)> error;

    private:
        void mkPrimitive();
        void mkList();
        void mkSet();
        void mkMap();
        void mkPtr();
        void mkArray();
        void mkTypeName();
        void mkTypeUse();
        void mkAlias();
        void mkBases();
        void mkVariantField();
        void mkVariant();
        void mkStructField();
        void mkStruct();
        void mkEnumField();
        void mkEnum();
        void mkMethodParam();
        void mkMethod();
        void mkIface();
        void mkScope();
        void mkImport();
        void mkDecl();
        void mkDecls();


    private:
        qi::rule<TokIterator, Primitive()>                              primitive;
        qi::rule<TokIterator, List()>                                   list;
        qi::rule<TokIterator, Set()>                                    set;
        qi::rule<TokIterator, Map()>                                    map;
        qi::rule<TokIterator, Ptr()>                                    ptr;
        qi::rule<TokIterator, Array()>                                  array;
        qi::rule<TokIterator, TypeName()>                               typeName;
        qi::rule<TokIterator, TypeUse()>                                typeUse;
        qi::rule<TokIterator, Alias()>                                  alias;
        qi::rule<TokIterator, Bases()>                                  bases;
        qi::rule<TokIterator, VariantField()>                           variantField;
        qi::rule<TokIterator, Variant()>                                variant;
        qi::rule<TokIterator, StructField()>                            structField;
        qi::rule<TokIterator, Struct()>                                 struct_;
        qi::rule<TokIterator, EnumField()>                              enumField;
        qi::rule<TokIterator, Enum()>                                   enum_;
        qi::rule<TokIterator, MethodParam()>                            methodParam;
        qi::rule<TokIterator, Method(), qi::locals<MethodDirection>>    method;
        qi::rule<TokIterator, Iface()>                                  iface;
        qi::rule<TokIterator, Scope()>                                  scope;
        qi::rule<TokIterator, Import()>                                 import;
        qi::rule<TokIterator, Decl()>                                   decl;
        qi::rule<TokIterator, std::vector<Decl>()>                      decls;
    };

}}}}
