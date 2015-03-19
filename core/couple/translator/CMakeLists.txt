set(PROJECT dci-couple-translator)
project(${PROJECT})




############################################################
set(SRC
    src/main.cpp
)


set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS program_options)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "boost program_options not found")
endif()

include_directories(${Boost_INCLUDE_DIRS})



############################################################
add_executable(${PROJECT}
    ${INC}                  ${SRC}
)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT} ${Boost_PROGRAM_OPTIONS_LIBRARY})

target_link_libraries(${PROJECT}
    dci-couple-parser
    dci-couple-meta
)
