#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Log.h"

namespace gl { namespace details { const char* GetErrorString(GLenum _enum); } }

#define GL_ERROR(condition) \
			if (condition) { GLenum glError = glGetError(); if (glError != GL_NO_ERROR) { Log(tinyngine::Logger::Error, "GL error 0x%x %s", glError, gl::details::GetErrorString(glError)); abort(); } }

#define GL_CHECK(_call) \
			for(;;) { \
			_call; \
			GLenum glError = glGetError(); \
			if (glError != GL_NO_ERROR) { Log(tinyngine::Logger::Error, "GL error 0x%x %s", glError, gl::details::GetErrorString(glError)); abort(); } \
			break; }