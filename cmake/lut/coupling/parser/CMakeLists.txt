set(PROJECT lut-coupling-parser)
project(${PROJECT})
set(PINC ${INCDIR}/lut/coupling/parser)
set(PSRC ${SRCDIR}/lut/coupling/parser)


############################################################
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS filesystem system)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "boost filesystem system not found")
endif()

include_directories(${Boost_INCLUDE_DIRS})



############################################################
set(INC
    ${PINC}/exec.hpp
    ${PINC}/config.hpp
    ${PINC}/errorInfo.hpp

)
source_group(include/coupling/parser FILES ${INC})

############################################################
set(SRC
    ${PSRC}/exec.cpp
    ${PSRC}/config.cpp
    ${PSRC}/errorInfo.cpp
    ${PSRC}/pch.hpp
)
source_group(src/coupling/parser FILES ${SRC})

set(SRC_PCH
    ${PSRC}/pch.cpp
)
source_group(src/coupling/parser FILES ${SRC_PCH})






############################################################
set(INC_IMPL
    ${PSRC}/impl/posIterator.hpp
    ${PSRC}/impl/common.hpp
    ${PSRC}/impl/ast.hpp
    ${PSRC}/impl/tokenizer.hpp
    ${PSRC}/impl/grammar.hpp
    ${PSRC}/impl/grammarError.hpp
    ${PSRC}/impl/parse.hpp
    ${PSRC}/impl/parseState.hpp
)
source_group(src/coupling/parser/impl FILES ${INC_IMPL})

############################################################
set(SRC_IMPL
    ${PSRC}/impl/common.cpp
    ${PSRC}/impl/tokenizer.cpp
    ${PSRC}/impl/grammar.cpp
    ${PSRC}/impl/grammarMkPrimitive.cpp
    ${PSRC}/impl/grammarMkList.cpp
    ${PSRC}/impl/grammarMkSet.cpp
    ${PSRC}/impl/grammarMkMap.cpp
    ${PSRC}/impl/grammarMkPtr.cpp
    ${PSRC}/impl/grammarMkArray.cpp
    ${PSRC}/impl/grammarMkName.cpp
    ${PSRC}/impl/grammarMkScopedName.cpp
    ${PSRC}/impl/grammarMkTypeUse.cpp
    ${PSRC}/impl/grammarMkAlias.cpp
    ${PSRC}/impl/grammarMkBases.cpp
    ${PSRC}/impl/grammarMkVariantField.cpp
    ${PSRC}/impl/grammarMkVariant.cpp
    ${PSRC}/impl/grammarMkStructField.cpp
    ${PSRC}/impl/grammarMkStruct.cpp
    ${PSRC}/impl/grammarMkEnumField.cpp
    ${PSRC}/impl/grammarMkEnum.cpp
    ${PSRC}/impl/grammarMkMethodParam.cpp
    ${PSRC}/impl/grammarMkMethod.cpp
    ${PSRC}/impl/grammarMkIface.cpp
    ${PSRC}/impl/grammarMkScope.cpp
    ${PSRC}/impl/grammarMkInclude.cpp
    ${PSRC}/impl/grammarMkDecl.cpp
    ${PSRC}/impl/grammarMkDecls.cpp
    ${PSRC}/impl/grammarMkFile.cpp
    ${PSRC}/impl/grammarError.cpp
    ${PSRC}/impl/parse.cpp
)
source_group(src/coupling/parser/impl FILES ${SRC_IMPL})





############################################################
include_directories(${INCDIR})
include_directories(${SRCDIR})



############################################################
add_library(${PROJECT} STATIC
    ${INC}                  ${SRC}
    ${INC_IMPL}             ${SRC_IMPL}

    ${SRC_PCH}
)

target_link_libraries(${PROJECT} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})

create_pch(${PROJECT} ${PSRC}/pch.hpp ${PSRC}/pch.cpp)
use_pch(${PROJECT} ${PSRC}/pch.hpp ${SRC} ${SRC_IMPL})
