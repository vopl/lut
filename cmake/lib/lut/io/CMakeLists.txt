set(PROJECT lut-io)
project(${PROJECT})
set(PINC ${INCDIR}/lut/io)
set(PSRC ${SRCDIR}/lut/io)




############################################################
set(INC
    ${PINC}/loop.hpp
    ${PINC}/stream.hpp
    ${PINC}/server.hpp
    ${PINC}/client.hpp
    ${PINC}/endpoint.hpp
    ${PINC}/poller.hpp
    ${PINC}/timer.hpp
    ${PINC}/data.hpp
)
source_group(include FILES ${INC})
############################################################
set(SRC
    ${PSRC}/loop.cpp
    ${PSRC}/stream.cpp
    ${PSRC}/server.cpp
    ${PSRC}/client.cpp
    ${PSRC}/endpoint.cpp
    ${PSRC}/poller.cpp
    ${PSRC}/timer.cpp
    ${PSRC}/data.cpp
)
source_group(src FILES ${SRC})









############################################################
set(INC_IMPL
    ${PSRC}/impl/loop.hpp
    ${PSRC}/impl/stream.hpp
    ${PSRC}/impl/server.hpp
    ${PSRC}/impl/client.hpp
    ${PSRC}/impl/poller.hpp
    ${PSRC}/impl/timer.hpp
    ${PSRC}/impl/fd/base.hpp
    ${PSRC}/impl/fd/stream.hpp
    ${PSRC}/impl/fd/stream/reader.hpp
    ${PSRC}/impl/fd/stream/writer.hpp
    ${PSRC}/impl/fd/tcpConnector.hpp
    ${PSRC}/impl/data.hpp
)
source_group(include/impl FILES ${INC_IMPL})
############################################################
set(SRC_IMPL
    ${PSRC}/impl/loop.cpp
    ${PSRC}/impl/stream.cpp
    ${PSRC}/impl/server.cpp
    ${PSRC}/impl/client.cpp
    ${PSRC}/impl/poller.cpp
    ${PSRC}/impl/timer.cpp
    ${PSRC}/impl/fd/base.cpp
    ${PSRC}/impl/fd/stream.cpp
    ${PSRC}/impl/fd/stream/reader.cpp
    ${PSRC}/impl/fd/stream/writer.cpp
    ${PSRC}/impl/fd/tcpConnector.cpp
    ${PSRC}/impl/data.cpp
)
source_group(src/impl FILES ${SRC_IMPL})







mkHiddenImplSizes(
    OUTPUT
        ${PINC}/sizeofImpl.hpp

    HEADERS
        ${PSRC}/impl/stream.hpp
        ${PSRC}/impl/client.hpp
        ${PSRC}/impl/server.hpp
        ${PSRC}/impl/data.hpp

    CLASSES
        ::lut::io::impl::Stream
        ::lut::io::impl::Client
        ::lut::io::impl::Server
        ::lut::io::impl::Data
)


############################################################
add_library(${PROJECT} STATIC
    ${INC}                  ${SRC}
    ${INC_IMPL}             ${SRC_IMPL}

    ${SRC_PCH}
    ${PINC}/sizeofImpl.hpp)

target_link_libraries(${PROJECT}
    lut-mm lut-async)




