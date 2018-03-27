file(GLOB EXAMPLES_COMMON_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")

if(MSVC) 
	list(APPEND EXAMPLES_COMMON_ALL_INCLUDES ${EXAMPLES_COMMON_INCLUDES}) 
endif()

add_library(examples-common
	${EXAMPLES_COMMON_ALL_INCLUDES}
	${PROJECT_SOURCE_DIR}/3rdparty/glad/src/glad.c
)

if(MSVC)
	source_group("Header Files\\" FILES ${EXAMPLES_COMMON_INCLUDES}) 
endif()

Enable_Cpp11(examples-common)
AddCompilerFlags(examples-common)

