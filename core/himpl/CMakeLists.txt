set(PROJECT dci-himpl)
project(${PROJECT})


############################################################
set(INC
    include/faceLayout.hpp
    include/implLayout.hpp
    include/layoutChecker.hpp
    include/sizeProvider.hpp
    include/sizeProviderDefault.hpp
    include/sizeFetcher.hpp
    include/face2Impl.hpp
    include/impl2Face.hpp
)

add_library(${PROJECT} STATIC
    ${INC}
)
set_target_properties(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)




if(COMMAND localinstall)
    localinstall(${PROJECT} include/dci/himpl DROP_PREFIX include/ ${INC})
    localinstall(${PROJECT} share/cmake/Modules DciHimpl.cmake)
endif()


