set(PROJECT lut-io)
project(${PROJECT})
set(PINC ${INCDIR}/lut/io)
set(PSRC ${SRCDIR}/lut/io)




############################################################
set(INC
    ${PINC}/handle.hpp
    ${PINC}/stream.hpp
    ${PINC}/tcp.hpp
    ${PINC}/udp.hpp
    ${PINC}/pipe.hpp
    ${PINC}/tty.hpp
    ${PINC}/poll.hpp
    ${PINC}/timer.hpp
    ${PINC}/process.hpp
    ${PINC}/fs_event.hpp
    ${PINC}/fs_poll.hpp
    ${PINC}/signal.hpp
)
source_group(include FILES ${INC})
############################################################
set(SRC
    ${PSRC}/handle.cpp
    ${PSRC}/stream.cpp
    ${PSRC}/tcp.cpp
    ${PSRC}/udp.cpp
    ${PSRC}/pipe.cpp
    ${PSRC}/tty.cpp
    ${PSRC}/poll.cpp
    ${PSRC}/timer.cpp
    ${PSRC}/process.cpp
    ${PSRC}/fs_event.cpp
    ${PSRC}/fs_poll.cpp
    ${PSRC}/signal.cpp
)
source_group(src FILES ${SRC})









############################################################
set(INC_IMPL
    ${PSRC}/impl/handle.hpp
    ${PSRC}/impl/stream.hpp
    ${PSRC}/impl/tcp.hpp
    ${PSRC}/impl/udp.hpp
    ${PSRC}/impl/pipe.hpp
    ${PSRC}/impl/tty.hpp
    ${PSRC}/impl/poll.hpp
    ${PSRC}/impl/timer.hpp
    ${PSRC}/impl/process.hpp
    ${PSRC}/impl/fs_event.hpp
    ${PSRC}/impl/fs_poll.hpp
    ${PSRC}/impl/signal.hpp
)
source_group(include/impl FILES ${INC_IMPL})
############################################################
set(SRC_IMPL
    ${PSRC}/impl/handle.cpp
    ${PSRC}/impl/stream.cpp
    ${PSRC}/impl/tcp.cpp
    ${PSRC}/impl/udp.cpp
    ${PSRC}/impl/pipe.cpp
    ${PSRC}/impl/tty.cpp
    ${PSRC}/impl/poll.cpp
    ${PSRC}/impl/timer.cpp
    ${PSRC}/impl/process.cpp
    ${PSRC}/impl/fs_event.cpp
    ${PSRC}/impl/fs_poll.cpp
    ${PSRC}/impl/signal.cpp
)
source_group(src/impl FILES ${SRC_IMPL})







mkHiddenImplSizes(
    OUTPUT
        ${PINC}/sizeofImpl.hpp

    HEADERS
        ${PSRC}/impl/handle.hpp

    CLASSES
        ::lut::io::impl::Handle
)


############################################################
add_library(${PROJECT} STATIC
    ${INC}                  ${SRC}
    ${INC_IMPL}             ${SRC_IMPL}

    ${SRC_PCH}
    ${PINC}/sizeofImpl.hpp)

target_link_libraries(${PROJECT}
    lut-mm lut-async)




