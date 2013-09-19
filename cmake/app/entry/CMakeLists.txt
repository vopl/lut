set(PROJECT entry)
set(PSRC ${SRCDIR}/entry)
set(PINC ${INCDIR}/entry)

project(entry)

set(INC
)
SOURCE_GROUP(include FILES ${INC})

set(SRC
	${PSRC}/main.cpp
)
source_group(src FILES ${SRC})

include_directories(${PSRC})

add_executable(entry ${INC} ${SRC} ${SRC_PCH})

target_link_libraries(entry 
    lut-async)

#if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG)
#    target_link_libraries(${PROJECT} rt)
#endif()
