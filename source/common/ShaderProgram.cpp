#include "ShaderProgram.h"

#include "GLApi.h"
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
		Log(tinyngine::Logger::Error, "ERROR::SHADER_COMPILATION_ERROR %s", infoLog);
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
		Log(tinyngine::Logger::Error, "ERROR::PROGRAM_LINKING_ERROR %s", infoLog);
	}
	return true;
}

class ShaderProgram {
public:
	ShaderProgram() = default;
	~ShaderProgram() {
		Destroy();
	}

	void Create() {
		mId = glCreateProgram();
		GL_ERROR(mId == 0);
		mAttachedShaders.reserve(4);
	}

	void Destroy() {
		if (IsValid()) {
			GL_CHECK(glDeleteProgram(mId));
			mId = 0;
		}
	}

	void AttachShader(uint32_t shaderType, const char* shaderCode) {
		if (shaderCode && IsValid()) {
			uint32_t shaderId = glCreateShader(shaderType);
			GL_ERROR(shaderId == 0);
			GL_CHECK(glShaderSource(shaderId, 1, &shaderCode, NULL));
			GL_CHECK(glCompileShader(shaderId));
			if (CheckShaderDidCompile(shaderId)) {
				GL_CHECK(glAttachShader(mId, shaderId));
				mAttachedShaders.push_back(shaderId);
			} else {
				Destroy();
			}
		}
	}

	void Link() {
		if (IsValid()) {
			GL_CHECK(glLinkProgram(mId));
			if (CheckProgramDidCompile(mId)) {
				for (auto shaderId : mAttachedShaders) {
					GL_CHECK(glDeleteShader(shaderId));
				}
			} else {
				Destroy();
			}
			mAttachedShaders.clear();
		}
	}

	void Use() {
		if (IsValid()) {
			GL_CHECK(glUseProgram(mId));
		}
	}

	void SetUniformInt(const char* name, GLint data) {
		if (IsValid() && name) {
			int32_t location = glGetUniformLocation(mId, name);
			GL_ERROR(location == -1);
			GL_CHECK(glUniform1i(location, data));
		}
	}

	void SetUniformFloat(const char* name, GLfloat data) {
		if (IsValid() && name) {
			int32_t location = glGetUniformLocation(mId, name);
			GL_ERROR(location == -1);
			GL_CHECK(glUniform1f(location, data));
		}
	}

	void SetUniformVec2f(const char* name, GLfloat f0, GLfloat f1) {
		if (IsValid() && name) {
			int32_t location = glGetUniformLocation(mId, name);
			GL_ERROR(location == -1);
			GL_CHECK(glUniform2f(location, f0, f1));
		}
	}

	void SetUniformVec3f(const char* name, GLfloat f0, GLfloat f1, GLfloat f2) {
		if (IsValid() && name) {
			int32_t location = glGetUniformLocation(mId, name);
			GL_ERROR(location == -1);
			GL_CHECK(glUniform3f(location, f0, f1, f2));
		}
	}

	void SetUniformVec4f(const char* name, GLfloat f0, GLfloat f1, GLfloat f2, GLfloat f3) {
		if (IsValid() && name) {
			int32_t location = glGetUniformLocation(mId, name);
			GL_ERROR(location == -1);
			GL_CHECK(glUniform4f(location, f0, f1, f2, f3));
		}
	}

	void SetUniformVec2v(const char* name, const GLfloat* data) {
		if (IsValid() && name) {
			int32_t location = glGetUniformLocation(mId, name);
			GL_ERROR(location == -1);
			GL_CHECK(glUniform2fv(location, 1, data));
		}
	}

	void SetUniformVec3v(const char* name, const GLfloat* data) {
		if (IsValid() && name) {
			int32_t location = glGetUniformLocation(mId, name);
			GL_ERROR(location == -1);
			GL_CHECK(glUniform3fv(location, 1, data));
		}
	}

	void SetUniformVec4v(const char* name, const GLfloat* data) {
		if (IsValid() && name) {
			int32_t location = glGetUniformLocation(mId, name);
			GL_ERROR(location == -1);
			GL_CHECK(glUniform4fv(location, 1, data));
		}
	}

	void SetUniformMat2v(const char* name, const GLfloat* data) {
		if (IsValid() && name) {
			int32_t location = glGetUniformLocation(mId, name);
			GL_ERROR(location == -1);
			GL_CHECK(glUniformMatrix2fv(location, 1, GL_FALSE, data));
		}
	}

	void SetUniformMat3v(const char* name, const GLfloat* data) {
		if (IsValid() && name) {
			int32_t location = glGetUniformLocation(mId, name);
			GL_ERROR(location == -1);
			GL_CHECK(glUniformMatrix3fv(location, 1, GL_FALSE, data));
		}
	}

	void SetUniformMat4v(const char* name, const GLfloat* data) {
		if (IsValid() && name) {
			int32_t location = glGetUniformLocation(mId, name);
			GL_ERROR(location == -1);
			GL_CHECK(glUniformMatrix4fv(location, 1, GL_FALSE, data));
		}
	}

	bool IsValid() const {
		return mId > 0;
	}

private:
	GLuint mId = 0;
	std::vector<GLuint> mAttachedShaders;
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

void ShaderProgram_SetInt(const ShaderProgramHandle & handle, const char * name, int data) {
	if (!handle.IsValid()) {
		return;
	}
	auto& program = sShaderPrograms[handle.mHandle];
	program.SetUniformInt(name, data);
}

void ShaderProgram_SetFloat(const ShaderProgramHandle& handle, const char* name, float data) {
	if (!handle.IsValid()) {
		return;
	}
	auto& program = sShaderPrograms[handle.mHandle];
	program.SetUniformFloat(name, data);
}

void ShaderProgram_SetVec2(const ShaderProgramHandle& handle, const char* name, float f0, float f1) {
	if (!handle.IsValid()) {
		return;
	}
	auto& program = sShaderPrograms[handle.mHandle];
	program.SetUniformVec2f(name, f0, f1);
}

void ShaderProgram_SetVec3(const ShaderProgramHandle& handle, const char* name, float f0, float f1, float f2) {
	if (!handle.IsValid()) {
		return;
	}
	auto& program = sShaderPrograms[handle.mHandle];
	program.SetUniformVec3f(name, f0, f1, f2);
}

void ShaderProgram_SetVec4(const ShaderProgramHandle& handle, const char* name, float f0, float f1, float f2, float f3) {
	if (!handle.IsValid()) {
		return;
	}
	auto& program = sShaderPrograms[handle.mHandle];
	program.SetUniformVec4f(name, f0, f1, f2, f3);
}

void ShaderProgram_SetVec2(const ShaderProgramHandle& handle, const char* name, const glm::vec2& data) {
	if (!handle.IsValid()) {
		return;
	}
	auto& program = sShaderPrograms[handle.mHandle];
	program.SetUniformVec2v(name, &data[0]);
}

void ShaderProgram_SetVec3(const ShaderProgramHandle& handle, const char* name, const glm::vec3& data) {
	if (!handle.IsValid()) {
		return;
	}
	auto& program = sShaderPrograms[handle.mHandle];
	program.SetUniformVec3v(name, &data[0]);
}

void ShaderProgram_SetVec4(const ShaderProgramHandle& handle, const char* name, const glm::vec4& data) {
	if (!handle.IsValid()) {
		return;
	}
	auto& program = sShaderPrograms[handle.mHandle];
	program.SetUniformVec4v(name, &data[0]);
}

void ShaderProgram_SetMat2(const ShaderProgramHandle & handle, const char * name, const glm::mat2& data) {
	if (!handle.IsValid()) {
		return;
	}
	auto& program = sShaderPrograms[handle.mHandle];
	program.SetUniformMat2v(name, &data[0][0]);
}

void ShaderProgram_SetMat3(const ShaderProgramHandle & handle, const char * name, const glm::mat3& data) {
	if (!handle.IsValid()) {
		return;
	}
	auto& program = sShaderPrograms[handle.mHandle];
	program.SetUniformMat3v(name, &data[0][0]);
}


void ShaderProgram_SetMat4(const ShaderProgramHandle & handle, const char * name, const glm::mat4& data) {
	if (!handle.IsValid()) {
		return;
	}
	auto& program = sShaderPrograms[handle.mHandle];
	program.SetUniformMat4v(name, &data[0][0]);
}
