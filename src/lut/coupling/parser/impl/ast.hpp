#pragma once

#include "lut/coupling/parser/impl/common.hpp"
#include "lut/coupling/parser/impl/tokenizer.hpp"
#include <boost/variant.hpp>

namespace lut { namespace coupling { namespace parser { namespace impl
{
    /*
        primitive

        list
        set
        map
        ptr
        array

        name

        typeName

        typeUse = primitive | list | set | map | ptr | array | typeName

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
    enum class Primitive;

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

    //    typeName
    struct STypeName;
    using TypeName = std::shared_ptr<STypeName>;


    //    typeUse = primitive | list | set | map | ptr | array | typeName
    using TypeUse = boost::variant<
          Primitive
        , List
        , Set
        , Map
        , Ptr
        , Array
        , TypeName
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
    enum class Primitive
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

    //    list
    struct SList
    {
        TypeUse elementType;
    };

    //    set
    struct SSet
    {
        TypeUse elementType;
    };

    //    map
    struct SMap
    {
        TypeUse keyType;
        TypeUse valueType;
    };

    //    ptr
    struct SPtr
    {
        TypeUse valueType;
    };

    //    array
    struct SArray
    {
        TypeUse     elementType;
        std::string size;
    };

    //    name
    struct SName
    {
        Token::token_value_type pos;
        std::string             value;
    };

    //    typeName
    struct STypeName
        : SName
    {
    };

    //    typeUse = primitive | list | set | map | ptr | array | typeName

    struct SScopeEntry
    {
        SScope      *owner{0};
        TypeName    name;
    };

    //    scope
    struct SScope
        : SScopeEntry
    {
        std::vector<Decl>       decls;
    };

    //    alias
    struct SAlias
        : SScopeEntry
    {
        TypeUse     type;
    };

    //    bases
    struct SBaseStructs
    {
        std::vector<TypeName>   names;
        std::vector<Struct>     instances;
    };

    struct SBaseVariants
    {
        std::vector<TypeName>   names;
        std::vector<Variant>    instances;
    };

    struct SBaseEnums
    {
        std::vector<TypeName>   names;
        std::vector<Enum>       instances;
    };

    struct SBaseIfaces
    {
        std::vector<TypeName>   names;
        std::vector<Iface>      instances;
    };

    //    variantField
    struct SVariantField
    {
        SVariant    *owner{0};
        Name        name;
        TypeUse     type;
    };

    //    variant
    struct SVariant
        : SScope
    {
        BaseVariants                bases;
        std::vector<VariantField>   fields;
    };

    //    structField
    struct SStructField
    {
        SStruct *owner{0};
        Name    name;
        TypeUse type;
    };

    //    struct_
    struct SStruct
        : SScope
    {
        BaseStructs                 bases;
        std::vector<StructField>    fields;
    };

    //    enumField
    struct SEnumField
    {
        SEnum   *owner{0};
        Name    name;
    };

    //    enum_
    struct SEnum
        : SScopeEntry
    {
        BaseEnums               bases;
        std::vector<EnumField>  fields;
    };

    //    methodParam
    struct SMethodParam
    {
        SMethod *owner{0};
        Name    name;
        TypeUse type;
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
    };

    //    iface
    struct SIface
        : SScope
    {
        BaseIfaces              bases;
        std::vector<Method>     fields;
    };

    //    include

    //    decl = alias | variant | struct | enum | iface

    //    decls = *(decl | include)

}}}}
