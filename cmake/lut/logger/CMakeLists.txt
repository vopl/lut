set(PROJECT lut-logger)
project(${PROJECT})
set(PINC ${INCDIR}/lut/logger)
set(PSRC ${SRCDIR}/lut/logger)



add_library(${PROJECT} SHARED
    ${PINC}/logger.hpp
    ${PSRC}/logger.cpp
)

#set_target_properties(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)
