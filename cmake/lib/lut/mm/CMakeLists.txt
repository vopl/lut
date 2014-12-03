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
set(INC
    ${PINC}.hpp
    ${PINC}/config.hpp
    ${PINC}/stack.hpp
    ${PINC}/sharedInstance.hpp
)
source_group(include/mm FILES ${INC})
############################################################
set(SRC
    ${PSRC}/vm.cpp
    ${PSRC}/allocator.hpp
    ${PSRC}/vm.hpp
    ${PSRC}/system.hpp
    ${PSRC}/system.cpp
    ${PSRC}/utils/sized_cast.hpp
    ${PSRC}/utils/intrusiveDeque.hpp
    ${PSRC}/utils/intrusiveDeque.ipp
    ${PSRC}/utils/sized_cast.ipp
    ${PSRC}/allocator/block.ipp
    ${PSRC}/allocator/blocksHolder.hpp
    ${PSRC}/allocator/bitIndex.ipp
    ${PSRC}/allocator/bitIndex.hpp
    ${PSRC}/allocator/bitIndex/level.hpp
    ${PSRC}/allocator/bitIndex/level.ipp
    ${PSRC}/allocator/bitIndex/orderEvaluator.hpp
    ${PSRC}/allocator/bitIndex/address.hpp
    ${PSRC}/allocator/block.hpp
    ${PSRC}/allocator/blocksHolder.ipp
    ${PSRC}/allocator/stack.hpp
    ${PSRC}/allocator/stack.cpp
    ${PSRC}/allocator/stackLayout.hpp
    ${PSRC}/allocator/stackState.hpp
    ${PSRC}/allocator.ipp
    ${PSRC}/allocator.cpp
    ${PSRC}.ipp
    ${PSRC}.cpp
)
source_group(src/mm FILES ${SRC})








############################################################
#include_directories(${CMAKE_CURRENT_BINARY_DIR})
#include_directories(${PSRC})

include_directories(${INCDIR})
include_directories(${SRCDIR})













############################################################
add_library(${PROJECT} STATIC
    ${INC}                  ${SRC}

    ${SRC_PCH}
)




