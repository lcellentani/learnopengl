#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace
{

constexpr float cYaw = -90.0f;
constexpr float cPitch = 0.0f;
constexpr float cMovementSpeed = 2.5f;
constexpr float cMouseSensitivity = 0.1f;
constexpr float cDefaultFOV = 45.0f;

}

Camera::Camera()
	: mPosition(0.0f, 0.0f, 0.0f)
	, mForward(0.0f, 0.0f, -1.0f)
	, mUp(0.0f, 1.0f, 0.0f)
	, mRight(1.0f, 0.0f, 0.0f)
	, mYaw(cYaw)
	, mPitch(cPitch)
	, mMovementSpeed(cMovementSpeed)
	, mMouseSensitivity(cMouseSensitivity)
	, mFOV(cDefaultFOV)
	, mWorldUp(0.0f, 1.0f, 0.0f) {
	Update();
}

Camera::Camera(const glm::vec3& position, const glm::vec3& up, const glm::vec3& direction)
	: mPosition(position)
	, mForward(direction)
	, mUp(0.0f, 1.0f, 0.0f)
	, mRight(1.0f, 0.0f, 0.0f)
	, mYaw(cYaw)
	, mPitch(cPitch)
	, mMovementSpeed(cMovementSpeed)
	, mMouseSensitivity(cMouseSensitivity)
	, mFOV(cDefaultFOV)
	, mWorldUp(up) {
	Update();
}

void Camera::SetPosition(const glm::vec3& position) {
	mPosition = position;
	Update();
}

void Camera::SetUpVector(const glm::vec3& up) {
	mWorldUp = up;
	Update();
}

void Camera::SetDirection(const glm::vec3& direction) {
	mForward = direction;
	Update();
}

glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(mPosition, mPosition + mForward, mUp);
}

void Camera::ProcessKeyboard(Move action, float deltaTime) {
	float velocity = mMovementSpeed * deltaTime;
	switch (action) {
	case Camera::Move::Forward:
		mPosition += mForward * velocity;
		break;
	case Camera::Move::Backward:
		mPosition -= mForward * velocity;
		break;
	case Camera::Move::Left:
		mPosition -= mRight * velocity;
		break;
	case Camera::Move::Right:
		mPosition += mRight * velocity;
		break;
	default:
		break;
	}
}

void Camera::ProcessMouse(float xOffset, float yOffset, bool constraintPitch) {
	xOffset *= mMouseSensitivity;
	yOffset *= mMouseSensitivity;

	mYaw += xOffset;
	mPitch += yOffset;

	if (constraintPitch) {
		if (mPitch > 89.0f) {
			mPitch = 89.0f;
		} else if (mPitch < -89.0f) {
			mPitch = -89.0f;
		}
	}
	Update();
}

void Camera::ProcessMouseScroll(float scrollOffset) {
	if (mFOV >= 1.0f && mFOV <= 45.0f) {
		mFOV -= scrollOffset;
	}
	if (mFOV <= 1.0f) {
		mFOV = 1.0f;
	} else if (mFOV >= 45.0f) {
		mFOV = 45.0f;
	}
}

void Camera::Update() {
	glm::vec3 forward;
	forward.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	forward.y = sin(glm::radians(mPitch));
	forward.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mForward = glm::normalize(forward);
	mRight = glm::normalize(glm::cross(mForward, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mForward));
}
