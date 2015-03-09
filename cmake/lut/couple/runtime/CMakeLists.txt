set(PROJECT lut-couple-runtime)
project(${PROJECT})
set(PINC ${INCDIR}/lut/couple/runtime)
set(PSRC ${SRCDIR}/lut/couple/runtime)


############################################################
set(INC
    ${PINC}/iface.hpp
    ${PINC}/hub.hpp

)
source_group(include/couple/runtime FILES ${INC})

############################################################
set(SRC
    ${PSRC}/iface.cpp
    ${PSRC}/hub.cpp
)
source_group(src/couple/iface FILES ${SRC})

set(SRC_PCH
    ${PSRC}/pch.hpp
    ${PSRC}/pch.cpp
)
source_group(src/couple/runtime FILES ${SRC_PCH})






############################################################
set(INC_IMPL
    ${PSRC}/impl/iface.hpp
    ${PSRC}/impl/hub.hpp
)
source_group(src/couple/runtime/impl FILES ${INC_IMPL})

############################################################
set(SRC_IMPL
    ${PSRC}/impl/iface.cpp
    ${PSRC}/impl/hub.cpp
)
source_group(src/couple/runtime/impl FILES ${SRC_IMPL})





############################################################
include_directories(${INCDIR})
include_directories(${SRCDIR})


############################################################
mkHimplSizes(
    OUTPUT
        ${PINC}/sizeProvider.hpp

    HEADERS
        ${PSRC}/impl/hub.hpp
        ${PSRC}/impl/iface.hpp

    CLASSES
        ::lut::couple::runtime::impl::Hub
        ::lut::couple::runtime::impl::Iface
)



############################################################
add_library(${PROJECT} STATIC
    ${INC}                  ${SRC}
    ${INC_IMPL}             ${SRC_IMPL}

    ${SRC_PCH}
    ${PINC}/sizeProvider.hpp
)

create_pch(${PROJECT} ${PSRC}/pch.hpp ${PSRC}/pch.cpp)
use_pch(${PROJECT} ${PSRC}/pch.hpp ${SRC} ${SRC_IMPL})
