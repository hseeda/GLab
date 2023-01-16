#include "pch.h"
#include "Camera.h"
#include "EventSystem.h"
#include "WindowSystem.h"
#include "ImguiSystem.h"

#define DEFAULT_FORWARD    (vec3(0, 0, -1))
#define DEFAULT_UP         (vec3(0, 1, 0))
#define DEFAULT_RIGHT      (vec3(1, 0, 0))

using namespace glm;

Camera::Camera() {
	//&HS::CLASS::handle_function, this, std::placeholders::_1 ));
	ES::subscribe(ET::window_resize , std::bind(&Camera::resize, this, std::placeholders::_1));
	ES::subscribe(ET::mouse_wheel, std::bind(&Camera::mouse_wheel, this, std::placeholders::_1));
	ES::subscribe(ET::mouse_click, std::bind(&Camera::mouse_click, this, std::placeholders::_1));
	ES::subscribe(ET::mouse_move_while_pressed, std::bind(&Camera::mouse_move, this, std::placeholders::_1));

	m_quat = glm::quat();
	m_quat = glm::normalize(m_quat);
	update();
}

void Camera::qrotate(const float& degrees, const glm::vec3& axis)
{
	if (axis == glm::vec3(0.0f, 1.0f, 0.0f)) {
		m_quat = glm::normalize(glm::angleAxis(degrees, axis)) * d_quat;
	}
	else
	{
		m_quat = d_quat * glm::normalize(glm::angleAxis(degrees, axis));
	}
		
}

void Camera::resize(CEvent e) {
	update();
}

void Camera::update()
{
	if (is_perspective) {
		glm::quat temp = glm::conjugate(m_quat);
		Vmat = glm::identity<glm::mat4>();
		Vmat = glm::translate(Vmat, -m_position);
		Vmat = Vmat * glm::mat4_cast(temp);
		PVmat = Projection(WS::Aspect) * Vmat;
	}
	else {
		Vmat = glm::scale(Vmat, glm::vec3(1.0f / ortho_scale, 1.0f / ortho_scale, 1.0f / ortho_scale));
		
		PVmat = glm::ortho(-WS::Aspect , WS::Aspect,
				-1.0f, 1.0f, -m_zfar, m_zfar) * Vmat;
	}
	ES::needsUpdate();
}

float angleBetween(glm::vec3 a,glm::vec3 b,glm::vec3 origin) {
	glm::vec3 da = glm::normalize(a - origin);
	glm::vec3 db = glm::normalize(b - origin);
	return glm::acos(glm::dot(da, db));
}

void Camera::mouse_move(CEvent e) {
	float m = 1.0f; if (WS::mods == 1) m = 0.1f; else if (WS::mods == 2) m = 10.0f;
	
	if (WS::mouse_down == GLFW_MOUSE_BUTTON_LEFT) {
		
		vec3 raxis(0, 0, 1);
		
		float dx = WS::xc - WS::xc_1;
		float dy = WS::yc - WS::yc_1;
		float tmp_ang;

		if (WS::xd > WS::Width * ORBIT_MULTIPLIER &&
			WS::xd < WS::Width * (1.0f - ORBIT_MULTIPLIER) &&
			WS::yd > WS::Height * ORBIT_MULTIPLIER &&
			WS::yd < WS::Height * (1.0f - ORBIT_MULTIPLIER)
			)
		{
			raxis = glm::normalize(glm::cross(d_position, d_position + vec3(-e.data.d(0), e.data.d(1), 0.0f)));
			tmp_ang = angle;
			angle = pow(WS::deltaX * WS::deltaX + WS::deltaY * WS::deltaY, 0.5) / WS::Width * ROTATION_MULTIPLIER;

			//_pl(angle, " ",m);
			qrotate(angle, raxis);
		}
		else
		{
			if (WS::xd < WS::Width * ORBIT_MULTIPLIER )
				angle += -dy / WS::Height * ROTATION_MULTIPLIER / 2.0f*m;
			else if (WS::xd > WS::Width * (1.0f - ORBIT_MULTIPLIER)) 
				angle += dy / WS::Height * ROTATION_MULTIPLIER / 2.0f*m;
			else if (WS::yd < WS::Height * ORBIT_MULTIPLIER)
				angle +=  dx / WS::Width * ROTATION_MULTIPLIER / 2.0f*m;
			else
				angle += -dx / WS::Width * ROTATION_MULTIPLIER /2.0f*m;
			qrotate(angle, raxis);
		}
		update();
		return;
	}

	if (WS::mouse_down == GLFW_MOUSE_BUTTON_MIDDLE) {
		if (is_perspective) {
			float mult = 1 * m_position.z / (float) WS::Width / 0.70710678 * m;
			m_position += vec3((WS::xc_1 - WS::xc) * mult, (WS::yc - WS::yc_1) * mult, 0.0f);
		}
		else {
			float mult = 4.0f / (float) WS::Width * m;
			m_position += vec3((WS::xc_1 - WS::xc) * mult, (WS::yc - WS::yc_1) * mult, 0.0f);
		}
		update();
		return;
	}
}

void Camera::mouse_wheel(CEvent e) {
	float m = 1.0f; if (WS::mods == 1) m = 0.1f; else if (WS::mods == 2) m = 10.0f;
	
	if (is_perspective) {
		m_position += vec3(0.0, 0.0, e.data.d(1) * ZOOM_MULTIPLIER * m);
		ortho_scale = m_position.z * 0.374f;
	}
	else {
		//float tst = ortho_scale + e.data.d[1] * ZOOM_MULTIPLIER * m;
		float tst = ortho_scale ;
		if (tst >= 1.0) {
			ortho_scale += e.data.d(1) * ZOOM_MULTIPLIER * m;
		}
		else {
			if(e.data.d(1) > 0)
			ortho_scale *= ((ORTHO_ZOOM_MULTIPLIER-1.0f) *m + 1.0f);
			else
				ortho_scale /= ((ORTHO_ZOOM_MULTIPLIER - 1.0f) * m + 1.0f);
		}
		m_position.z = ortho_scale / 0.374f;
	}
	update();
}

void Camera::mouse_click(CEvent e) {
	GLFWwindow* w = (GLFWwindow * ) e.p;
	d_position = m_position;
	d_quat = m_quat;
	angle = 0.0f;

	//if (e.data.i[0] == GLFW_MOUSE_BUTTON_LEFT && e.data.i[1] == GLFW_PRESS) {
	//	
	//}
	//if (e.data.i[0] == GLFW_MOUSE_BUTTON_LEFT && e.data.i[1] == GLFW_RELEASE) {
	//	
	//}
}

bool Camera::to2D(glm::vec3& in_pos3d, glm::vec2& out_pos2d)
{
	glm::vec3 screenPos = PVmat * glm::vec4(in_pos3d, 1.0f);
	
	if (is_perspective) {
		if (screenPos.z <= 0) return false;
		out_pos2d.x = screenPos.x / screenPos.z;
		out_pos2d.y = screenPos.y / screenPos.z;
		if (out_pos2d.x >= -1.0 && out_pos2d.x <= 1.0 &&
			out_pos2d.y >= -1.0 && out_pos2d.y <= 1.0)
		{
			out_pos2d.x = (out_pos2d.x + 1.0f) / 2.0f * (float)WS::Width;
			out_pos2d.y = (out_pos2d.y + 1.0f) / 2.0f * (float)WS::Height;
			return true;
		}
		return false;
	}
	else {
		out_pos2d.x = screenPos.x ;
		out_pos2d.y = screenPos.y ;
		if (out_pos2d.x >= -1.0f && out_pos2d.x <= 1.0f &&
			out_pos2d.y >= -1.0f && out_pos2d.y <= 1.0f)
		{
			out_pos2d.x = (out_pos2d.x + 1.0f) / (2.0f) * (float)WS::Width;
			out_pos2d.y = (out_pos2d.y + 1.0f) / (2.0f) * (float)WS::Height;
			return true;
		}
	}
	return false;
}

Camera::~Camera(){}

vec3 Camera::Position() const
{
	return m_position;
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

void Camera::addImguiControls(int imguiWID, std::string imguiItemName)
{
	auto ff = [this](CEvent e)
	{
		if (e.data.b(0)) this->is_perspective = false;
		else this->is_perspective = true;
		this->switchCameraMode(this->is_perspective);
		ES::needsUpdate();
	};
	ES::subscribe(ET::imgui_checkbox, ff, IS::addCheckbox(imguiWID, imguiItemName, false));
}

void Camera::switchCameraMode(bool perspective) {
	//_pl("before: ",m_position.z, " ", ortho_scale, " ", WS::Aspect);

	//float mult = 1 * m_position.z / 0.70710678/ 4.0f;
	float mult = 1;

	if (perspective) {

		//m_position.z = ortho_scale / 0.374f;
		//	ortho_scale = 1.0;

	}
	else {
		//ortho_scale = m_position.z * 0.374f;
		//m_position.z = 1.0;
	}
	_pl("after: ", m_position.z, " ", ortho_scale);
	update();
}
