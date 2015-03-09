set(PROJECT lut-couple-translator)
project(${PROJECT})
set(PINC ${INCDIR}/lut/couple/translator)
set(PSRC ${SRCDIR}/lut/couple/translator)




############################################################
set(SRC
    ${PSRC}/main.cpp
)
source_group(src/couple/translator FILES ${SRC})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS program_options)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "boost program_options not found")
endif()

include_directories(${Boost_INCLUDE_DIRS})



############################################################
add_executable(${PROJECT}
    ${INC}                  ${SRC}
    ${IMC_IMPL}             ${SRC_IMPL}

    ${SRC_PCH}
)

target_link_libraries(${PROJECT} ${Boost_PROGRAM_OPTIONS_LIBRARY})

target_link_libraries(${PROJECT}
    lut-couple-parser
    lut-couple-meta
)
