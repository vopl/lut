set(PROJECT dci-async)
project(${PROJECT} CXX)
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)





############################################################
include(CheckIncludeFiles)










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
find_package(Boost COMPONENTS context system)

if(Boost_CONTEXT_FOUND)
    set(HAVE_BOOSTCONTEXT ON)
endif()

############################################################
include(FindValgrind.cmake)
if(VALGRIND_FOUND)
    set(HAVE_VALGRIND TRUE)
    #message(STATUS "found valgrind: ${VALGRIND_PROGRAM}, ${VALGRIND_INCLUDE_DIR}")
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
configure_file(config.h.in config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})










if(VALGRIND_FOUND)
    include_directories(${VALGRIND_INCLUDE_DIR})
endif()

















############################################################
set(INC
    include/functions.hpp
    include/mutex.hpp
    include/event.hpp
    include/acquire.hpp
    include/future.hpp
    include/details/task.hpp
    include/details/taskInstance.hpp
    include/details/acquire.hpp
    include/sizeProvider.hpp
)

############################################################
set(SRC
    src/functions.cpp
    src/mutex.cpp
    src/event.cpp
    src/details/task.cpp
    src/details/acquire.cpp
    
    src/impl/scheduler.hpp
    src/impl/scheduler.cpp
    src/impl/task.hpp
    src/impl/task.cpp
    src/impl/effortContainer.hpp

    src/impl/syncronizerWaiter.hpp
    src/impl/syncronizer.hpp
    src/impl/mutex.hpp
    src/impl/event.hpp

    src/impl/syncronizerWaiter.cpp
    src/impl/syncronizer.cpp
    src/impl/mutex.cpp
    src/impl/event.cpp
)







############################################################
set(INC_CTX
)

############################################################
set(SRC_CTX
    src/impl/ctx/root.hpp
    src/impl/ctx/root.cpp
    src/impl/ctx/coro.hpp
    src/impl/ctx/coro.cpp
)
if(ASYNCSCHEDULER_CONTEXTENGINE_WINFIBER)
    list(APPEND SRC_CTX src/impl/ctx/engine_winfiber.hpp)
    list(APPEND SRC_CTX src/impl/ctx/engine_winfiber.cpp)
endif()
if(ASYNCSCHEDULER_CONTEXTENGINE_UCONTEXT)
    list(APPEND SRC_CTX src/impl/ctx/engine_ucontext.hpp)
    list(APPEND SRC_CTX src/impl/ctx/engine_ucontext.cpp)
endif()
if(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT)
    list(APPEND SRC_CTX src/impl/ctx/engine_boostcontext.hpp)
    list(APPEND SRC_CTX src/impl/ctx/engine_boostcontext.cpp)
endif()



















############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


if(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT)
    include_directories(${Boost_INCLUDE_DIRS})
endif()








include(DciHimpl)
mkHimplSizes(${PROJECT}
    OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sizeProvider.hpp

    HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/scheduler.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/task.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/mutex.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/event.hpp

    CLASSES
        ::dci::async::impl::Scheduler
        ::dci::async::impl::Task
        ::dci::async::impl::Mutex
        ::dci::async::impl::Event
)



############################################################
add_library(${PROJECT} SHARED
    ${INC}             ${SRC}
    ${INC_CTX}         ${SRC_CTX}
)

target_link_libraries(${PROJECT}
    dci-himpl
    dci-mm
    dci-logger)

set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)

if(ASYNCSCHEDULER_CONTEXTENGINE_BOOSTCONTEXT)
    target_link_libraries(${PROJECT} ${Boost_CONTEXT_LIBRARY})
endif()

target_link_libraries(${PROJECT} ${Boost_SYSTEM_LIBRARY})

if(COMMAND localinstall)
    localinstall(${PROJECT} include/dci/async DROP_PREFIX include/ ${INC})
endif()




