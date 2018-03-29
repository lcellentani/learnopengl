file(GLOB EXAMPLES_COMMON_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")

if(MSVC) 
	list(APPEND EXAMPLES_COMMON_ALL_INCLUDES ${EXAMPLES_COMMON_INCLUDES}) 
endif()

add_library(common
	${EXAMPLES_COMMON_ALL_INCLUDES}
	${PROJECT_SOURCE_DIR}/3rdparty/glad/src/glad.c
	ShaderProgram.cpp
	StringUtils.cpp
)

if(MSVC)
	source_group("Header Files\\" FILES ${EXAMPLES_COMMON_INCLUDES}) 
endif()

Enable_Cpp11(common)
AddCompilerFlags(common)

