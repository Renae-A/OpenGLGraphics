#pragma once
#include "gl_core_4_4.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"

class MyApplication
{
public:
	MyApplication();
	virtual ~MyApplication();

	int startup();
	void shutdown();
	bool update();

	unsigned int getWindowWidth();
	unsigned int getWindowHeight();

private:

	GLFWwindow*		m_window;
	Camera			m_camera;

	double			m_prevTime;
	double			m_currTime;
	double			m_deltaTime;

	aie::ShaderProgram	m_shader;
	aie::ShaderProgram	m_texturedShader;

	aie::Texture		m_gridTexture;

	Mesh				m_quadMesh;
	glm::mat4			m_quadTransform;
	aie::OBJMesh		m_bunnyMesh;
	glm::mat4			m_bunnyTransform;
};
