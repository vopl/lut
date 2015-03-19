set(PROJECT dci-io)
project(${PROJECT})




############################################################
set(INC
    include/loop.hpp
    include/stream.hpp
    include/server.hpp
    include/client.hpp
    include/endpoint.hpp
    include/poller.hpp
    include/timer.hpp
    include/data.hpp
    include/error.hpp
    include/sizeProvider.hpp
)

############################################################
set(SRC
    src/loop.cpp
    src/stream.cpp
    src/server.cpp
    src/client.cpp
    src/endpoint.cpp
    src/poller.cpp
    src/timer.cpp
    src/data.cpp
    src/error.cpp

    src/impl/loop.hpp
    src/impl/stream.hpp
    src/impl/server.hpp
    src/impl/client.hpp
    src/impl/poller.hpp
    src/impl/timer.hpp
    src/impl/fd/base.hpp
    src/impl/fd/stream.hpp
    src/impl/fd/stream/reader.hpp
    src/impl/fd/stream/writer.hpp
    src/impl/fd/connector.hpp
    src/impl/data.hpp
    src/impl/data/segment.hpp
    src/impl/utils/sockaddr.hpp

    src/impl/loop.cpp
    src/impl/stream.cpp
    src/impl/server.cpp
    src/impl/client.cpp
    src/impl/poller.cpp
    src/impl/timer.cpp
    src/impl/fd/base.cpp
    src/impl/fd/stream.cpp
    src/impl/fd/stream/reader.cpp
    src/impl/fd/stream/writer.cpp
    src/impl/fd/connector.cpp
    src/impl/data.cpp
    src/impl/data/segment.cpp
    src/impl/utils/sockaddr.cpp
)




############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)




include(DciHimpl)
mkHimplSizes(${PROJECT}
    OUTPUT
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sizeProvider.hpp

    HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/stream.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/client.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/server.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/data.hpp

    CLASSES
        ::dci::io::impl::Stream
        ::dci::io::impl::Client
        ::dci::io::impl::Server
        ::dci::io::impl::Data
)


############################################################
add_library(${PROJECT} SHARED
    ${INC}                  ${SRC}
)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT}
    dci-mm
    dci-async
    dci-logger
    dci-himpl)

if(COMMAND localinstall)
    localinstall(${PROJECT} include/dci/io DROP_PREFIX include/ ${INC})
endif()
