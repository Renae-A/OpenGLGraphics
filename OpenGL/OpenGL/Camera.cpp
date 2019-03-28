#include "Camera.h"

using namespace aie;

// Returns a matrix 4 of the camera's projection where: float w = width of screen, float h = height of screen
glm::mat4 Camera::GetProjectionMatrix(float w, float h)
{
	return glm::perspective(glm::pi<float>() * 0.25f, w / h, 0.1f, 1000.f);
}

// Returns a matrix 4 of where the camera is directed to and displaying
glm::mat4 Camera::GetViewMatrix()
{
	float thetaR = m_theta * deg2Rad;
	float phiR = m_phi * deg2Rad;
	glm::vec3 forward(cos(phiR)*cos(thetaR), sin(phiR), cos(phiR)*sin(thetaR));
	return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

// Updates camera member variables (mouse position and camera position and rotation)
void Camera::update()
{
	aie::Input* input = aie::Input::getInstance();
	float thetaR = m_theta * deg2Rad;
	float phiR = m_phi * deg2Rad;

	// Calculate the forwards and right axes and up axis for the camera
	glm::vec3 forward(cos(phiR)*cos(thetaR), sin(phiR), cos(phiR)*sin(thetaR));
	glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
	glm::vec3 up(0, 1, 0);

	float deltaTime = 0.1f;

	// Use WASD, ZX keys to move camera around
	if (input->isKeyDown(aie::INPUT_KEY_X))
		m_position += up * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_Z))
		m_position += -up * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_A))
		m_position += -right * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_D))
		m_position += right * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_W))
		m_position += forward * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_S))
		m_position += -forward * deltaTime;


	// Get the current mouse coordinates
	float mx = input->getMouseX();
	float my = input->getMouseY();
	// If the right button is down, increment theta and phi
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		m_theta += 0.05f * (mx - m_lastMouseX);
		m_phi -= 0.05f * (my - m_lastMouseY);

	}
	// Store this frames values for next frame
	m_lastMouseX = mx;
	m_lastMouseY = my;
}
