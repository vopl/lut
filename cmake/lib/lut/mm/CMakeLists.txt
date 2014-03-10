set(PROJECT lut-mm)
project(${PROJECT})
set(PINC ${INCDIR}/lut/mm)
set(PSRC ${SRCDIR}/lut/mm)




############################################################
include(FindValgrind)
if(VALGRIND_FOUND)
    set(HAVE_VALGRIND TRUE)
    #message(STATUS "found valgrind: ${VALGRIND_PROGRAM}, ${VALGRIND_INCLUDE_DIR}")
    include_directories(${VALGRIND_INCLUDE_DIR})
endif()








############################################################
set(INC_IMPL
)
source_group(include/mm/impl FILES ${INC_IMPL_MM})
############################################################
set(SRC_IMPL
    ${PSRC}/impl/config.hpp
    ${PSRC}/impl/config.cpp
    ${PSRC}/impl/allocator.hpp
    ${PSRC}/impl/allocator.cpp
    ${PSRC}/impl/thread.hpp
    ${PSRC}/impl/thread.cpp
    ${PSRC}/impl/stacksContainer.hpp
    ${PSRC}/impl/buffersContainer.hpp
    ${PSRC}/impl/stack.hpp
    ${PSRC}/impl/stackInstance.hpp
    ${PSRC}/impl/vm.hpp
    ${PSRC}/impl/vm.cpp
    ${PSRC}/impl/bitIndex.hpp
)
source_group(src/mm/impl FILES ${SRC_IMPL_MM})






############################################################
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PSRC})












############################################################
add_library(${PROJECT} STATIC
    ${INC}                  ${SRC}
    ${INC_IMPL}             ${SRC_IMPL}

    ${SRC_PCH}
)




