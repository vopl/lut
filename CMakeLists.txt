project(dci)
cmake_minimum_required(VERSION 3.0)



###########################################
set(LOCALINSTALL_DIR ${CMAKE_BINARY_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LOCALINSTALL_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LOCALINSTALL_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LOCALINSTALL_DIR}/lib)

macro(localinstall_copy target src dst)
    add_custom_command(
        OUTPUT ${dst}
        DEPENDS ${src}
        COMMAND ${CMAKE_COMMAND} -E copy ${src} ${dst}
    )
    if(EXISTS ${src} AND ${src} IS_NEWER_THAN ${dst})
        configure_file(${src} ${dst} COPYONLY)
    endif()
    target_sources(${target} PUBLIC ${dst})
endmacro()

macro(localinstall target dir)
    if("DROP_PREFIX" STREQUAL ${ARGV2})
        set(argn ${ARGN})
        math(EXPR up ${ARGC}-3)
        foreach(i RANGE 2 ${up})
            list(GET argn ${i} f)
            string(REGEX REPLACE "^${ARGV3}" "" fnp ${f})
            localinstall_copy(${target} ${CMAKE_CURRENT_SOURCE_DIR}/${f} ${LOCALINSTALL_DIR}/${dir}/${fnp})
        endforeach()
    else()
        foreach(f ${ARGN})
            localinstall_copy(${target} ${CMAKE_CURRENT_SOURCE_DIR}/${f} ${LOCALINSTALL_DIR}/${dir}/${f})
        endforeach()
    endif()
endmacro()

set(CMAKE_MODULE_PATH ${LOCALINSTALL_DIR}/share/cmake/Modules ${CMAKE_MODULE_PATH})
include_directories(${LOCALINSTALL_DIR}/include)


###########################################
file(GLOB subs */CMakeLists.txt)
foreach(sub ${subs})
    get_filename_component(sub ${sub} DIRECTORY)
    get_filename_component(sub ${sub} NAME)
    add_subdirectory(${sub})
endforeach()
