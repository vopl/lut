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
    ${PINC}/config.hpp
    ${PINC}/stack.hpp
    ${PINC}/allocator.hpp
)
source_group(include/mm FILES ${INC})
############################################################
set(SRC
    ${PSRC}/allocator.cpp
)
source_group(src/mm FILES ${SRC})


############################################################
set(INC_IMPL
)
source_group(include/mm/impl FILES ${INC_IMPL})
############################################################
set(SRC_IMPL
    ${PSRC}/impl/allocator.hpp
    ${PSRC}/impl/allocator.cpp
    ${PSRC}/impl/indexedContainer.hpp
    ${PSRC}/impl/stackState.hpp
    ${PSRC}/impl/stackLayout.hpp
    ${PSRC}/impl/stack.hpp
    ${PSRC}/impl/stack.cpp
    ${PSRC}/impl/buffer.hpp
    ${PSRC}/impl/buffer.cpp
    ${PSRC}/impl/sizedBuffer.hpp
    ${PSRC}/impl/sizedBufferCalculator.hpp
    ${PSRC}/impl/vm.hpp
    ${PSRC}/impl/vm.cpp
    ${PSRC}/impl/bitIndex.hpp
    ${PSRC}/impl/bitIndexLevel.hpp
    ${PSRC}/impl/branchHint.hpp
)
source_group(src/mm/impl FILES ${SRC_IMPL})






############################################################
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PSRC})












############################################################
add_library(${PROJECT} STATIC
    ${INC}                  ${SRC}
    ${INC_IMPL}             ${SRC_IMPL}

    ${SRC_PCH}
)




