function(Enable_Cpp11 target)
	set_target_properties(${target} PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS OFF
	)
endfunction(Enable_Cpp11)

function(AddCompilerFlags target)
	if(MSVC)
		add_definitions(-D_CRT_SECURE_NO_WARNINGS)
		add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
		add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
		add_definitions(-D_SCL_SECURE_NO_DEPRECATE)
		if (MSVC14)
			add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
		else()
			add_definitions(-D_HAS_ITERATOR_DEBUGGING=0)
		endif()
	endif()

	set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/W4 /WX /GR- /Gy /arch:SSE2 /wd4201 /wd4324")
endfunction(AddCompilerFlags)

function(SetLinkerSubsystem target)
	if(WIN32) # Check if we are on Windows
		if(MSVC) # Check if we are using the Visual Studio compiler
			set_target_properties(${target} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\" /NODEFAULTLIB:\"msvcrt\"")
		elseif(CMAKE_COMPILER_IS_GNUCXX)
			# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
		else()
			message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
		endif()
	elseif(UNIX)
		# Nothing special required
	else()
		message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
	endif()
endfunction(SetLinkerSubsystem)

function(SetupSample target)
target_include_directories(${target}
	PUBLIC
		"${PROJECT_SOURCE_DIR}/source/common"
)
target_link_libraries(${target}
	common
	opengl32.lib
	glfw3.lib
)
endfunction(SetupSample)