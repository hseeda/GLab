#include "Camera.h"

#include "Glad/glad.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include "GLFW/glfw3.h"


#include "HUtil.hpp"
#include "EventSystem.h"
#include "WindowSystem.h"

#define DEFAULT_FORWARD    (vec3(0, 0, -1))
#define DEFAULT_UP         (vec3(0, 1, 0))
#define DEFAULT_RIGHT      (vec3(1, 0, 0))

using namespace glm;

Camera::Camera(const glm::vec3& position)

	: m_position(position)

{}

Camera::Camera() {
	//&HS::CLASS::handle_function, this, std::placeholders::_1 ));
	ES::es.subscribe(9 , std::bind(&Camera::mouse_wheel, this, std::placeholders::_1));
	ES::es.subscribe(11, std::bind(&Camera::mouse_click, this, std::placeholders::_1));
	ES::es.subscribe(10, std::bind(&Camera::mouse_move, this, std::placeholders::_1));
}

void Camera::mouse_wheel(ES::event e) {
	
	Move(vec3(0.0, 0.0, e.data.d[1] * ZOOM_SCALE));
}

void Camera::mouse_move(ES::event e) {
	if (WS::mouse_down == GLFW_MOUSE_BUTTON_LEFT) {
		//_pl(WS::mouse_down);
		m_position = m_d_position + vec3(-e.data.d[0] * 4.0, e.data.d[1] *4.0 , 0.0f);
		return;
	}
	
}

void Camera::mouse_click(ES::event e) {
	GLFWwindow* w = (GLFWwindow * ) e.p;

	if (e.data.i[0] == GLFW_MOUSE_BUTTON_LEFT && e.data.i[1] == GLFW_PRESS) {
		m_d_position = m_position;
		return;
	}
	if (e.data.i[0] == GLFW_MOUSE_BUTTON_LEFT && e.data.i[1] == GLFW_RELEASE) {
		
	}
		
	
}


Camera::~Camera()

{}

vec3 Camera::Position() const

{

	return m_position;

}

vec3 Camera::Forward() const

{

	return rotate(m_rotation, DEFAULT_FORWARD);

}

vec3 Camera::Back() const

{

	return -rotate(m_rotation, DEFAULT_FORWARD);

}

vec3 Camera::Left() const

{

	return -rotate(m_rotation, DEFAULT_RIGHT);

}

vec3 Camera::Right() const

{

	return rotate(m_rotation, DEFAULT_RIGHT);

}

vec3 Camera::Up() const

{

	return rotate(m_rotation, DEFAULT_UP);

}

vec3 Camera::Down() const

{

	return -rotate(m_rotation, DEFAULT_UP);

}

float Camera::FieldOfView() const

{

	return m_fieldofview;

}

float Camera::ZNear() const

{

	return m_znear;

}

float Camera::ZFar() const

{

	return m_zfar;

}

mat4 Camera::View() const

{

	return lookAt(m_position, m_position + Forward(), Up());

}

mat4 Camera::Projection(float aspectratio) const

{

	return perspective(m_fieldofview, aspectratio, m_znear, m_zfar);

}

void Camera::SetPosition(const glm::vec3& position)

{

	m_position = position;

}

void Camera::Move(const glm::vec3& direction)

{

	m_position += direction;

}

void Camera::Pitch(float angle)

{

	m_rotation = rotate(m_rotation, angle, DEFAULT_RIGHT);

}

void Camera::Yaw(float angle)

{

	m_rotation = rotate(m_rotation, angle, DEFAULT_UP);

}

void Camera::Roll(float angle)

{

	m_rotation = rotate(m_rotation, angle, DEFAULT_FORWARD);

}

void Camera::ZoomIn(float angle)

{

	m_fieldofview -= angle;

	if (m_fieldofview < radians(1.0f))

		m_fieldofview = radians(1.0f);

}

void Camera::ZoomOut(float angle)

{

	m_fieldofview += angle;

	if (m_fieldofview > radians(90.0f))

		m_fieldofview = radians(90.0f);

}

void Camera::SetClippingPlanes(float znear, float zfar)

{

	if (znear > 0 && zfar > znear)

	{

		m_znear = znear;

		m_zfar = zfar;

	}

}