set(PROJECT lut-async)
project(${PROJECT})
set(PINC ${INCDIR}/lut/async)
set(PSRC ${SRCDIR}/lut/async)




############################################################
include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckIncludeFiles)
include(CheckTypeSize)

############################################################
check_type_size("void *" PVOID_SIZE)
check_type_size("int" INT_SIZE)

############################################################
check_include_files(ucontext.h HAVE_UCONTEXT)

############################################################
if(WIN32)
	include(CheckPrototypeDefinition)
	check_prototype_definition(
		CreateFiberEx
		"LPVOID WINAPI CreateFiberEx(SIZE_T dwStackCommitSize, SIZE_T dwStackReserveSize, DWORD dwFlags, LPFIBER_START_ROUTINE lpStartAddress, LPVOID lpParameter);"
		"NULL"
		windows.h
		HAVE_WINFIBER)
endif()

############################################################
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS context)

if(Boost_CONTEXT_FOUND)
    set(HAVE_BOOSTCONTEXT ON)
endif()

############################################################
include(FindValgrind)
if(VALGRIND_FOUND)
    set(HAVE_VALGRIND TRUE)
    #message(STATUS "found valgrind: ${VALGRIND_PROGRAM}, ${VALGRIND_INCLUDE_DIR}")
    include_directories(${VALGRIND_INCLUDE_DIR})
endif()








############################################################
if(NOT ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER AND NOT ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT AND NOT ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT)
    if(HAVE_UCONTEXT)
        set(ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT ON)
    elseif(HAVE_WINFIBER)
        set(ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER ON)
    elseif(HAVE_BOOSTCONTEXT)
        set(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT ON)
    else()
        message(FATAL_ERROR "no support for context engine available")
    endif()
endif()



set(CONTEXT_ENGINE "")

if(ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER AND HAVE_WINFIBER)
    set(CONTEXT_ENGINE "WINFIBER")
elseif(ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT AND HAVE_UCONTEXT)
    set(CONTEXT_ENGINE "UCONTEXT")
elseif(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT AND HAVE_BOOSTCONTEXT)
    set(CONTEXT_ENGINE "BOOSTCONTEXT")
else()
    message(FATAL_ERROR "context engine has no support")
endif()

message(STATUS "use ${CONTEXT_ENGINE} as context engine")
if(CONTEXT_ENGINE STREQUAL "WINFIBER")
    set(ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER ON)
    set(ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT OFF)
    set(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT OFF)
elseif(CONTEXT_ENGINE STREQUAL "UCONTEXT")
    set(ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER OFF)
    set(ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT ON)
    set(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT OFF)
elseif(CONTEXT_ENGINE STREQUAL "BOOSTCONTEXT")
    set(ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER OFF)
    set(ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT OFF)
    set(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT ON)
else()
    message(FATAL_ERROR "unknown context engine2")
endif()


set(ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER ${ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER} CACHE BOOL "use windows fibers as context engine" FORCE)
set(ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT ${ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT} CACHE BOOL "use posix ucontext as context engine" FORCE)
set(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT ${ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT} CACHE BOOL "use boost::context as context engine" FORCE)

############################################################
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${PINC}/config.h)








############################################################
set(INC
    ${PINC}/threadState.hpp
    ${PINC}/threadPool.hpp
    ${PINC}/scheduler.hpp
    ${PINC}/hiddenImpl.hpp
    ${PINC}/conditionVariable.hpp
    ${PINC}/event.hpp
    ${PINC}/mutex.hpp
    ${PINC}/recursiveMutex.hpp
    ${PINC}/semaphore.hpp
    ${PINC}/singleEvent.hpp
    ${PINC}/wait.hpp
)
source_group(include FILES ${INC})

############################################################
set(INC_IMPL
    ${PSRC}/impl/scheduler.hpp
    ${PSRC}/impl/thread.hpp
    ${PSRC}/impl/coro.hpp
    ${PSRC}/impl/context.hpp
    ${PSRC}/impl/event.hpp
    ${PSRC}/impl/mutex.hpp
#    ${PSRC}/impl/waiter.hpp
    ${PSRC}/impl/synchronizer.hpp
#    ${PSRC}/impl/synchronizerWaiterNode.hpp
    ${PSRC}/impl/intrusiveQueue.hpp
)

if(ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER)
    list(APPEND INC_IMPL ${PSRC}/impl/contextEngine_winfiber.hpp)
endif()
if(ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT)
    list(APPEND INC_IMPL ${PSRC}/impl/contextEngine_ucontext.hpp)
endif()
if(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT)
    list(APPEND INC_IMPL ${PSRC}/impl/contextEngine_boostcontext.hpp)
endif()

source_group(include/impl FILES ${INC_IMPL})

############################################################
set(SRC
    ${PSRC}/stable.hpp
    ${PSRC}/threadPool.cpp
    ${PSRC}/threadState.cpp
    ${PSRC}/scheduler.cpp
)
source_group(src FILES ${SRC})

############################################################
set(SRC_IMPL
    ${PSRC}/impl/scheduler.cpp
    ${PSRC}/impl/barrier.cpp
    ${PSRC}/impl/context.cpp
    ${PSRC}/impl/coro.cpp
    ${PSRC}/impl/synchronizer.cpp
    ${PSRC}/impl/thread.cpp
    ${PSRC}/impl/sizeofImplGen.cpp
)

if(ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER)
    list(APPEND SRC_IMPL ${PSRC}/impl/contextEngine_winfiber.cpp)
endif()
if(ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT)
    list(APPEND SRC_IMPL ${PSRC}/impl/contextEngine_ucontext.cpp)
endif()
if(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT)
    list(APPEND SRC_IMPL ${PSRC}/impl/contextEngine_boostcontext.cpp)
endif()

source_group(src/impl FILES ${SRC_IMPL})

set_source_files_properties(${PSRC}/impl/sizeofImplGen.cpp PROPERTIES HEADER_FILE_ONLY ON)



############################################################
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PSRC})

if(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT)
    include_directories(${Boost_INCLUDE_DIRS})
endif()











############################################################
string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_UC)

get_directory_property(INCS INCLUDE_DIRECTORIES)
set(IINCS)
foreach(i ${INCS})
    list(APPEND IINCS "-I" ${i})
endforeach(i)

get_directory_property(DEFS COMPILE_DEFINITIONS)
get_directory_property(DEFSB COMPILE_DEFINITIONS_${BUILD_TYPE_UC})
set(IDEFS)
foreach(i ${DEFS} ${DEFSB})
    LIST(APPEND IDEFS "-D" ${i})
endforeach(i)

set(cxx_args ${IDEFS} ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${BUILD_TYPE_UC}} ${IINCS} ${PSRC}/impl/sizeofImplGen.cpp -o sizeofImplGen)
separate_arguments(cxx_args)

add_custom_command(OUTPUT ${PINC}/sizeofImpl.hpp
                    COMMAND ${CMAKE_CXX_COMPILER} ${cxx_args}
                    COMMAND ./sizeofImplGen > ${PINC}/sizeofImpl.hpp
                    DEPENDS
                        ${PSRC}/impl/sizeofImplGen.cpp
                        ${PSRC}/impl/scheduler.hpp
                        ${PSRC}/impl/event.hpp
                        ${PSRC}/impl/mutex.hpp
                    VERBATIM)
                    
#set_source_files_properties(
#    ${PINC}/scheduler.hpp
#    ${PINC}/codeManager.hpp
#    ${PINC}/event.hpp
#    ${PINC}/mutex.hpp
#    ${PINC}/wait.hpp
#    PROPERTIES OBJECT_DEPENDS ${PINC}/sizeofImpl.hpp )




############################################################
add_library(${PROJECT} STATIC
    ${INC} ${INC_IMPL}
    ${SRC} ${SRC_IMPL}
    ${SRC_PCH}
    ${PINC}/sizeofImpl.hpp)

if(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT)
    target_link_libraries(${PROJECT} ${Boost_CONTEXT_LIBRARY})
endif()






