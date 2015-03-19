set(PROJECT dci-logger)
project(${PROJECT})


############################################################
set(INC
    include/logger.hpp
)

set(SRC
    src/logger.cpp
)

add_library(${PROJECT} SHARED
    ${INC} ${SRC}
)


if(COMMAND localinstall)
    localinstall(${PROJECT} include/dci/logger DROP_PREFIX include/ ${INC})
endif()


