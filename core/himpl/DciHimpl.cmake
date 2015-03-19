
macro(mkHimplSizes target)

    ############################################################
    set(output)
    set(headers)
    set(classes)
    set(predecl)

    set(_state "null")

    foreach(arg ${ARGN})
        if(arg STREQUAL "OUTPUT" OR arg STREQUAL "HEADERS" OR arg STREQUAL "CLASSES" OR arg STREQUAL "PREDECL")
            set(_state ${arg})
        else()
            if(_state STREQUAL "OUTPUT")
                list(APPEND output ${arg})
            elseif(_state STREQUAL "HEADERS")
                list(APPEND headers ${arg})
            elseif(_state STREQUAL "CLASSES")
                list(APPEND classes ${arg})
            elseif(_state STREQUAL "PREDECL")
                list(APPEND predecl ${arg})
            else()
                message(WARNING "unknown arg kind for mkHimplSizes: kind=${arg}, state=${_state}")
            endif()
        endif()
    endforeach()

    #message("output: ${output}, headers: ${headers}, classes: ${classes}")


    ############################################################
    set(src "${CMAKE_CURRENT_BINARY_DIR}/sizeProviderGen.cpp")

    file(WRITE ${src} "#define GENERATE_SIZEPROVIDER 1\n")
    file(APPEND ${src} "#include <dci/himpl/sizeProviderDefault.hpp>\n")
    file(APPEND ${src} "#include <dci/himpl/sizeFetcher.hpp>\n")

    if(predecl)
        file(APPEND ${src} "#include \"${predecl}\"\n")
    endif()

    foreach(header ${headers})
        file(APPEND ${src} "#include \"${header}\"\n")
    endforeach()

    file(APPEND ${src} "#include <iostream>\n")

    file(APPEND ${src} "int main(){\n")
    file(APPEND ${src} "    std::cout<<\n")
    file(APPEND ${src} "        \"#pragma once\\n\"\n")
    file(APPEND ${src} "        \"\\n\"\n")
    file(APPEND ${src} "        \"#include <dci/himpl/sizeProvider.hpp>\\n\"\n")
    file(APPEND ${src} "        \"\\n\"\n")

    ############################################################
    if(predecl)
        file(APPEND ${src} "        \"#include \\\"${predecl}\\\"\\n\"\n")
    else()
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
    endif()

    ############################################################
    file(APPEND ${src} "        \"\\n\"\n")
    file(APPEND ${src} "        \"namespace dci { namespace himpl {\\n\"\n")

    foreach(class ${classes})
        file(APPEND ${src} "        \"    template <> struct sizeProvider<${class}> { static const std::size_t _value =  \" << ::dci::himpl::SizeFetcher<${class}>::_value << \";};\\n\"\n")
    endforeach()

    file(APPEND ${src} "        \"}}\\n\"\n")

    file(APPEND ${src} "        \"\\n\"\n")
    file(APPEND ${src} ";\n")
    file(APPEND ${src} "    return 0;\n")
    file(APPEND ${src} "}\n")


    ############################################################
    set(sizeProviderGen ${target}-sizeProviderGen)
    add_executable(${sizeProviderGen} EXCLUDE_FROM_ALL ${src})
    set_target_properties(${sizeProviderGen} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set_target_properties(${sizeProviderGen} PROPERTIES CXX_STANDARD 14)
    set_target_properties(${sizeProviderGen} PROPERTIES CXX_STANDARD_REQUIRED ON)


    ############################################################
    add_custom_command(OUTPUT ${output}
                        COMMAND ${sizeProviderGen} > ${output}
                        DEPENDS ${sizeProviderGen}
                        )

endmacro()
