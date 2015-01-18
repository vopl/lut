set(PROJECT lut-hiddenImpl)
project(${PROJECT})
set(PINC ${INCDIR}/lut/hiddenImpl)
set(PSRC ${SRCDIR}/lut/hiddenImpl)



macro(mkHiddenImplSizes)

    ############################################################
    set(output)
    set(headers)
    set(classes)

    set(_state "null")

    foreach(arg ${ARGN})
        if(${arg} STREQUAL "OUTPUT" OR ${arg} STREQUAL "HEADERS" OR ${arg} STREQUAL "CLASSES")
            set(_state ${arg})
        else()
            if(${_state} STREQUAL "OUTPUT")
                list(APPEND output ${arg})
            elseif(${_state} STREQUAL "HEADERS")
                list(APPEND headers ${arg})
            elseif(${_state} STREQUAL "CLASSES")
                list(APPEND classes ${arg})
            else()
                message(WARNING "unknown arg kind for mkHiddenImplSizes: ${arg}")
            endif()
        endif()
    endforeach()

    #message("output: ${output}, headers: ${headers}, classes: ${classes}")

    ############################################################
    set(srcText "${CMAKE_CURRENT_BINARY_DIR}/sizeProviderGen.text")

    file(WRITE ${srcText} "#define GENERATE_SIZEPROVIDER 1\n")
    file(APPEND ${srcText} "#include \"lut/stable.hpp\"\n")
    file(APPEND ${srcText} "#include \"lut/hiddenImpl/sizeProviderDefault.hpp\"\n")
    foreach(header ${headers})
        file(APPEND ${srcText} "#include \"${header}\"\n")
    endforeach()

    file(APPEND ${srcText} "#include <iostream>\n")

    file(APPEND ${srcText} "int main(){\n")
    file(APPEND ${srcText} "    std::cout<<\n")
    file(APPEND ${srcText} "        \"#pragma once\\n\"\n")
    file(APPEND ${srcText} "        \"\\n\"\n")
    file(APPEND ${srcText} "        \"#include \\\"lut/hiddenImpl/sizeProvider.hpp\\\"\\n\"\n")
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
        file(APPEND ${srcText} "        \"    template <> struct sizeProvider<${class}> { static const std::size_t _value =  \" << sizeof(${class}) << \";};\\n\"\n")
    endforeach()

    file(APPEND ${srcText} "        \"}}\\n\"\n")

    file(APPEND ${srcText} "        \"\\n\"\n")
    file(APPEND ${srcText} ";\n")
    file(APPEND ${srcText} "    return 0;\n")
    file(APPEND ${srcText} "}\n")




    ############################################################
    set(src "${CMAKE_CURRENT_BINARY_DIR}/sizeProviderGen.cpp")
    add_custom_command(OUTPUT ${src}
                        COMMAND ${CMAKE_COMMAND} -E copy ${srcText}  ${src}
                        DEPENDS ${headers} ${srcText} #${PINC}.hpp
                        )

    ############################################################
    set(sizeProviderGenTarget ${CMAKE_CURRENT_BINARY_DIR}/sizeProviderGen)
#    add_executable(${sizeProviderGenTarget} EXCLUDE_FROM_ALL ${src})
#    set_target_properties(${sizeProviderGenTarget} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
#    set_target_properties(${sizeProviderGenTarget} PROPERTIES RUNTIME_OUTPUT_NAME sizeProviderGen)
#    get_target_property(sizeProviderGenExecutable ${sizeProviderGenTarget} LOCATION)

    set(cxx_args)

    get_directory_property(incs INCLUDE_DIRECTORIES)
    foreach(i ${incs})
            list(APPEND cxx_args "-I" ${i})
    endforeach(i)

    get_directory_property(defs COMPILE_DEFINITIONS)
    string(TOUPPER "${CMAKE_BUILD_TYPE}" build_type_uc)
    get_directory_property(defsb COMPILE_DEFINITIONS_${build_type_uc})
    foreach(i ${defs} ${defsb})
            list(APPEND cxx_args "-D" ${i})
    endforeach(i)

    set(cxx_args ${cxx_args} ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${build_type_uc}})
    separate_arguments(cxx_args)

    add_custom_command(OUTPUT ${sizeProviderGenTarget}
            COMMAND ${CMAKE_CXX_COMPILER} ${cxx_args} ${src} -o ${sizeProviderGenTarget}
            DEPENDS ${src} )

    ############################################################
    add_custom_command(OUTPUT ${output}
                        COMMAND ${sizeProviderGenTarget} > ${output}
                        DEPENDS ${sizeProviderGenTarget}
                        )

endmacro()

add_library(${PROJECT} STATIC
    ${PINC}/single.hpp
    ${PINC}/compound.hpp
    ${PINC}/sizeProvider.hpp
    ${PINC}/sizeProviderDefault.hpp
    ${PSRC}/accessor.hpp)

set_target_properties(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)
