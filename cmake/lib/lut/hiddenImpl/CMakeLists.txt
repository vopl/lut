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
    set(srcText "${CMAKE_CURRENT_BINARY_DIR}/sizeofImplGen.text")

    file(WRITE ${srcText} "#define GENERATE_SIZEOFIMPL 1\n")
    foreach(header ${headers})
        file(APPEND ${srcText} "#include \"${header}\"\n")
    endforeach()

    file(APPEND ${srcText} "#include <iostream>\n")

    file(APPEND ${srcText} "int main(){\n")
    file(APPEND ${srcText} "    std::cout<<\n")
    file(APPEND ${srcText} "        \"#ifndef ${guard}\\n\"\n")
    file(APPEND ${srcText} "        \"#define ${guard}\\n\"\n")
    file(APPEND ${srcText} "        \"\\n\"\n")
    file(APPEND ${srcText} "        \"#include \\\"lut/hiddenImpl/sizeofImpl.hpp\\\"\\n\"\n")
    file(APPEND ${srcText} "        \"\\n\"\n")

    ############################################################
    foreach(class ${classes})

        string(REPLACE "::" ";" namespaces ${class})
        list(REMOVE_ITEM namespaces "")
        list(REVERSE namespaces)
        list(GET namespaces 0 classWithoutNamespace)
        list(REMOVE_AT namespaces 0)
        list(REVERSE namespaces)

        file(APPEND ${srcText} "        \"")
        foreach(namespace ${namespaces})
            file(APPEND ${srcText} "namespace ${namespace} {")
        endforeach()

        file(APPEND ${srcText} "class ${classWithoutNamespace};")

        foreach(namespace ${namespaces})
            file(APPEND ${srcText} "}")
        endforeach()
        file(APPEND ${srcText} "\\n\"\n")
    endforeach()

    ############################################################
    file(APPEND ${srcText} "        \"\\n\"\n")
    file(APPEND ${srcText} "        \"namespace lut { namespace hiddenImpl {\\n\"\n")

    foreach(class ${classes})
        file(APPEND ${srcText} "        \"    template <> struct sizeofImpl<${class}> { static const std::size_t _value =  \" << sizeof(${class}) << \";};\\n\"\n")
    endforeach()

    file(APPEND ${srcText} "        \"}}\\n\"\n")

    file(APPEND ${srcText} "        \"\\n\"\n")
    file(APPEND ${srcText} "        \"#endif\\n\"\n")
    file(APPEND ${srcText} ";\n")
    file(APPEND ${srcText} "    return 0;\n")
    file(APPEND ${srcText} "}\n")




    ############################################################
    set(src "${CMAKE_CURRENT_BINARY_DIR}/sizeofImplGen.cpp")
    add_custom_command(OUTPUT ${src}
                        COMMAND ${CMAKE_COMMAND} -E copy ${srcText}  ${src}
                        DEPENDS ${headers} ${PINC}.hpp
                        )

    ############################################################
    set(sizeofImplGenTarget sizeofImplGen${guard})
    add_executable(${sizeofImplGenTarget} EXCLUDE_FROM_ALL ${src})
    set_target_properties(${sizeofImplGenTarget} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set_target_properties(${sizeofImplGenTarget} PROPERTIES RUNTIME_OUTPUT_NAME sizeofImplGen)
    get_target_property(sizeofImplGenExecutable ${sizeofImplGenTarget} LOCATION)

    add_custom_command(OUTPUT ${output}
                        COMMAND ${sizeofImplGenExecutable} > ${output}
                        DEPENDS ${sizeofImplGenTarget}
                        )

endmacro()

add_library(${PROJECT} STATIC
    ${PINC}.hpp
    ${PINC}/sizeofImpl.hpp)

set_target_properties(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)
