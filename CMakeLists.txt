file(GLOB HEADER_FILES *.hpp)
file(GLOB EXT_SRC_FILES [^e][^x]*.cpp)
file(GLOB EXE_SRC_FILES ex[^\#]*.cpp)
#file(GLOB_RECURSE SHADER_FILES *.glsl)
list(REMOVE_ITEM EXT_SRC_FILES ${EXE_SRC_FILES})

foreach(EXE_SRC_FILE ${EXE_SRC_FILES})
	get_filename_component(FILE ${EXE_SRC_FILE} NAME_WE)
	get_filename_component(DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
	set(OUTPUT ${DIR}_${FILE})
	#    add_executable(${OUTPUT} ${SRC_FILE} ${HEADER_FILES} ${SHADER_FILES})
	set(SRC_FILES ${EXT_SRC_FILES} ${EXE_SRC_FILE})
	add_executable(${OUTPUT} ${SRC_FILES} ${HEADER_FILES})
	target_link_libraries(${OUTPUT} ${ALL_LIBRARIES})
	set_target_properties(${OUTPUT} PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
	)
	set_target_properties(${OUTPUT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
	if (MSVC)
		target_compile_options(${OUTPUT} PRIVATE /W3)
	else()
		target_compile_options(${OUTPUT} PRIVATE -Wall -Wextra)
	endif()
endforeach()

#file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})