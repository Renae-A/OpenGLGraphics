#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Input.h"

using glm::vec3;
using glm::mat4;

class Camera
{
public:
	Camera() : m_theta(0), m_phi(-20), m_position(-10, 4, 0) {}

	glm::mat4 GetProjectionMatrix(float w, float h);
	glm::mat4 GetViewMatrix();
	
	void update();

private:
	float m_theta;
	float m_phi;
	glm::vec3 m_position;

	int m_lastMouseX;
	int m_lastMouseY;

	const float deg2Rad = 3.14159f / 180.0f;
};

