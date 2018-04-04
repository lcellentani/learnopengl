#pragma once

#include "CommonDefine.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat2x2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"

#define SHADER_SOURCE(...) #__VA_ARGS__

using ShaderProgramHandle = ResourceHandle;

struct ShaderProgramParams {
	std::string mVertexShaderData;
	std::string mFragmentShaderData;
};

ShaderProgramHandle ShaderProgram_Create(const ShaderProgramParams& params);

void ShaderProgram_Destroy(const ShaderProgramHandle& handle);

void ShaderProgram_Use(const ShaderProgramHandle& handle);

void ShaderProgram_SetInt(const ShaderProgramHandle& handle, const char* name, int data);
void ShaderProgram_SetFloat(const ShaderProgramHandle& handle, const char* name, float data);
void ShaderProgram_SetVec2(const ShaderProgramHandle& handle, const char* name, float f0, float f1);
void ShaderProgram_SetVec3(const ShaderProgramHandle& handle, const char* name, float f0, float f1, float f2);
void ShaderProgram_SetVec4(const ShaderProgramHandle& handle, const char* name, float f0, float f1, float f2, float f3);
void ShaderProgram_SetVec2(const ShaderProgramHandle& handle, const char* name, const glm::vec2& data);
void ShaderProgram_SetVec3(const ShaderProgramHandle& handle, const char* name, const glm::vec3& data);
void ShaderProgram_SetVec4(const ShaderProgramHandle& handle, const char* name, const glm::vec4& data);
void ShaderProgram_SetMat2(const ShaderProgramHandle& handle, const char* name, const glm::mat2& data);
void ShaderProgram_SetMat3(const ShaderProgramHandle& handle, const char* name, const glm::mat3& data);
void ShaderProgram_SetMat4(const ShaderProgramHandle& handle, const char* name, const glm::mat4& data);