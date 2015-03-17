set(PROJECT lut-couple-meta)
project(${PROJECT})
set(PINC ${INCDIR}/lut/couple/meta)
set(PSRC ${SRCDIR}/lut/couple/meta)




############################################################
set(INC
    ${PINC}/alias.hpp
    ${PINC}/array.hpp
    ${PINC}/attribute.hpp
    ${PINC}/compound.hpp
    ${PINC}/container1.hpp
    ${PINC}/container2.hpp
    ${PINC}/enum.hpp
    ${PINC}/enumValue.hpp
    ${PINC}/iface.hpp
    ${PINC}/inheritable.hpp
    ${PINC}/list.hpp
    ${PINC}/map.hpp
    ${PINC}/method.hpp
    ${PINC}/ptr.hpp
    ${PINC}/primitive.hpp
    ${PINC}/scope.hpp
    ${PINC}/set.hpp
    ${PINC}/struct.hpp
    ${PINC}/named.hpp
    ${PINC}/type.hpp
    ${PINC}/variant.hpp

    ${PINC}/sizeProvider.hpp
    ${PINC}/predecl.hpp

    ${PINC}/library.hpp
    ${PINC}/libraryBuilder.hpp
)
source_group(include/couple/meta FILES ${INC})

############################################################
set(SRC
    ${PSRC}/alias.cpp
    ${PSRC}/array.cpp
    ${PSRC}/attribute.cpp
    ${PSRC}/compound.cpp
    ${PSRC}/container1.cpp
    ${PSRC}/container2.cpp
    ${PSRC}/enum.cpp
    ${PSRC}/enumValue.cpp
    ${PSRC}/iface.cpp
    ${PSRC}/inheritable.cpp
    ${PSRC}/list.cpp
    ${PSRC}/map.cpp
    ${PSRC}/method.cpp
    ${PSRC}/ptr.cpp
    ${PSRC}/primitive.cpp
    ${PSRC}/scope.cpp
    ${PSRC}/set.cpp
    ${PSRC}/struct.cpp
    ${PSRC}/named.cpp
    ${PSRC}/type.cpp
    ${PSRC}/variant.cpp

    ${PSRC}/library.cpp
    ${PSRC}/libraryBuilder.cpp
)
source_group(src/couple/meta FILES ${SRC})







############################################################
set(INC_IMPL
    ${PSRC}/impl/alias.hpp
    ${PSRC}/impl/array.hpp
    ${PSRC}/impl/attribute.hpp
    ${PSRC}/impl/compound.hpp
    ${PSRC}/impl/container1.hpp
    ${PSRC}/impl/container2.hpp
    ${PSRC}/impl/enum.hpp
    ${PSRC}/impl/enumValue.hpp
    ${PSRC}/impl/iface.hpp
    ${PSRC}/impl/inheritable.hpp
    ${PSRC}/impl/list.hpp
    ${PSRC}/impl/map.hpp
    ${PSRC}/impl/method.hpp
    ${PSRC}/impl/ptr.hpp
    ${PSRC}/impl/primitive.hpp
    ${PSRC}/impl/scope.hpp
    ${PSRC}/impl/set.hpp
    ${PSRC}/impl/struct.hpp
    ${PSRC}/impl/named.hpp
    ${PSRC}/impl/type.hpp
    ${PSRC}/impl/variant.hpp
    ${PSRC}/impl/library.hpp
    ${PSRC}/impl/libraryBuilder.hpp
    ${PSRC}/impl/libraryContent.hpp
)
source_group(src/couple/meta/impl FILES ${INC_IMPL})

############################################################
set(SRC_IMPL
    ${PSRC}/impl/alias.cpp
    ${PSRC}/impl/array.cpp
    ${PSRC}/impl/attribute.cpp
    ${PSRC}/impl/compound.cpp
    ${PSRC}/impl/container1.cpp
    ${PSRC}/impl/container2.cpp
    ${PSRC}/impl/enum.cpp
    ${PSRC}/impl/enumValue.cpp
    ${PSRC}/impl/iface.cpp
    ${PSRC}/impl/inheritable.cpp
    ${PSRC}/impl/list.cpp
    ${PSRC}/impl/map.cpp
    ${PSRC}/impl/method.cpp
    ${PSRC}/impl/ptr.cpp
    ${PSRC}/impl/primitive.cpp
    ${PSRC}/impl/scope.cpp
    ${PSRC}/impl/set.cpp
    ${PSRC}/impl/struct.cpp
    ${PSRC}/impl/named.cpp
    ${PSRC}/impl/type.cpp
    ${PSRC}/impl/variant.cpp
    ${PSRC}/impl/library.cpp
    ${PSRC}/impl/libraryBuilder.cpp
    ${PSRC}/impl/libraryContent.cpp
)
source_group(src/couple/meta/impl FILES ${SRC_IMPL})





############################################################
include_directories(${INCDIR})
include_directories(${SRCDIR})









mkHimplSizes(
    OUTPUT
        ${PINC}/sizeProvider.hpp

    HEADERS
        ${PSRC}/impl/alias.hpp
        ${PSRC}/impl/array.hpp
        ${PSRC}/impl/attribute.hpp
        ${PSRC}/impl/compound.hpp
        ${PSRC}/impl/container1.hpp
        ${PSRC}/impl/container2.hpp
        ${PSRC}/impl/enum.hpp
        ${PSRC}/impl/enumValue.hpp
        ${PSRC}/impl/iface.hpp
        ${PSRC}/impl/inheritable.hpp
        ${PSRC}/impl/list.hpp
        ${PSRC}/impl/map.hpp
        ${PSRC}/impl/method.hpp
        ${PSRC}/impl/ptr.hpp
        ${PSRC}/impl/primitive.hpp
        ${PSRC}/impl/scope.hpp
        ${PSRC}/impl/set.hpp
        ${PSRC}/impl/struct.hpp
        ${PSRC}/impl/named.hpp
        ${PSRC}/impl/type.hpp
        ${PSRC}/impl/variant.hpp
        ${PSRC}/impl/library.hpp
        ${PSRC}/impl/libraryBuilder.hpp

    PREDECL
        lut/couple/meta/predecl.hpp

    CLASSES
        ::lut::couple::meta::impl::Alias
        ::lut::couple::meta::impl::Array
        ::lut::couple::meta::impl::Attribute
        ::lut::couple::meta::impl::Compound<::lut::couple::meta::Attribute>
        ::lut::couple::meta::impl::Compound<::lut::couple::meta::EnumValue>
        ::lut::couple::meta::impl::Compound<::lut::couple::meta::Method>
        ::lut::couple::meta::impl::Compound<::lut::couple::meta::Type>
        ::lut::couple::meta::impl::Compound<::lut::couple::meta::Scope>
        ::lut::couple::meta::impl::Container1
        ::lut::couple::meta::impl::Container2
        ::lut::couple::meta::impl::Enum
        ::lut::couple::meta::impl::EnumValue
        ::lut::couple::meta::impl::Iface
        ::lut::couple::meta::impl::Inheritable<::lut::couple::meta::Enum>
        ::lut::couple::meta::impl::Inheritable<::lut::couple::meta::Iface>
        ::lut::couple::meta::impl::Inheritable<::lut::couple::meta::Struct>
        ::lut::couple::meta::impl::Inheritable<::lut::couple::meta::Variant>
        ::lut::couple::meta::impl::List
        ::lut::couple::meta::impl::Map
        ::lut::couple::meta::impl::Method
        ::lut::couple::meta::impl::Ptr
        ::lut::couple::meta::impl::Primitive
        ::lut::couple::meta::impl::Scope
        ::lut::couple::meta::impl::Set
        ::lut::couple::meta::impl::Struct
        ::lut::couple::meta::impl::Named
        ::lut::couple::meta::impl::Type
        ::lut::couple::meta::impl::Variant
        ::lut::couple::meta::impl::Library
        ::lut::couple::meta::impl::LibraryBuilder
)




############################################################
add_library(${PROJECT} SHARED
    ${INC}                  ${SRC}
    ${IMC_IMPL}             ${SRC_IMPL}

    ${SRC_PCH}
)




