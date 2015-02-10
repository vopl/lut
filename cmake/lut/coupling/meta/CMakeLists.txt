set(PROJECT lut-coupling-meta)
project(${PROJECT})
set(PINC ${INCDIR}/lut/coupling/meta)
set(PSRC ${SRCDIR}/lut/coupling/meta)




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
    ${PINC}/type.hpp
    ${PINC}/variant.hpp

    ${PINC}/sizeProvider.hpp
    ${PINC}/predecl.hpp

    ${PINC}/library.hpp
    ${PINC}/libraryBuilder.hpp
)
source_group(include/coupling/meta FILES ${INC})

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
    ${PSRC}/type.cpp
    ${PSRC}/variant.cpp

    ${PSRC}/library.cpp
    ${PSRC}/libraryBuilder.cpp
)
source_group(src/coupling/meta FILES ${SRC})







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
    ${PSRC}/impl/type.hpp
    ${PSRC}/impl/variant.hpp
    ${PSRC}/impl/library.hpp
    ${PSRC}/impl/libraryBuilder.hpp
)
source_group(src/coupling/meta/impl FILES ${INC_IMPL})

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
    ${PSRC}/impl/type.cpp
    ${PSRC}/impl/variant.cpp
    ${PSRC}/impl/library.cpp
    ${PSRC}/impl/libraryBuilder.cpp
)
source_group(src/coupling/meta/impl FILES ${SRC_IMPL})





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
        ${PSRC}/impl/type.hpp
        ${PSRC}/impl/variant.hpp
        ${PSRC}/impl/library.hpp
        ${PSRC}/impl/libraryBuilder.hpp

    PREDECL
        lut/coupling/meta/predecl.hpp

    CLASSES
        ::lut::coupling::meta::impl::Alias
        ::lut::coupling::meta::impl::Array
        ::lut::coupling::meta::impl::Attribute
        ::lut::coupling::meta::impl::Compound<::lut::coupling::meta::Attribute>
        ::lut::coupling::meta::impl::Compound<::lut::coupling::meta::EnumValue>
        ::lut::coupling::meta::impl::Compound<::lut::coupling::meta::Method>
        ::lut::coupling::meta::impl::Compound<::lut::coupling::meta::Type>
        ::lut::coupling::meta::impl::Container1
        ::lut::coupling::meta::impl::Container2
        ::lut::coupling::meta::impl::Enum
        ::lut::coupling::meta::impl::EnumValue
        ::lut::coupling::meta::impl::Iface
        ::lut::coupling::meta::impl::Inheritable<::lut::coupling::meta::Enum>
        ::lut::coupling::meta::impl::Inheritable<::lut::coupling::meta::Iface>
        ::lut::coupling::meta::impl::Inheritable<::lut::coupling::meta::Struct>
        ::lut::coupling::meta::impl::Inheritable<::lut::coupling::meta::Variant>
        ::lut::coupling::meta::impl::List
        ::lut::coupling::meta::impl::Map
        ::lut::coupling::meta::impl::Method
        ::lut::coupling::meta::impl::Ptr
        ::lut::coupling::meta::impl::Primitive
        ::lut::coupling::meta::impl::Scope
        ::lut::coupling::meta::impl::Set
        ::lut::coupling::meta::impl::Struct
        ::lut::coupling::meta::impl::Type
        ::lut::coupling::meta::impl::Variant
        ::lut::coupling::meta::impl::Library
        ::lut::coupling::meta::impl::LibraryBuilder
)




############################################################
add_library(${PROJECT} STATIC
    ${INC}                  ${SRC}
    ${IMC_IMPL}             ${SRC_IMPL}

    ${SRC_PCH}
)




