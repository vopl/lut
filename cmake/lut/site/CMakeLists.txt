set(PROJECT lut-site)
set(PSRC ${SRCDIR}/lut/site)
set(PINC ${INCDIR}/lut/site)

project(${PROJECT})

set(INC
    ${PINC}/error.hpp
    ${PINC}/moduleId.hpp
    ${PINC}/modulePlace.hpp
)
SOURCE_GROUP(include FILES ${INC})

set(SRC
    ${PSRC}/main.cpp
    ${PSRC}/error.cpp
    ${PSRC}/modulePlace.cpp
)
source_group(src FILES ${SRC})



############################################################
set(INC_IMPL
    ${PSRC}/impl/instance.hpp
    ${PSRC}/impl/module.hpp
    ${PSRC}/impl/modulePlace.hpp
    ${PSRC}/impl/moduleState.hpp
)
source_group(include/impl FILES ${INC_IMPL})
############################################################
set(SRC_IMPL
    ${PSRC}/impl/instance.cpp
    ${PSRC}/impl/module.cpp
    ${PSRC}/impl/modulePlace.cpp
)
source_group(src/impl FILES ${SRC_IMPL})


mkHimplSizes(
    OUTPUT
        ${PINC}/sizeProvider.hpp

    HEADERS
        ${PSRC}/impl/modulePlace.hpp

    CLASSES
        ::lut::site::impl::ModulePlace
)



set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS program_options filesystem)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "boost program_options not found")
endif()

include_directories(${Boost_INCLUDE_DIRS})


add_executable(${PROJECT}
    ${INC}                  ${SRC}
    ${IMC_IMPL}             ${SRC_IMPL}

    ${PINC}/sizeProvider.hpp

    ${SRC_PCH}
)


target_link_libraries(${PROJECT} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

target_link_libraries(${PROJECT}
    lut-mm
    lut-async
    lut-io
    lut-module)
