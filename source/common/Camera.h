#pragma once

#include "CommonDefine.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

class Camera {
public:
	enum class Move {
		Forward,
		Backward,
		Left,
		Right
	};
public:
	Camera();
	Camera(const glm::vec3& position, const glm::vec3& up, const glm::vec3& direction);
	~Camera() = default;

	void SetPosition(const glm::vec3& position);
	void SetUpVector(const glm::vec3& up);
	void SetDirection(const glm::vec3& direction);

	glm::mat4 GetViewMatrix() const;

	void ProcessKeyboard(Move action, float deltaTime);
	void ProcessMouse(float xOffset, float yOffset, bool constraintPitch = true);
	void ProcessMouseScroll(float scrollOffset);

	float GetFOV() const { return mFOV; }
	glm::vec3 GetPosition() const { return mPosition; }

private:
	void Update();

private:
	glm::vec3 mPosition;
	glm::vec3 mForward;
	glm::vec3 mUp;
	glm::vec3 mRight;

	float mYaw;
	float mPitch;

	float mMovementSpeed;
	float mMouseSensitivity;
	float mFOV;

	glm::vec3 mWorldUp;
};