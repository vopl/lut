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
    ${PSRC}/impl/tokenizer.hpp
    ${PSRC}/impl/grammar.hpp
    ${PSRC}/impl/grammarError.hpp
    ${PSRC}/impl/parse.hpp
    ${PSRC}/impl/parseState.hpp

    ${PSRC}/impl/ast.hpp
    ${PSRC}/impl/ast/namesChecker.hpp
    ${PSRC}/impl/ast/ownerIndexer.hpp
    ${PSRC}/impl/ast/scopeMerger.hpp
    ${PSRC}/impl/ast/scopedNamesResolver.hpp
    ${PSRC}/impl/ast/basesChecker.hpp
    ${PSRC}/impl/ast/metaCreator.hpp
    ${PSRC}/impl/ast/metaLinker.hpp
)
source_group(src/coupling/parser/impl FILES ${INC_IMPL})

############################################################
set(SRC_IMPL
    ${PSRC}/impl/common.cpp
    ${PSRC}/impl/tokenizer.cpp

    ${PSRC}/impl/grammar.cpp
    ${PSRC}/impl/grammar/mkPrimitive.cpp
    ${PSRC}/impl/grammar/mkList.cpp
    ${PSRC}/impl/grammar/mkSet.cpp
    ${PSRC}/impl/grammar/mkMap.cpp
    ${PSRC}/impl/grammar/mkPtr.cpp
    ${PSRC}/impl/grammar/mkArray.cpp
    ${PSRC}/impl/grammar/mkName.cpp
    ${PSRC}/impl/grammar/mkScopedName.cpp
    ${PSRC}/impl/grammar/mkTypeUse.cpp
    ${PSRC}/impl/grammar/mkAlias.cpp
    ${PSRC}/impl/grammar/mkBases.cpp
    ${PSRC}/impl/grammar/mkVariantField.cpp
    ${PSRC}/impl/grammar/mkVariant.cpp
    ${PSRC}/impl/grammar/mkStructField.cpp
    ${PSRC}/impl/grammar/mkStruct.cpp
    ${PSRC}/impl/grammar/mkEnumField.cpp
    ${PSRC}/impl/grammar/mkEnum.cpp
    ${PSRC}/impl/grammar/mkMethodParam.cpp
    ${PSRC}/impl/grammar/mkMethod.cpp
    ${PSRC}/impl/grammar/mkIface.cpp
    ${PSRC}/impl/grammar/mkScope.cpp
    ${PSRC}/impl/grammar/mkInclude.cpp
    ${PSRC}/impl/grammar/mkDecl.cpp
    ${PSRC}/impl/grammar/mkDecls.cpp
    ${PSRC}/impl/grammar/mkFile.cpp
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
