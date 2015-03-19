#pragma once

#include "tokenizer.hpp"
#include "ast.hpp"
#include "parseState.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{

    class Grammar
        : public qi::grammar<TokIterator, Scope()>
    {
    public:
        Grammar(ParseState &parseState);

    public:
        ParseState &_parseState;

    private:
        qi::rule<TokIterator, void(std::string)> error;

    private:
        void mkPrimitive();
        void mkList();
        void mkSet();
        void mkMap();
        void mkPtr();
        void mkArray();
        void mkName();
        void mkScopedName();
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
        void mkInclude();
        void mkDecl();
        void mkDecls();
        void mkFile();

        std::vector<Decl> doInclude(const Token::token_value_type &str);

    private:
        qi::rule<TokIterator, Primitive(), qi::locals<PrimitiveKind>>   primitive;
        qi::rule<TokIterator, List()>                                   list;
        qi::rule<TokIterator, Set()>                                    set;
        qi::rule<TokIterator, Map()>                                    map;
        qi::rule<TokIterator, Ptr()>                                    ptr;
        qi::rule<TokIterator, Array()>                                  array;
        qi::rule<TokIterator, Name()>                                   name;
        qi::rule<TokIterator, ScopedName()>                             scopedName;
        qi::rule<TokIterator, TypeUse()>                                typeUse;
        qi::rule<TokIterator, Alias()>                                  alias;
        qi::rule<TokIterator, BaseStructs()>                            baseStructs;
        qi::rule<TokIterator, BaseVariants()>                           baseVariants;
        qi::rule<TokIterator, BaseEnums()>                              baseEnums;
        qi::rule<TokIterator, BaseIfaces()>                             baseIfaces;
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
        qi::rule<TokIterator, std::vector<Decl>()>                      include;
        qi::rule<TokIterator, Decl()>                                   decl;
        qi::rule<TokIterator, std::vector<Decl>()>                      decls;

        qi::rule<TokIterator, Scope()>                                  file;
    };

}}}}
