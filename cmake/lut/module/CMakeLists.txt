set(PROJECT lut-module)
project(${PROJECT})
set(PINC ${INCDIR}/lut/module)
set(PSRC ${SRCDIR}/lut/module)




############################################################
set(INC
    ${PINC}/error.hpp
    ${PINC}/controller.hpp
    ${PINC}/place.hpp
)
source_group(include/module FILES ${INC})

############################################################
set(SRC
    ${PSRC}/error.cpp
    ${PSRC}/controller.cpp
    ${PSRC}/place.cpp
)
source_group(src/module FILES ${SRC})







############################################################
set(INC_IMPL
    ${PSRC}/impl/controller.hpp
    ${PSRC}/impl/place.hpp
)
source_group(src/module/impl FILES ${INC_IMPL})

############################################################
set(SRC_IMPL
    ${PSRC}/impl/controller.cpp
    ${PSRC}/impl/place.cpp
)
source_group(src/module/impl FILES ${SRC_IMPL})





############################################################
include_directories(${INCDIR})
include_directories(${SRCDIR})









mkHimplSizes(
    OUTPUT
        ${PINC}/sizeProvider.hpp

    HEADERS
        ${PSRC}/impl/controller.hpp
        ${PSRC}/impl/place.hpp

    CLASSES
        ::lut::module::impl::Controller
        ::lut::module::impl::Place
)




############################################################
add_library(${PROJECT} STATIC
    ${INC}                  ${SRC}
    ${IMC_IMPL}             ${SRC_IMPL}

    ${PINC}/sizeProvider.hpp

    ${SRC_PCH}
)




target_link_libraries(${PROJECT}
    lut-couple-runtime
    lut-async
    dl
)
