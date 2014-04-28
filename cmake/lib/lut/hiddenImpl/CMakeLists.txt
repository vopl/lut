set(PROJECT lut-hiddenImpl)
project(${PROJECT})
set(PINC ${INCDIR}/lut/hiddenImpl)
set(PSRC ${SRCDIR}/lut/hiddenImpl)



macro(mkHiddenImplSizes)

    ############################################################
    set(guard)
    set(output)
    set(headers)
    set(classes)

    set(_state "null")

    foreach(arg ${ARGN})
        if(${arg} STREQUAL "OUTPUT" OR ${arg} STREQUAL "GUARD" OR ${arg} STREQUAL "HEADERS" OR ${arg} STREQUAL "CLASSES")
            set(_state ${arg})
        else()
            if(${_state} STREQUAL "OUTPUT")
                list(APPEND output ${arg})
            elseif(${_state} STREQUAL "GUARD")
                list(APPEND guard ${arg})
            elseif(${_state} STREQUAL "HEADERS")
                list(APPEND headers ${arg})
            elseif(${_state} STREQUAL "CLASSES")
                list(APPEND classes ${arg})
            else()
                message(WARNING "unknown arg kind for mkHiddenImplSizes: ${arg}")
            endif()
        endif()
    endforeach()

    #message("output: ${output}, guard: ${guard}, headers: ${headers}, classes: ${classes}")

    ############################################################
    set(src "${CMAKE_CURRENT_BINARY_DIR}/sizeofImplGen.cpp")
    file(WRITE ${src} "#define GENERATE_SIZEOFIMPL 1\n")
    foreach(header ${headers})
        file(APPEND ${src} "#include \"${header}\"\n")
    endforeach()

    file(APPEND ${src} "#include <iostream>\n")

    file(APPEND ${src} "int main(){\n")
    file(APPEND ${src} "    std::cout<<\n")
    file(APPEND ${src} "        \"#ifndef ${guard}\\n\"\n")
    file(APPEND ${src} "        \"#define ${guard}\\n\"\n")
    file(APPEND ${src} "        \"\\n\"\n")
    file(APPEND ${src} "        \"#include \\\"lut/hiddenImpl/sizeofImpl.hpp\\\"\\n\"\n")
    file(APPEND ${src} "        \"\\n\"\n")

    ############################################################
    foreach(class ${classes})

        string(REPLACE "::" ";" namespaces ${class})
        list(REMOVE_ITEM namespaces "")
        list(REVERSE namespaces)
        list(GET namespaces 0 classWithoutNamespace)
        list(REMOVE_AT namespaces 0)
        list(REVERSE namespaces)

        file(APPEND ${src} "        \"")
        foreach(namespace ${namespaces})
            file(APPEND ${src} "namespace ${namespace} {")
        endforeach()

        file(APPEND ${src} "class ${classWithoutNamespace};")

        foreach(namespace ${namespaces})
            file(APPEND ${src} "}")
        endforeach()
        file(APPEND ${src} "\\n\"\n")
    endforeach()

    ############################################################
    file(APPEND ${src} "        \"\\n\"\n")
    file(APPEND ${src} "        \"namespace lut { namespace hiddenImpl {\\n\"\n")

    foreach(class ${classes})
        file(APPEND ${src} "        \"    template <> struct sizeofImpl<${class}> { static const std::size_t _value =  \" << sizeof(${class}) << \";};\\n\"\n")
    endforeach()

    file(APPEND ${src} "        \"}}\\n\"\n")

    file(APPEND ${src} "        \"\\n\"\n")
    file(APPEND ${src} "        \"#endif\\n\"\n")
    file(APPEND ${src} ";\n")
    file(APPEND ${src} "    return 0;\n")
    file(APPEND ${src} "}\n")




    ############################################################
    set(sizeofImplGenTarget sizeofImplGen${guard})
    add_executable(${sizeofImplGenTarget} EXCLUDE_FROM_ALL ${src} ${PINC}.hpp ${headers})
    set_target_properties(${sizeofImplGenTarget} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set_target_properties(${sizeofImplGenTarget} PROPERTIES RUNTIME_OUTPUT_NAME sizeofImplGen)
    get_target_property(sizeofImplGenExecutable ${sizeofImplGenTarget} LOCATION)

    add_custom_command(OUTPUT ${output}
                        COMMAND ${sizeofImplGenExecutable} > ${output}
                        DEPENDS ${headers} ${sizeofImplGenTarget}
                        VERBATIM)

endmacro()

add_library(${PROJECT} STATIC
    ${PINC}.hpp
    ${PINC}/sizeofImpl.hpp)

set_target_properties(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)
