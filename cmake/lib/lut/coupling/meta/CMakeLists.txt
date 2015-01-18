set(PROJECT lut-coupling-meta)
project(${PROJECT})
set(PINC ${INCDIR}/lut/coupling/meta)
set(PSRC ${SRCDIR}/lut/coupling/meta)




############################################################
set(INC
    ${PINC}/array.hpp
    ${PINC}/enum.hpp
    ${PINC}/iface.hpp
    ${PINC}/integer.hpp
    ${PINC}/list.hpp
    ${PINC}/map.hpp
    ${PINC}/ptr.hpp
    ${PINC}/real.hpp
    ${PINC}/scope.hpp
    ${PINC}/set.hpp
    ${PINC}/string.hpp
    ${PINC}/struct.hpp
    ${PINC}/type.hpp
    ${PINC}/variant.hpp
)
source_group(include/coupling/meta FILES ${INC})

############################################################
set(SRC
    ${PSRC}/array.cpp
    ${PSRC}/enum.cpp
    ${PSRC}/iface.cpp
    ${PSRC}/integer.cpp
    ${PSRC}/list.cpp
    ${PSRC}/map.cpp
    ${PSRC}/ptr.cpp
    ${PSRC}/real.cpp
    ${PSRC}/scope.cpp
    ${PSRC}/set.cpp
    ${PSRC}/string.cpp
    ${PSRC}/struct.cpp
    ${PSRC}/type.cpp
    ${PSRC}/variant.cpp
)
source_group(src/coupling/meta FILES ${SRC})







############################################################
set(INC_IMPL
    ${PSRC}/impl/array.hpp
    ${PSRC}/impl/enum.hpp
    ${PSRC}/impl/iface.hpp
    ${PSRC}/impl/integer.hpp
    ${PSRC}/impl/list.hpp
    ${PSRC}/impl/map.hpp
    ${PSRC}/impl/ptr.hpp
    ${PSRC}/impl/real.hpp
    ${PSRC}/impl/scope.hpp
    ${PSRC}/impl/set.hpp
    ${PSRC}/impl/string.hpp
    ${PSRC}/impl/struct.hpp
    ${PSRC}/impl/type.hpp
    ${PSRC}/impl/variant.hpp
)
source_group(src/coupling/meta/impl FILES ${INC_IMPL})

############################################################
set(SRC_IMPL
    ${PSRC}/impl/array.cpp
    ${PSRC}/impl/enum.cpp
    ${PSRC}/impl/iface.cpp
    ${PSRC}/impl/integer.cpp
    ${PSRC}/impl/list.cpp
    ${PSRC}/impl/map.cpp
    ${PSRC}/impl/ptr.cpp
    ${PSRC}/impl/real.cpp
    ${PSRC}/impl/scope.cpp
    ${PSRC}/impl/set.cpp
    ${PSRC}/impl/string.cpp
    ${PSRC}/impl/struct.cpp
    ${PSRC}/impl/type.cpp
    ${PSRC}/impl/variant.cpp
)
source_group(src/coupling/meta/impl FILES ${SRC_IMPL})





############################################################
#include_directories(${CMAKE_CURRENT_BINARY_DIR})
#include_directories(${PSRC})

include_directories(${INCDIR})
include_directories(${SRCDIR})









mkHiddenImplSizes(
    OUTPUT
        ${PINC}/sizeProvider.hpp

    HEADERS
        ${PSRC}/impl/array.hpp
        ${PSRC}/impl/enum.hpp
        ${PSRC}/impl/iface.hpp
        ${PSRC}/impl/integer.hpp
        ${PSRC}/impl/list.hpp
        ${PSRC}/impl/map.hpp
        ${PSRC}/impl/ptr.hpp
        ${PSRC}/impl/real.hpp
        ${PSRC}/impl/scope.hpp
        ${PSRC}/impl/set.hpp
        ${PSRC}/impl/string.hpp
        ${PSRC}/impl/struct.hpp
        ${PSRC}/impl/type.hpp
        ${PSRC}/impl/variant.hpp


    CLASSES
        ::lut::coupling::meta::impl::Array
        ::lut::coupling::meta::impl::Enum
        ::lut::coupling::meta::impl::Iface
        ::lut::coupling::meta::impl::Integer
        ::lut::coupling::meta::impl::List
        ::lut::coupling::meta::impl::Map
        ::lut::coupling::meta::impl::Ptr
        ::lut::coupling::meta::impl::Real
        ::lut::coupling::meta::impl::Scope
        ::lut::coupling::meta::impl::Set
        ::lut::coupling::meta::impl::String
        ::lut::coupling::meta::impl::Struct
        ::lut::coupling::meta::impl::Type
        ::lut::coupling::meta::impl::Variant
)




############################################################
add_library(${PROJECT} STATIC
    ${INC}                  ${SRC}
    ${IMC_IMPL}             ${SRC_IMPL}

    ${SRC_PCH}
    ${PINC}/sizeProvider.hpp
)




