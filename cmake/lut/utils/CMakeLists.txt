set(PROJECT lut-utils)
set(PINC ${INCDIR}/lut/utils)
set(PSRC ${SRCDIR}/lut/utils)

project(${PROJECT})

set(INC
	${PINC}/options.hpp
	${PINC}/logInitializer.hpp
	${PINC}/logTemplate.hpp
)
source_group(include FILES ${INC})

set(SRC
	${PSRC}/options.cpp
	${PSRC}/logInitializer.cpp
)
source_group(src FILES ${SRC})

set(SRC_HDR
)
source_group(src FILES ${SRC_HDR})


set(SRC_PCH
)
source_group(src FILES ${SRC_PCH})

add_library(${PROJECT} STATIC ${INC} ${SRC} ${SRC_HDR} ${SRC_PCH})
