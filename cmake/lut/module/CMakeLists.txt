set(PROJECT lut-module)
project(${PROJECT})
set(PINC ${INCDIR}/lut/module)
set(PSRC ${SRCDIR}/lut/module)




############################################################
set(INC
    ${PINC}/error.hpp
)
source_group(include/module FILES ${INC})

############################################################
set(SRC
    ${PSRC}/error.cpp
)
source_group(src/module FILES ${SRC})







############################################################
set(INC_IMPL
)
source_group(src/module/impl FILES ${INC_IMPL})

############################################################
set(SRC_IMPL
)
source_group(src/module/impl FILES ${SRC_IMPL})





############################################################
include_directories(${INCDIR})
include_directories(${SRCDIR})











############################################################
add_library(${PROJECT} SHARED
    ${INC}                  ${SRC}
    ${IMC_IMPL}             ${SRC_IMPL}

    ${SRC_PCH}
)




target_link_libraries(${PROJECT}
    lut-couple-runtime
    lut-async
    dl
)
