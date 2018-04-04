#include "TransformHelper.h"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <memory>

constexpr uint16_t cMaxModelMatrix = 8;
constexpr uint16_t cMaxViewMatrix = 8;
constexpr uint16_t cMaxProjectionMatrix = 2;
constexpr uint16_t cMaxTextureMatrix = 2;

struct TransformHelper::Impl {
	MatrixMode mMode;

	uint8_t mModelMatrixIndex;
	uint8_t mViewMatrixIndex;
	uint8_t mProjectionMatrixIndex;
	uint8_t mTextureMatrixIndex;

	glm::mat4 mModelMatrix[cMaxModelMatrix];
	glm::mat4 mViewMatrix[cMaxViewMatrix];
	glm::mat4 mProjectionMatrix[cMaxProjectionMatrix];
	glm::mat4 mTextureMatrix[cMaxTextureMatrix];

	glm::mat4 mMoldeViewMatrix;
	glm::mat4 mModelViewProjectionMatrix;
};

TransformHelper::TransformHelper() : mImpl(std::make_unique<Impl>()) {
	Reset();
}

TransformHelper::TransformHelper(TransformHelper& rhs) : mImpl(nullptr) {
	if (rhs.mImpl != nullptr) {
		mImpl = std::make_unique<Impl>((*rhs.mImpl));
	}
}

TransformHelper& TransformHelper::operator=(TransformHelper& rhs) {
	if (rhs.mImpl == nullptr) {
		mImpl.reset();
	}
	else if (mImpl == nullptr) {
		mImpl = std::make_unique<Impl>((*rhs.mImpl));
	} else {
		*mImpl = *rhs.mImpl;
	}
	return (*this);
}

//====================================================================================================================

void TransformHelper::Reset() {
	mImpl->mMode = MatrixMode::None;
	mImpl->mModelMatrixIndex = 0;
	mImpl->mViewMatrixIndex = 0;
	mImpl->mProjectionMatrixIndex = 0;
	mImpl->mTextureMatrixIndex = 0;
	SetMatrixMode(MatrixMode::Model);
	LoadIdentity();
	SetMatrixMode(MatrixMode::View);
	LoadIdentity();
	SetMatrixMode(MatrixMode::Projection);
	LoadIdentity();
	SetMatrixMode(MatrixMode::Texture);
	LoadIdentity();
}

void TransformHelper::SetMatrixMode(MatrixMode mode) {
	mImpl->mMode = mode;
}

void TransformHelper::LoadIdentity() {
	MatrixMode mode = mImpl->mMode;
	if (mode == MatrixMode::Model) {
		glm::mat4& m = GetModelMatrix();
		std::memset((void*)std::addressof(m), 0, sizeof(glm::mat4));
		m[0][0] = 1.0f; m[1][1] = 1.0f; m[2][2] = 1.0; m[3][3] = 1.0;
		return;
	}
	if (mode == MatrixMode::View) {
		glm::mat4& m = GetViewMatrix();
		std::memset((void*)std::addressof(m), 0, sizeof(glm::mat4));
		m[0][0] = 1.0f; m[1][1] = 1.0f; m[2][2] = 1.0; m[3][3] = 1.0;
		return;
	}
	if (mode == MatrixMode::Projection) {
		glm::mat4& m = GetProjectionMatrix();
		std::memset((void*)std::addressof(m), 0, sizeof(glm::mat4));
		m[0][0] = 1.0f; m[1][1] = 1.0f; m[2][2] = 1.0; m[3][3] = 1.0;
		return;
	}
	if (mode == MatrixMode::Texture) {
		glm::mat4& m = GetTextureMatrix();
		std::memset((void*)std::addressof(m), 0, sizeof(glm::mat4));
		m[0][0] = 1.0f; m[1][1] = 1.0f; m[2][2] = 1.0; m[3][3] = 1.0;
		return;
	}
}

void TransformHelper::LoadMatrix(glm::mat4& matrix) {
	MatrixMode mode = mImpl->mMode;
	if (mode == MatrixMode::Model) {
		glm::mat4& m = GetModelMatrix();
		std::memcpy((void*)std::addressof(m), (const void*)std::addressof(matrix), sizeof(glm::mat4));
		return;
	}
	if (mode == MatrixMode::View) {
		glm::mat4& m = GetViewMatrix();
		std::memcpy((void*)std::addressof(m), (const void*)std::addressof(matrix), sizeof(glm::mat4));
		return;
	}
	if (mode == MatrixMode::Projection) {
		glm::mat4& m = GetProjectionMatrix();
		std::memcpy((void*)std::addressof(m), (const void*)std::addressof(matrix), sizeof(glm::mat4));
		return;
	}
	if (mode == MatrixMode::Texture) {
		glm::mat4& m = GetTextureMatrix();
		std::memcpy((void*)std::addressof(m), (const void*)std::addressof(matrix), sizeof(glm::mat4));
		return;
	}
}

void TransformHelper::MultiplyMatrix(glm::mat4& matrix) {
	MatrixMode mode = mImpl->mMode;
	if (mode == MatrixMode::Model) {
		glm::mat4& m = GetModelMatrix();
		m *= matrix;
		return;
	}
	if (mode == MatrixMode::View) {
		glm::mat4& m = GetViewMatrix();
		m *= matrix;
		return;
	}
	if (mode == MatrixMode::Projection) {
		glm::mat4& m = GetProjectionMatrix();
		m *= matrix;
		return;
	}
	if (mode == MatrixMode::Texture) {
		glm::mat4& m = GetTextureMatrix();
		m *= matrix;
		return;
	}
}

void TransformHelper::Translate(glm::vec3& translation) {
	MatrixMode mode = mImpl->mMode;
	if (mode == MatrixMode::Model) {
		glm::mat4& m = GetModelMatrix();
		m = glm::translate(m, translation);
		return;
	}
	if (mode == MatrixMode::View) {
		glm::mat4& m = GetViewMatrix();
		m = glm::translate(m, translation);
		return;
	}
	if (mode == MatrixMode::Projection) {
		glm::mat4& m = GetProjectionMatrix();
		m = glm::translate(m, translation);
		return;
	}
	if (mode == MatrixMode::Texture) {
		glm::mat4& m = GetTextureMatrix();
		m = glm::translate(m, translation);
		return;
	}
}

void TransformHelper::Translate(float tx, float ty, float tz) {
	glm::vec3 t(tx, ty, tz);
	Translate(t);
}

void TransformHelper::Rotate(float angle_in_degree, glm::vec3& axis) {
	float angle_in_radians = glm::radians(angle_in_degree);
	MatrixMode mode = mImpl->mMode;
	if (mode == MatrixMode::Model) {
		glm::mat4& m = GetModelMatrix();
		m = glm::rotate(m, angle_in_radians, axis);
		return;
	}
	if (mode == MatrixMode::View) {
		glm::mat4& m = GetViewMatrix();
		m = glm::rotate(m, angle_in_radians, axis);
		return;
	}
	if (mode == MatrixMode::Projection) {
		glm::mat4& m = GetProjectionMatrix();
		m = glm::rotate(m, angle_in_radians, axis);
		return;
	}
	if (mode == MatrixMode::Texture) {
		glm::mat4& m = GetTextureMatrix();
		m = glm::rotate(m, angle_in_radians, axis);
		return;
	}
}

void TransformHelper::Rotate(float angle_in_degree, float ax, float ay, float az) {
	glm::vec3 axis(ax, ay, az);
	Rotate(angle_in_degree, axis);
}

void TransformHelper::Scale(glm::vec3& scale) {
	MatrixMode mode = mImpl->mMode;
	if (mode == MatrixMode::Model) {
		glm::mat4& m = GetModelMatrix();
		m = glm::scale(m, scale);
		return;
	}
	if (mode == MatrixMode::View) {
		glm::mat4& m = GetViewMatrix();
		m = glm::scale(m, scale);
		return;
	}
	if (mode == MatrixMode::Projection) {
		glm::mat4& m = GetProjectionMatrix();
		m = glm::scale(m, scale);
		return;
	}
	if (mode == MatrixMode::Texture) {
		glm::mat4& m = GetTextureMatrix();
		m = glm::scale(m, scale);
		return;
	}
}

void TransformHelper::Scale(float sx, float sy, float sz) {
	glm::vec3 s(sx, sy, sz);
	Scale(s);
}

void TransformHelper::PushMatrix() {
	MatrixMode mode = mImpl->mMode;
	if (mode == MatrixMode::Model) {
		if (mImpl->mModelMatrixIndex >= cMaxModelMatrix) {
			return;
		}
		std::memcpy((void*)std::addressof(mImpl->mModelMatrix[mImpl->mModelMatrixIndex + 1]), (const void*)std::addressof(mImpl->mModelMatrix[mImpl->mModelMatrixIndex]), sizeof(glm::mat4));
		mImpl->mModelMatrixIndex++;
		return;
	}
	if (mode == MatrixMode::View) {
		if (mImpl->mViewMatrixIndex >= cMaxViewMatrix) {
			return;
		}
		std::memcpy((void*)std::addressof(mImpl->mViewMatrix[mImpl->mViewMatrixIndex + 1]), (const void*)std::addressof(mImpl->mViewMatrix[mImpl->mViewMatrixIndex]), sizeof(glm::mat4));
		mImpl->mViewMatrixIndex++;
		return;
	}
	if (mode == MatrixMode::Projection) {
		if (mImpl->mProjectionMatrixIndex >= cMaxProjectionMatrix) {
			return;
		}
		std::memcpy((void*)std::addressof(mImpl->mProjectionMatrix[mImpl->mProjectionMatrixIndex + 1]), (const void*)std::addressof(mImpl->mProjectionMatrix[mImpl->mProjectionMatrixIndex]), sizeof(glm::mat4));
		mImpl->mProjectionMatrixIndex++;
		return;
	}
	if (mode == MatrixMode::Texture) {
		if (mImpl->mTextureMatrixIndex >= cMaxTextureMatrix) {
			return;
		}
		std::memcpy((void*)std::addressof(mImpl->mTextureMatrix[mImpl->mTextureMatrixIndex + 1]), (const void*)std::addressof(mImpl->mTextureMatrix[mImpl->mTextureMatrixIndex]), sizeof(glm::mat4));
		mImpl->mTextureMatrixIndex++;
		return;
	}
}

void TransformHelper::PopMatrix() {
	MatrixMode mode = mImpl->mMode;
	if (mode == MatrixMode::Model) {
		if (mImpl->mModelMatrixIndex == 0) {
			return;
		}
		mImpl->mModelMatrixIndex--;
		return;
	}
	if (mode == MatrixMode::View) {
		if (mImpl->mViewMatrixIndex == 0) {
			return;
		}
		mImpl->mViewMatrixIndex--;
		return;
	}
	if (mode == MatrixMode::Projection) {
		if (mImpl->mProjectionMatrixIndex == 0) {
			return;
		}
		mImpl->mProjectionMatrixIndex--;
		return;
	}
	if (mode == MatrixMode::Texture) {
		if (mImpl->mTextureMatrixIndex == 0) {
			return;
		}
		mImpl->mTextureMatrixIndex--;
		return;
	}
}

glm::mat4& TransformHelper::GetModelMatrix() {
	return mImpl->mModelMatrix[mImpl->mModelMatrixIndex];
}

const glm::mat4& TransformHelper::GetModelMatrix() const {
	return mImpl->mModelMatrix[mImpl->mModelMatrixIndex];
}

glm::mat4& TransformHelper::GetViewMatrix() {
	return mImpl->mViewMatrix[mImpl->mViewMatrixIndex];
}

const glm::mat4& TransformHelper::GetViewMatrix() const {
	return mImpl->mViewMatrix[mImpl->mViewMatrixIndex];
}

glm::mat4& TransformHelper::GetProjectionMatrix() {
	return mImpl->mProjectionMatrix[mImpl->mProjectionMatrixIndex];
}

const glm::mat4& TransformHelper::GetProjectionMatrix() const {
	return mImpl->mProjectionMatrix[mImpl->mProjectionMatrixIndex];
}

glm::mat4& TransformHelper::GetTextureMatrix() {
	return mImpl->mTextureMatrix[mImpl->mTextureMatrixIndex];
}

const glm::mat4& TransformHelper::GetTextureMatrix() const {
	return mImpl->mTextureMatrix[mImpl->mTextureMatrixIndex];
}

const glm::mat4& TransformHelper::GetModelViewMatrix() const {
	mImpl->mMoldeViewMatrix = GetViewMatrix() * GetModelMatrix();
	return mImpl->mMoldeViewMatrix;
}

const glm::mat4& TransformHelper::GetModelViewProjectionMatrix() const {
	mImpl->mModelViewProjectionMatrix = GetProjectionMatrix() * GetViewMatrix() * GetModelMatrix();
	return mImpl->mModelViewProjectionMatrix;

}
