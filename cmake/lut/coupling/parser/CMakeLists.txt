set(PROJECT lut-coupling-parser)
project(${PROJECT})
set(PINC ${INCDIR}/lut/coupling/parser)
set(PSRC ${SRCDIR}/lut/coupling/parser)




############################################################
set(INC
    ${PINC}/exec.hpp
    ${PINC}/config.hpp
    ${PINC}/errorInfo.hpp

)
source_group(include/coupling/parser FILES ${INC})

############################################################
set(SRC
    ${PSRC}/exec.cpp
    ${PSRC}/config.cpp
    ${PSRC}/errorInfo.cpp
)
source_group(src/coupling/parser FILES ${SRC})







############################################################
set(INC_IMPL
    ${PSRC}/impl/tokens.hpp
)
source_group(src/coupling/parser/impl FILES ${INC_IMPL})

############################################################
set(SRC_IMPL
)
source_group(src/coupling/parser/impl FILES ${SRC_IMPL})





############################################################
include_directories(${INCDIR})
include_directories(${SRCDIR})








#[[
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
)
]]



############################################################
add_library(${PROJECT} STATIC
    ${INC}                  ${SRC}
    ${INC_IMPL}             ${SRC_IMPL}

    ${SRC_PCH}
)
