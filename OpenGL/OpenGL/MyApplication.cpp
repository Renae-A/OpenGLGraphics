#include "MyApplication.h"
#include "Input.h"
#include <GLFW/glfw3.h>
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include "Shader.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

using namespace aie;

MyApplication::MyApplication()
{
	m_prevTime = glfwGetTime();
	m_currTime = 0;
	m_deltaTime = 0;
}

MyApplication::~MyApplication()
{
}

int MyApplication::startup()
{
	if (glfwInit() == false)
		return -1;
	m_window = glfwCreateWindow(1280, 720, "OpenGL", nullptr, nullptr);
	if (m_window == nullptr) {
		glfwTerminate();
		return -2;
	}
	glfwMakeContextCurrent(m_window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return -3;
	}

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow*, int w, int h) { glViewport(0, 0, w, h); });

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	aie::Gizmos::create(65336, 65336, 256, 256);

	aie::Input::create();

	// load vertex shader from file
	m_shader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/simple.vert");
	// load fragment shader from file
	m_shader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/simple.frag");
	if (m_shader.link() == false) {
		printf("Shader Error: %s\n", m_shader.getLastError());
	}

	if (m_gridTexture.load("./textures/numbered_grid.tga") == false) {
		printf("Failed to load texture!\n");
		return false;
	}

	m_quadMesh.initialiseQuad();

	m_quadTransform = {
		10, 0, 0, 0,
		0, 10, 0, 0,
		0, 0, 10, 0,
		0, 0, 0, 1		};

	//if (m_bunnyMesh.load("./stanford/bunny.obj") == false) {
	//	printf("Bunny Mesh Error!\n");
	//	return false;
	//}
	//m_bunnyTransform = {
	//	0.5f, 0, 0, 0,
	//	0, 0.5f, 0, 0,
	//	0, 0, 0.5f, 0,
	//	0, 0, 0, 1		};

	return 0;
}

void MyApplication::shutdown()
{
	Gizmos::destroy();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool MyApplication::update()
{
	Input::getInstance()->clearStatus();

	m_camera.update();

	// update deltaTime
	m_currTime = glfwGetTime();
	m_deltaTime = m_currTime - m_prevTime;
	m_prevTime = m_currTime;

	// our game logic and update code goes here!

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::clear();
	Gizmos::addTransform(glm::mat4(1));
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	// load imaginary texture
	aie::Texture texture1;
	texture1.load("mytexture.png");

	// create a 2x2 black-n-white checker texture
	// RED simply means one colour channel, i.e. grayscale
	aie::Texture texture2;
	unsigned char texelData[4] = { 0, 255, 255, 0 };
	texture2.create(2, 2, aie::Texture::RED, texelData);

	// bind shader
	//m_shader.bind();
	m_texturedShader.bind();

	// bind transform quad
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_quadTransform;
	//auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_bunnyTransform;
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);

	// bind texture location
	m_texturedShader.bindUniform("diffuseTexture", 0);
	// bind texture to specified location
	m_gridTexture.bind(0);

	// draw quad
	m_quadMesh.draw();
	//m_bunnyMesh.draw();

	Gizmos::draw(m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix());

	// so does our render code!
	glfwSwapBuffers(m_window);
	glfwPollEvents();
	
	return (glfwWindowShouldClose(m_window) == false && glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS);
}

unsigned int MyApplication::getWindowWidth()  
{
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	return width;
}

unsigned int MyApplication::getWindowHeight() 
{
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	return height;
}

glm::vec2 getScreenRes()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();

	GetWindowRect(hDesktop, &desktop);

	return glm::vec2(desktop.right, desktop.bottom);
}