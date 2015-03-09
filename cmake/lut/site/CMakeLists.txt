set(PROJECT lut-site)
set(PSRC ${SRCDIR}/lut/site)
set(PINC ${INCDIR}/lut/site)

project(${PROJECT})

set(INC
    ${PINC}/control.hpp
    ${PINC}/interact.hpp
)
SOURCE_GROUP(include FILES ${INC})

set(SRC
    ${PSRC}/main.cpp
    ${PSRC}/control.cpp
    ${PSRC}/interact.cpp
)
source_group(src FILES ${SRC})



############################################################
set(INC_IMPL
    ${PSRC}/impl/instance.hpp
)
source_group(include/impl FILES ${INC_IMPL})
############################################################
set(SRC_IMPL
    ${PSRC}/impl/instance.cpp
)
source_group(src/impl FILES ${SRC_IMPL})




set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS program_options)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "boost program_options not found")
endif()

include_directories(${Boost_INCLUDE_DIRS})


add_executable(${PROJECT} ${INC} ${SRC} ${INC_IMPL} ${SRC_IMPL} ${SRC_PCH})

target_link_libraries(${PROJECT} ${Boost_PROGRAM_OPTIONS_LIBRARY})

target_link_libraries(${PROJECT}
    lut-mm
    lut-async
    lut-io)
