#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

class Camera
{
public:
	glm::vec3 m_WorldUp;
	glm::vec3 m_Pos;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::mat4 m_Matrix;
	float     m_MovementSpeed{ 5.0f };
	float     m_Pitch{}, m_Yaw{ -90.0f };
	float     m_Zoom{ 10.0f };
	enum class MoveDirection
	{
		FRONT = 0, BACK, LEFT, RIGHT
	};
	enum class RotateDirection
	{
		UP = 0, DOWN, LEFT, RIGHT
	};
public:
	Camera(const glm::vec3& CameraPosition, const glm::vec3& CameraFront = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& WorldUp = glm::vec3(0.0f, 1.0f, 0.0f));
	void move(MoveDirection Direction, float dt);
	void rotate(RotateDirection Direction, float deg);
	void zoom(float ratio);
private:
	void updateVectors();
	void updateViewMatrix();
};

Camera::Camera(const glm::vec3& CameraPosition, const glm::vec3& CameraFront, const glm::vec3& WorldUp)
	:m_Pos(CameraPosition), m_Front(glm::normalize(CameraFront)), m_WorldUp(glm::normalize(WorldUp))
{
	updateVectors();
	updateViewMatrix();
};

void Camera::updateVectors()
{
	glm::vec3 direction{ 0.0f, 0.0f, -1.0f };
	direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	direction.y = sin(glm::radians(m_Pitch));
	direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(direction);
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
};

void Camera::move(MoveDirection Direction, float dt)
{
	switch (Direction)
	{
	case MoveDirection::FRONT: m_Pos += m_Front * m_MovementSpeed * dt; break;
	case MoveDirection::BACK : m_Pos -= m_Front * m_MovementSpeed * dt; break;
	case MoveDirection::LEFT : m_Pos -= m_Right * m_MovementSpeed * dt; break;
	case MoveDirection::RIGHT: m_Pos += m_Right * m_MovementSpeed * dt; break;
	}
	updateViewMatrix();
};

void Camera::rotate(RotateDirection Direction, float deg)
{
	switch (Direction)
	{
	case RotateDirection::UP:   m_Pitch += deg; break;
	case RotateDirection::DOWN: m_Pitch -= deg; break;
	case RotateDirection::LEFT: m_Yaw   -= deg; break;
	case RotateDirection::RIGHT:m_Yaw   += deg; break;
	}

	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;
	
	updateVectors();
	updateViewMatrix();
};

void Camera::zoom(float ratio)
{
	m_Zoom -= ratio;
	if (m_Zoom < 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom > 45.0f)
		m_Zoom = 45.0f;
};

void Camera::updateViewMatrix()
{
	glm::vec3 zaxis{ glm::normalize(-m_Front) };
	glm::vec3 xaxis{ m_Right };
	glm::vec3 yaxis{ m_Up };

	glm::mat4 translation = glm::translate(glm::mat4{ 1.0f }, -m_Pos);
	glm::mat4 rotation = glm::transpose(glm::mat4{ glm::vec4{xaxis, 0.0f} , glm::vec4{ yaxis, 0.0f }, glm::vec4{ zaxis, 0.0f }, glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f } });
	m_Matrix = rotation * translation;
};


#endif