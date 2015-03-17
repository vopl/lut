set(PROJECT lut-couple-generator)
project(${PROJECT})
set(PINC ${INCDIR}/lut/couple/generator)
set(PSRC ${SRCDIR}/lut/couple/generator)




############################################################
set(INC
    ${PINC}/test.hpp
)
source_group(include/couple/generator FILES ${INC})

############################################################
set(SRC
    ${PSRC}/test.cpp
)
source_group(src/couple/generator FILES ${SRC})







############################################################
set(INC_IMPL
    ${PSRC}/impl/test.hpp
)
source_group(src/couple/generator/impl FILES ${INC_IMPL})

############################################################
set(SRC_IMPL
    ${PSRC}/impl/test.cpp
)
source_group(src/couple/generator/impl FILES ${SRC_IMPL})





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
        lut/couple/meta/predecl.hpp

    CLASSES
        ::lut::couple::meta::impl::Alias
        ::lut::couple::meta::impl::Array
        ::lut::couple::meta::impl::Attribute
        ::lut::couple::meta::impl::Compound<::lut::couple::meta::Attribute>
        ::lut::couple::meta::impl::Compound<::lut::couple::meta::EnumValue>
        ::lut::couple::meta::impl::Compound<::lut::couple::meta::Method>
        ::lut::couple::meta::impl::Compound<::lut::couple::meta::Type>
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
        ::lut::couple::meta::impl::Type
        ::lut::couple::meta::impl::Variant
)
]]



############################################################
add_library(${PROJECT} SHARED
    ${INC}                  ${SRC}
    ${IMC_IMPL}             ${SRC_IMPL}

    ${SRC_PCH}
)
