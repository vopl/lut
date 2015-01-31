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

        decl = alias | variant | struct | enum | iface | scope | include



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
    struct SBases;
    using Bases = std::shared_ptr<SBases>;

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
    struct SInclude;
    using Include = std::shared_ptr<SInclude>;

    //    decl = alias | variant | struct | enum | iface | scope | include
    using Decl = boost::variant<
          Alias
        , Variant
        , Struct
        , Enum
        , Iface
        , Scope
        , Include
    >;



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

    //    typeName
    struct STypeName
    {
        std::string name;
    };

    //    typeUse = primitive | list | set | map | ptr | array | typeName

    //    alias
    struct SAlias
    {
        TypeName    name;
        TypeUse     type;
    };

    //    bases
    struct SBases
    {
        std::vector<TypeName> names;
    };

    //    variantField
    struct SVariantField
    {
        TypeName    name;
        TypeUse     type;
    };

    //    variant
    struct SVariant
    {
        TypeName                    name;
        Bases                       bases;
        std::vector<Decl>           decls;
        std::vector<VariantField>   fields;
    };

    //    structField
    struct SStructField
    {
        TypeName    name;
        TypeUse     type;
    };

    //    struct_
    struct SStruct
    {
        TypeName                    name;
        Bases                       bases;
        std::vector<Decl>           decls;
        std::vector<StructField>    fields;
    };

    //    enumField
    struct SEnumField
    {
        TypeName name;
    };

    //    enum_
    struct SEnum
    {
        TypeName                name;
        Bases                   bases;
        std::vector<EnumField>  fields;
    };

    //    methodParam
    struct SMethodParam
    {
        TypeName    name;
        TypeUse     type;
    };

    //    method
    enum class MethodDirection
    {
        in,
        out
    };

    struct SMethod
    {
        MethodDirection             direction;
        TypeUse                     resultType;
        bool                        nowait;
        TypeName                    name;
        std::vector<MethodParam>    params;
    };

    //    iface
    struct SIface
    {
        TypeName                name;
        Bases                   bases;
        std::vector<Decl>       decls;
        std::vector<Method>     methods;
    };

    //    scope
    struct SScope
    {
        TypeName                name;
        std::vector<Decl>       decls;
    };

    //    include
    struct SInclude
    {
        std::string             target;
    };

    //    decl = alias | variant | struct | enum | iface | include

}}}}
