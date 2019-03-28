#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Input.h"

using glm::vec3;
using glm::mat4;

class Camera
{
public:
	// Constructor that by default sets 
	Camera() : m_theta(0), m_phi(-20), m_position(-10, 4, 0) {}			 

	glm::mat4 GetProjectionMatrix(float w, float h);	// Returns a matrix 4 of the camera's projection where: float w = width of screen, float h = height of screen
	glm::mat4 GetViewMatrix();							// Returns a matrix 4 of where the camera is directed to and displaying
	
	void update();										// Updates camera member variables (mouse position and camera position and rotation)

private:
	float m_theta;			// The longitude angle – Theta is zero when you’re looking along the x-axis.
	float m_phi;			// The angle of elevation. If you’re looking horizontally, phi is zero. Looking down, phi becomes negative. Looking up, phi becomes positive.
	glm::vec3 m_position;

	int m_lastMouseX;
	int m_lastMouseY;

	// Convertion from degrees to radius
	const float deg2Rad = 3.14159f / 180.0f;
};

