#pragma once

#include "CommonDefine.h"

#define SHADER_SOURCE(...) #__VA_ARGS__

using ShaderProgramHandle = ResourceHandle;

struct ShaderProgramParams {
	std::string mVertexShaderData;
	std::string mFragmentShaderData;
};

ShaderProgramHandle ShaderProgram_Create(const ShaderProgramParams& params);

void ShaderProgram_Destroy(const ShaderProgramHandle& handle);

void ShaderProgram_Use(const ShaderProgramHandle& handle);

void ShaderProgram_SetFloat(const ShaderProgramHandle& handle, const char* name, float data);