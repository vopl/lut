set(PROJECT dci-couple-runtime)
project(${PROJECT})


############################################################
set(INC
    include/identifier.hpp
    include/intrusivePtr.hpp
    include/iface.hpp
    include/hub.hpp
    include/sizeProvider.hpp
)


############################################################
set(SRC
    src/iface.cpp
    src/hub.cpp

    src/impl/iface.hpp
    src/impl/hub.hpp

    src/impl/iface.cpp
    src/impl/hub.cpp
)






############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)



############################################################
include(DciHimpl)
mkHimplSizes(${PROJECT}
    OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sizeProvider.hpp

    HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/hub.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/iface.hpp

    CLASSES
        ::dci::couple::runtime::impl::Hub
        ::dci::couple::runtime::impl::Iface
)



############################################################
add_library(${PROJECT} SHARED
    ${INC}                  ${SRC}
)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT}
    dci-mm
    dci-async
    dci-logger
    dci-himpl)

if(COMMAND localinstall)
    localinstall(${PROJECT} include/dci/couple/runtime DROP_PREFIX include/ ${INC})
endif()
