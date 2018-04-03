file(GLOB EXAMPLES_COMMON_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")
file(GLOB EXAMPLES_COMMON_MEDIA "${PROJECT_SOURCE_DIR}/media/*.*")

if(MSVC) 
	list(APPEND EXAMPLES_COMMON_ALL_INCLUDES ${EXAMPLES_COMMON_INCLUDES}) 
endif()

add_library(common
	${EXAMPLES_COMMON_ALL_INCLUDES}
	${PROJECT_SOURCE_DIR}/3rdparty/glad/src/glad.c
	GLApi.cpp
	Log.cpp
	ShaderProgram.cpp
	StringUtils.cpp
	Texture.cpp
)

if(MSVC)
	source_group("Header Files\\" FILES ${EXAMPLES_COMMON_INCLUDES}) 
endif()

Enable_Cpp11(common)
AddCompilerFlags(common)

