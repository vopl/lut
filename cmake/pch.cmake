
#######################################################################################
MACRO(TO_NATIVE_PATH PATH OUT)
	FILE(TO_NATIVE_PATH "${PATH}" "${OUT}")
	IF(MINGW)
		STRING(REPLACE "/" "\\" "${OUT}" "${${OUT}}")
	ENDIF(MINGW)
ENDMACRO(TO_NATIVE_PATH)

#######################################################################################
IF(MSVC)
	MESSAGE(STATUS "pch: MSVC")
	
	MACRO(PCH_KEY2FILENAME pchfile key header)
		GET_FILENAME_COMPONENT(filename ${header} NAME_WE)
		SET(pchfile "$(IntDir)/${filename}.pch")
		GET_FILENAME_COMPONENT(key ${header} NAME)
	ENDMACRO(PCH_KEY2FILENAME)


	MACRO(CREATE_PCH target header srcfile)
		PCH_KEY2FILENAME(pchfile key ${header})

		SET_SOURCE_FILES_PROPERTIES(${srcfile} PROPERTIES COMPILE_FLAGS "/Yc${key} /Fp${pchfile}")
#		MESSAGE("precompiled ${key} created by ${srcfile}")
	ENDMACRO(CREATE_PCH)

	MACRO(USE_PCH target header)
		PCH_KEY2FILENAME(pchfile key ${header})
	
		FOREACH(i ${ARGN})
			SET_SOURCE_FILES_PROPERTIES(${i} PROPERTIES COMPILE_FLAGS "/Yu${key} /Fp${pchfile}")
#			MESSAGE("precompiled ${key} used in ${i}")
		ENDFOREACH(i)
	ENDMACRO(USE_PCH)
ELSEIF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG)
	MESSAGE(STATUS "pch: GCC")

	MACRO(PCH_KEY2FILENAME pchfile header)
		GET_FILENAME_COMPONENT(filename ${header} NAME)

		#GET_FILENAME_COMPONENT(path ${header} PATH)
		SET(path ${CMAKE_CURRENT_BINARY_DIR})
		
		SET(pchfile "${path}/${filename}.gch")
	ENDMACRO(PCH_KEY2FILENAME)






	MACRO(CREATE_PCH target header srcfile)
	
		PCH_KEY2FILENAME(pchfile ${header})
		GET_FILENAME_COMPONENT(pchfile_path ${pchfile} PATH)
		
		GET_DIRECTORY_PROPERTY(INCS INCLUDE_DIRECTORIES)
		SET(IINCS)
		FOREACH(i ${INCS})
			LIST(APPEND IINCS "-I" ${i})
		ENDFOREACH(i)



		GET_DIRECTORY_PROPERTY(DEFS COMPILE_DEFINITIONS)
		STRING(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_UC)
		GET_DIRECTORY_PROPERTY(DEFSB COMPILE_DEFINITIONS_${BUILD_TYPE_UC})

		SET(IDEFS)
		FOREACH(i ${DEFS})
			LIST(APPEND IDEFS "-D" ${i})
		ENDFOREACH(i)
		FOREACH(i ${DEFSB})
			LIST(APPEND IDEFS "-D" ${i})
		ENDFOREACH(i)
		
		#SET(IDEFS ${DEFS} ${DEFSB})
		#MESSAGE(${IDEFS})


		TO_NATIVE_PATH("${CMAKE_CXX_COMPILER}" cxx_compiler)
		#FILE(TO_NATIVE_PATH "${CMAKE_CXX_COMPILER}" cxx_compiler)
		#MESSAGE("${cxx_compiler}")

		SET(cxx_args "${CMAKE_CXX_FLAGS}")
		SEPARATE_ARGUMENTS(cxx_args)

		SET(cxx_args_bt ${CMAKE_CXX_FLAGS_${BUILD_TYPE_UC}})
		SEPARATE_ARGUMENTS(cxx_args_bt)

		SET(cxx_args ${IDEFS} ${cxx_args} ${cxx_args_bt} ${IINCS} ${header} -o ${pchfile})
		#SEPARATE_ARGUMENTS(cxx_args)

		SET(target_pch ${target}_build_pch)

		ADD_CUSTOM_COMMAND(OUTPUT ${pchfile} 
			COMMAND ${cxx_compiler} ${cxx_args}
			DEPENDS ${header})

		#SET(CMAKE_CXX_FLAGS_${BUILD_TYPE_UC} "-I${pchfile_path} ${CMAKE_CXX_FLAGS_${BUILD_TYPE_UC}}")

		#MESSAGE("precompiled ${pchfile} created")
	ENDMACRO(CREATE_PCH)

	MACRO(USE_PCH target header)
		PCH_KEY2FILENAME(pchfile ${header})
		GET_FILENAME_COMPONENT(header_name ${header} NAME)

		FOREACH(i ${ARGN})
			SET_SOURCE_FILES_PROPERTIES(${i} PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR} )
			SET_SOURCE_FILES_PROPERTIES(${i} PROPERTIES COMPILE_FLAGS "-Winvalid-pch -include ${header_name}" )
			SET_SOURCE_FILES_PROPERTIES(${i} PROPERTIES OBJECT_DEPENDS ${pchfile} )
			#MESSAGE("precompiled ${pchfile} used in ${i}")
		ENDFOREACH(i)
	ENDMACRO(USE_PCH)
	
	
	
	
	
	
ELSE()
	MESSAGE(STATUS "pch: NO USE")
	MACRO(CREATE_PCH target header srcfile)
	ENDMACRO(CREATE_PCH)

	MACRO(USE_PCH target header)
	ENDMACRO(USE_PCH)
ENDIF()
