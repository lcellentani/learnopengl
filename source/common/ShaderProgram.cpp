#include "ShaderProgram.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <vector>

namespace
{

bool CheckShaderDidCompile(uint32_t shader) {
	int success;
	char infoLog[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER_COMPILATION_ERROR\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		return false;
	}
	return true;
}

bool CheckProgramDidCompile(uint32_t program) {
	int success;
	char infoLog[1024];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 1024, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}
	return true;
}

class ShaderProgram {
public:
	ShaderProgram() = default;

	void Create() {
		mId = glCreateProgram();
		mAttachedShaders.reserve(4);
	}

	void Destroy() {
		if (IsValid()) {
			glDeleteProgram(mId);
			mId = 0;
		}
	}

	void AttachShader(uint32_t shaderType, const char* shaderCode) {
		if (shaderCode && IsValid()) {
			uint32_t shaderId = glCreateShader(shaderType);
			glShaderSource(shaderId, 1, &shaderCode, NULL);
			glCompileShader(shaderId);
			if (CheckShaderDidCompile(shaderId)) {
				glAttachShader(mId, shaderId);
				mAttachedShaders.push_back(shaderId);
			}
		}
	}

	void Link() {
		if (IsValid()) {
			glLinkProgram(mId);
			CheckProgramDidCompile(mId);
			for (auto shaderId : mAttachedShaders) {
				glDeleteShader(shaderId);
			}
			mAttachedShaders.clear();
		}
	}

	void Use() {
		if (IsValid()) {
			glUseProgram(mId);
		}
	}

	void SetUniformFloat(const char* name, float data) {
		if (IsValid()) {
			glUniform1f(glGetUniformLocation(mId, name), data);
		}
	}

	bool IsValid() const { return mId > 0; }

private:
	uint32_t mId = 0;
	std::vector<uint32_t> mAttachedShaders;
};

static constexpr uint32_t cMaxShaderProgramHandles = (1 << 6);
uint32_t sProgramsCount = 0;
std::array<ShaderProgram, cMaxShaderProgramHandles> sShaderPrograms;

}


ShaderProgramHandle ShaderProgram_Create(const ShaderProgramParams& params) {
	if (params.mVertexShaderData.empty()) {
		return ShaderProgramHandle(cInvalidHandle);
	}

	const char* vertexShaderCode = params.mVertexShaderData.c_str();
	const char* fragmentShaderCode = !params.mFragmentShaderData.empty() ? params.mFragmentShaderData.c_str() : nullptr;

	ShaderProgramHandle handle = ShaderProgramHandle(sProgramsCount);
	auto& program = sShaderPrograms[handle.mHandle];

	program.Create();
	program.AttachShader(GL_VERTEX_SHADER, vertexShaderCode);
	program.AttachShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
	program.Link();

	if (program.IsValid()) {
		sProgramsCount++;
		return handle;
	}
	return ShaderProgramHandle(cInvalidHandle);
}

void ShaderProgram_Destroy(const ShaderProgramHandle & handle) {
	if (!handle.IsValid()) {
		return;
	}
	auto& program = sShaderPrograms[handle.mHandle];
	program.Destroy();
}

void ShaderProgram_Use(const ShaderProgramHandle& handle) {
	if (!handle.IsValid()) {
		return;
	}
	auto& program = sShaderPrograms[handle.mHandle];
	program.Use();
}

void ShaderProgram_SetFloat(const ShaderProgramHandle& handle, const char* name, float data) {
	if (!handle.IsValid()) {
		return;
	}
	auto& program = sShaderPrograms[handle.mHandle];
	program.SetUniformFloat(name, data);
}