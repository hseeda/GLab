#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "EventSystem.h"

class Camera

{

public:

	Camera(const glm::vec3& position);

	Camera();
	
	void mouse_wheel(ES::event e);

	void mouse_click(ES::event e);
	
	virtual ~Camera();

	glm::vec3 Position() const;

	glm::vec3 Forward() const;

	glm::vec3 Back() const;

	glm::vec3 Left() const;

	glm::vec3 Right() const;

	glm::vec3 Up() const;

	glm::vec3 Down() const;

	float FieldOfView() const;

	float ZNear() const;

	float ZFar() const;

	glm::mat4 View() const;

	glm::mat4 Projection(float aspectratio) const;

	void SetPosition(const glm::vec3& position);

	void Move(const glm::vec3& direction);

	void Pitch(float angle);

	void Yaw(float angle);

	void Roll(float angle);

	void ZoomIn(float angle);

	void ZoomOut(float angle);

	void SetClippingPlanes(float znear, float zfar);

private:

	glm::vec3 m_position{ 0, 0, 10 };

	glm::quat m_rotation{ 1, 0, 0, 0 };

	float m_fieldofview{ glm::radians(45.0f) };

	float m_znear{ 0.1f };

	float m_zfar{ 100.0f };

};


#endif // CAMERA_H
