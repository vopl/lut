set(PROJECT lut-coupling-compiler)
project(${PROJECT})
set(PINC ${INCDIR}/lut/coupling/compiler)
set(PSRC ${SRCDIR}/lut/coupling/compiler)




############################################################
set(SRC
    ${PSRC}/main.cpp
)
source_group(src/coupling/compiler FILES ${SRC})




############################################################
add_executable(${PROJECT}
    ${INC}                  ${SRC}
    ${IMC_IMPL}             ${SRC_IMPL}

    ${SRC_PCH}
)
