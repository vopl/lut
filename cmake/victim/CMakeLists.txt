set(PROJECT victim)
set(PSRC ${SRCDIR}/victim)
set(PINC ${INCDIR}/victim)

project(${PROJECT})

set(INC
)
SOURCE_GROUP(include FILES ${INC})

set(SRC
    ${PSRC}/main.cpp
)
source_group(src FILES ${SRC})

add_executable(${PROJECT} ${INC} ${SRC} ${SRC_PCH})

target_link_libraries(${PROJECT}
    lut-mm
    lut-async
    lut-io)

#if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG)
#    target_link_libraries(${PROJECT} rt)
#endif()
