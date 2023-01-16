#pragma once
#include "pch.h"

#define ZOOM_MULTIPLIER 0.5f
#define ORTHO_ZOOM_MULTIPLIER 1.1f
#define ROTATION_MULTIPLIER 6.283185307179586f // 2 pi
#define ORBIT_MULTIPLIER 0.2f

namespace ES { struct event; }

class Camera

{

public:

	glm::mat4 Vmat;
	glm::mat4 PVmat;

	Camera();
	
	void qrotate(const float& degrees, const glm::vec3& axis);

	
	void resize(CEvent e);
	void update();
	void mouse_wheel(CEvent e);

	void mouse_move(CEvent e);
	void mouse_click(CEvent e);
	
	bool to2D(glm::vec3& in_pos3d, glm::vec2& out_pos2d);
	
	virtual ~Camera();
	glm::vec3 Position() const;
	float FieldOfView() const;
	float ZNear() const;
	float ZFar() const;
	glm::mat4 Projection(float aspectratio) const;
	void SetPosition(const glm::vec3& position);
	void Move(const glm::vec3& direction);
	void Pitch(float angle);
	void Yaw(float angle);
	void Roll(float angle);
	void ZoomIn(float angle);
	void ZoomOut(float angle);
	void SetClippingPlanes(float znear, float zfar);
	
	void addImguiControls(int  imguiWID, std::string imguiItemName);

	void switchCameraMode(bool perspective);
private:
	int is_perspective = 1;
	float ortho_scale  = 3.73f;
	float angle = 0.0f;
	glm::quat m_quat;
	glm::quat d_quat;

	glm::vec3 m_position{ -1, 0, 10 };
	glm::vec3 d_position;
	
	glm::quat m_rotation{ 1, 0, 0, 0 };
	float m_fieldofview{ glm::radians(45.0f) };
	float m_znear{ 0.01f };
	float m_zfar{ 100.0f };
};