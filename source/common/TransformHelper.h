#pragma once

#include "CommonDefine.h"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

class TransformHelper final {
public:
	enum class MatrixMode : uint8_t {
		None = 0,
		Projection,
		Model,
		View,
		Texture
	};

	void Reset();

	void SetMatrixMode(MatrixMode mode);

	void LoadIdentity();
	void LoadMatrix(glm::mat4& matrix);

	void MultiplyMatrix(glm::mat4& matrix);

	void Translate(glm::vec3& translation);
	void Translate(float tx, float ty, float tz);

	void Rotate(float angle_in_degree, glm::vec3& axis);
	void Rotate(float angle_in_degree, float ax, float ay, float az);

	void Scale(glm::vec3& scale);
	void Scale(float sx, float sy, float sz);

	void PushMatrix();
	void PopMatrix();

	glm::mat4& GetModelMatrix();
	const glm::mat4& GetModelMatrix() const;
	
	glm::mat4& GetViewMatrix();
	const glm::mat4& GetViewMatrix() const;

	glm::mat4& GetProjectionMatrix();
	const glm::mat4& GetProjectionMatrix() const;
	
	glm::mat4& GetTextureMatrix();
	const glm::mat4& GetTextureMatrix() const;

	const glm::mat4& GetModelViewMatrix() const;
	const glm::mat4& GetModelViewProjectionMatrix() const;

	TransformHelper();
	~TransformHelper() = default;

	TransformHelper(TransformHelper& rhs);
	TransformHelper& operator=(TransformHelper& rhs);

	TransformHelper(TransformHelper&& rhs) noexcept = default;
	TransformHelper& operator= (TransformHelper&& rhs) noexcept = default;

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};
