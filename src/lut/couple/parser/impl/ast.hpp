#pragma once

#include "lut/couple/parser/impl/common.hpp"
#include "lut/couple/parser/impl/tokenizer.hpp"

#include <boost/variant.hpp>

namespace lut { namespace couple { namespace meta
{
    class Primitive;
    class List;
    class Set;
    class Map;
    class Ptr;
    class Array;
    class Scope;
    class Alias;
    class Attribute;
    class Variant;
    class Attribute;
    class Struct;
    class EnumValue;
    class Enum;
    class Attribute;
    class Method;
    class Iface;
}}}

namespace lut { namespace couple { namespace parser { namespace impl
{
    /*
        primitive

        list
        set
        map
        ptr
        array

        name

        scopedName

        typeUse = primitive | list | set | map | ptr | array | scopedName

        alias

        bases

        variantField
        variant

        structField
        struct_

        enumField
        enum_

        methodParam
        method
        iface

        scope

        include

        decl = alias | variant | struct | enum | iface | scope

        decls = *decl | include



    */


    ////////////////////////////////////////////////////////////////////////////////
    // predecls

    //    primitive
    struct SPrimitive;
    using Primitive = std::shared_ptr<SPrimitive>;

    //    list
    struct SList;
    using List = std::shared_ptr<SList>;

    //    set
    struct SSet;
    using Set = std::shared_ptr<SSet>;

    //    map
    struct SMap;
    using Map = std::shared_ptr<SMap>;

    //    ptr
    struct SPtr;
    using Ptr = std::shared_ptr<SPtr>;

    //    array
    struct SArray;
    using Array = std::shared_ptr<SArray>;

    //    name
    struct SName;
    using Name = std::shared_ptr<SName>;

    //    scopedName
    struct SScopedName;
    using ScopedName = std::shared_ptr<SScopedName>;


    //    typeUse = primitive | list | set | map | ptr | array | scopedName
    using TypeUse = boost::variant<
          Primitive
        , List
        , Set
        , Map
        , Ptr
        , Array
        , ScopedName
    >;

    //    alias
    struct SAlias;
    using Alias = std::shared_ptr<SAlias>;

    //    bases
    struct SBaseStructs;
    using BaseStructs = std::shared_ptr<SBaseStructs>;

    struct SBaseVariants;
    using BaseVariants = std::shared_ptr<SBaseVariants>;

    struct SBaseEnums;
    using BaseEnums = std::shared_ptr<SBaseEnums>;

    struct SBaseIfaces;
    using BaseIfaces = std::shared_ptr<SBaseIfaces>;

    //    structField
    struct SStructField;
    using StructField = std::shared_ptr<SStructField>;

    //    struct_
    struct SStruct;
    using Struct = std::shared_ptr<SStruct>;

    //    variantField
    struct SVariantField;
    using VariantField = std::shared_ptr<SVariantField>;

    //    variant
    struct SVariant;
    using Variant = std::shared_ptr<SVariant>;

    //    enumField
    struct SEnumField;
    using EnumField = std::shared_ptr<SEnumField>;

    //    enum_
    struct SEnum;
    using Enum = std::shared_ptr<SEnum>;

    //    methodParam
    struct SMethodParam;
    using MethodParam = std::shared_ptr<SMethodParam>;

    //    method
    struct SMethod;
    using Method = std::shared_ptr<SMethod>;

    //    iface
    struct SIface;
    using Iface = std::shared_ptr<SIface>;

    struct SScopeEntry;

    //    scope
    struct SScope;
    using Scope = std::shared_ptr<SScope>;

    //    include

    //    decl = alias | variant | struct | enum | iface | scope
    using Decl = boost::variant<
          Alias
        , Variant
        , Struct
        , Enum
        , Iface
        , Scope
    >;

    //    decls = *decl | include


    ////////////////////////////////////////////////////////////////////////////////
    //    primitive
    enum class PrimitiveKind
    {
        void_,

        bool_,

        string,

        int8,
        int16,
        int32,
        int64,

        uint8,
        uint16,
        uint32,
        uint64,

        real32,
        real64,
    };

    struct SPrimitive
    {
        PrimitiveKind   kind;
        meta::Primitive *meta{nullptr};
    };

    //    list
    struct SList
    {
        TypeUse     elementType;

        meta::List  *meta{nullptr};
    };

    //    set
    struct SSet
    {
        TypeUse     elementType;

        meta::Set   *meta{nullptr};
    };

    //    map
    struct SMap
    {
        TypeUse     keyType;
        TypeUse     valueType;

        meta::Map   *meta{nullptr};
    };

    //    ptr
    struct SPtr
    {
        TypeUse     valueType;

        meta::Ptr   *meta{nullptr};
    };

    //    array
    struct SArray
    {
        TypeUse     elementType;
        std::string size;

        meta::Array *meta{nullptr};
    };

    //    name
    struct SName
    {
        Token::token_value_type pos;
        std::string             value;
    };

    //    scopedName
    struct SScopedName
    {
        Token::token_value_type pos;
        bool                    root{false};
        std::vector<Name>       values;

        boost::variant<
              SAlias *
            , SVariant *
            , SStruct *
            , SEnum *
            , SIface *
        > asDecl;

        SScopeEntry *asScopedEntry;

        std::string toString()
        {
            std::string res = std::accumulate(
                        values.begin(),
                        values.end(),
                        std::string(),
                        [](const std::string &state, const Name &v){return state.empty() ? v->value : state+"::"+v->value;});

            return root ? "::"+res : res;
        }
    };

    //    typeUse = primitive | list | set | map | ptr | array | scopedName

    struct SScopeEntry
    {
        SScope  *owner{0};
        Name    name;
    };

    //    scope
    struct SScope
        : SScopeEntry
    {
        std::vector<Decl>                   decls;

        std::map<std::string, SAlias *>     aliases;
        std::map<std::string, SStruct *>    structs;
        std::map<std::string, SVariant *>   variants;
        std::map<std::string, SEnum *>      enums;
        std::map<std::string, SIface *>     ifaces;
        std::map<std::string, SScope *>     scopes;

        meta::Scope                         *meta{nullptr};
    };

    //    alias
    struct SAlias
        : SScopeEntry
    {
        TypeUse     type;

        meta::Alias *meta{nullptr};
    };

    //    bases
    struct SBaseStructs
    {
        std::vector<ScopedName> scopedNames;
        std::vector<SStruct*>   instances;
    };

    struct SBaseVariants
    {
        std::vector<ScopedName> scopedNames;
        std::vector<SVariant*>  instances;
    };

    struct SBaseEnums
    {
        std::vector<ScopedName> scopedNames;
        std::vector<SEnum*>     instances;
    };

    struct SBaseIfaces
    {
        std::vector<ScopedName> scopedNames;
        std::vector<SIface*>    instances;
    };

    //    variantField
    struct SVariantField
    {
        SVariant        *owner{0};
        Name            name;
        TypeUse         type;

        meta::Attribute *meta{nullptr};
    };

    //    variant
    struct SVariant
        : SScope
    {
        BaseVariants                bases;
        std::vector<VariantField>   fields;

        meta::Variant               *meta{nullptr};
    };

    //    structField
    struct SStructField
    {
        SStruct         *owner{0};
        Name            name;
        TypeUse         type;

        meta::Attribute *meta{nullptr};
    };

    //    struct_
    struct SStruct
        : SScope
    {
        BaseStructs                 bases;
        std::vector<StructField>    fields;

        meta::Struct                *meta{nullptr};
    };

    //    enumField
    struct SEnumField
    {
        SEnum           *owner{0};
        Name            name;

        meta::EnumValue *meta{nullptr};
    };

    //    enum_
    struct SEnum
        : SScopeEntry
    {
        BaseEnums               bases;
        std::vector<EnumField>  fields;

        meta::Enum              *meta{nullptr};
    };

    //    methodParam
    struct SMethodParam
    {
        SMethod         *owner{0};
        Name            name;
        TypeUse         type;

        meta::Attribute *meta{nullptr};
    };

    //    method
    enum class MethodDirection
    {
        in,
        out
    };

    struct SMethod
    {
        SIface                      *owner{0};

        MethodDirection             direction;
        TypeUse                     resultType;
        bool                        nowait;
        Name                        name;
        std::vector<MethodParam>    params;

        meta::Method                *meta{nullptr};
    };

    //    iface
    struct SIface
        : SScope
    {
        BaseIfaces              bases;
        std::vector<Method>     fields;

        meta::Iface             *meta{nullptr};
    };

    //    include

    //    decl = alias | variant | struct | enum | iface

    //    decls = *(decl | include)

}}}}
