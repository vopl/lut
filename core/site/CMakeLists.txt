set(PROJECT dci-site)

project(${PROJECT})

set(INC
    include/error.hpp
    include/moduleId.hpp
    include/modulePlace.hpp
    include/sizeProvider.hpp
)

set(SRC
    src/main.cpp
    src/error.cpp
    src/modulePlace.cpp

    src/impl/instance.hpp
    src/impl/module.hpp
    src/impl/modulePlace.hpp
    src/impl/moduleState.hpp

    src/impl/instance.cpp
    src/impl/module.cpp
    src/impl/modulePlace.cpp
)

############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


include(DciHimpl)
mkHimplSizes(${PROJECT}
    OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sizeProvider.hpp

    HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/modulePlace.hpp

    CLASSES
        ::dci::site::impl::ModulePlace
)



find_package(Boost COMPONENTS program_options filesystem)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "boost program_options not found")
endif()

include_directories(${Boost_INCLUDE_DIRS})


add_executable(${PROJECT}
    ${INC}  ${SRC}
)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)


target_link_libraries(${PROJECT} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

target_link_libraries(${PROJECT}
    dci-mm
    dci-async
    dci-io
    dl
)

if(COMMAND localinstall)
    localinstall(${PROJECT} include/dci/site DROP_PREFIX include/ ${INC})
endif()
