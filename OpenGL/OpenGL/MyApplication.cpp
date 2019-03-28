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

	// load vertex textured shader from file
	m_texturedShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/textured.vert");
	// load fragment textured shader from file
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/textured.frag");
	if (m_texturedShader.link() == false) {
		printf("Shader Error: %s\n", m_texturedShader.getLastError());
	}

	// load vertex phong shader from file
	m_phongShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/phong.vert");
	// load fragment phong shader from file
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/phong.frag");
	if (m_phongShader.link() == false) {
		printf("Shader Error: %s\n", m_phongShader.getLastError());
	}

	// load vertex normal map shader from file
	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/normalmap.vert");
	// load fragment normal map shader from file
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/normalmap.frag");
	if (m_normalMapShader.link() == false) {
		printf("Shader Error: %s\n", m_normalMapShader.getLastError());
	}

	// load vertex oren-nayar BRDF shader from file
	m_physicBasedShadar.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/physic-based.vert");
	// load fragment normal map shader from file
	m_physicBasedShadar.loadShader(aie::eShaderStage::FRAGMENT,
		"./shaders/physic-based.frag");
	if (m_physicBasedShadar.link() == false) {
		printf("Shader Error: %s\n", m_physicBasedShadar.getLastError());
	}


	// Grid texture
	if (m_gridTexture.load("./textures/numbered_grid.tga") == false) {
		printf("Failed to load texture!\n");
		return false;
	}

	// Denim texture
	if (m_denimTexture.load("./textures/denim-textures-3.jpg") == false) {
		printf("Failed to load texture!\n");
		return false;
	}

	// Popcorn texture
	if (m_popcornTexture.load("./textures/popcorn.jpg") == false) {
		printf("Failed to load texture!\n");
		return false;
	}

	// Carpet texture
	if (m_carpetTexture.load("./textures/carpet.jpg") == false) {
		printf("Failed to load texture!\n");
		return false;
	}

	// Starry night texture
	if (m_starrynightTexture.load("./textures/starrynight.jpg") == false) {
		printf("Failed to load texture!\n");
		return false;
	}

	// Lightning texture
	if (m_lightningTexture.load("./textures/lightning.png") == false) {
		printf("Failed to load texture!\n");
		return false;
	}

	// Tartan texture
	if (m_tartanTexture.load("./textures/tartan.jpg") == false) {
		printf("Failed to load texture!\n");
		return false;
	}

	if (m_renderTarget.initialise(1, getWindowWidth(),
		getWindowHeight()) == false) {
		printf("Render Target Error!\n");
		return false;
	}

	m_quadMesh.initialiseQuad();

	m_quadTransform = {
		10, 0, 0, 0,
		0, 10, 0, 0,
		0, 0, 10, 0,
		0, 0, 0, 1		};

	// Bunny -----------------------------------------------
	//if (m_bunnyMesh.load("./stanford/bunny.obj") == false) {
	//	printf("Bunny Mesh Error!\n");
	//	return false;
	//}

	m_bunnyTransform = {
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1		};

	// Dragon -----------------------------------------------
	if (m_dragonMesh.load("./stanford/dragon.obj") == false) {
		printf("Dragon Mesh Error!\n");
		return false;
	}

	m_dragonTransform = {
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1 };

	// Lucy ----------------------------------------------
	//if (m_lucyMesh.load("./stanford/lucy.obj") == false) {
	//	printf("Lucy Mesh Error!\n");
	//	return false;
	//}

	m_lucyTransform = {
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1 };

	// Buddha ------------------------------------------------
	if (m_buddhaMesh.load("./stanford/buddha.obj") == false) {
		printf("Buddha Mesh Error!\n");
		return false;
	}

	m_buddhaTransform = {
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1 };

	// Spear ---------------------------------------
	if (m_spearMesh.load("./soulspear/soulspear.obj",
		true, true) == false) {
		printf("Soulspear Mesh Error!\n");
		return false;
	}

	m_spearTransform = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1	};

	// Light settings
	m_light.diffuse = { 1, 1, 1 };
	m_light.specular = { 1, 1, 1 };
	m_ambientLight = { 0.25f, 0.25f, 0.25f };

	// Multiple Lights Settings
	m_pointLightPos[0] = vec3(5, 5, 5);
	m_pointLightPos[1] = vec3(-5, 5, 5);
	m_pointLightPos[2] = vec3(-5, 5, -5);
	m_pointLightPos[3] = vec3(5, 5, -5);
	m_lightColors[0] = vec3(1, 0, 0);
	m_lightColors[1] = vec3(0, 1, 0);
	m_lightColors[2] = vec3(0, 0, 1);
	m_lightColors[3] = vec3(1, 1, 0);
	m_lightPower[0] = 100.0f; 
	m_lightPower[1] = 100.0f;
	m_lightPower[2] = 100.0f;
	m_lightPower[3] = 100.0f;
	m_lightCount = 4;

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

	// Update deltaTime
	m_currTime = glfwGetTime();
	m_deltaTime = m_currTime - m_prevTime;
	m_prevTime = m_currTime;

	// Rotate light
	//m_light.direction = glm::normalize(vec3(glm::cos(m_currTime * 2), glm::sin(m_currTime * 2), 0));

	// Set up singular light
	m_light.direction = glm::normalize(glm::vec3(7.0f, -3.0f, 5.0f));

	// Draw 

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::clear();
	Gizmos::addTransform(glm::mat4(1));
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}

	for (int i = 0; i < m_lightCount; i++)
	{
		Gizmos::addSphere(m_pointLightPos[i], 1, 8, 8, vec4(m_lightColors[i], 1));
	}

	// ------------------- Simple shader for quad --------------------

	// Bind shader
	m_shader.bind();
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_quadTransform;
	m_shader.bindUniform("ProjectionViewModel", pvm);
	// Draw quad
	m_quadMesh.draw();

	// ------------------- Simple shader for bunny --------------------

	//// Bind shader
	//m_shader.bind();
	//// Bind transform
	//auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_bunnyTransform;
	//m_shader.bindUniform("ProjectionViewModel", pvm);
	//// Draw bunny
	//m_bunnyMesh.draw();

	// ------------------- Simple shader for dragon --------------------

	//// Bind shader
	//m_shader.bind();
	//// Bind transform
	//auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_dragonTransform;
	//m_shader.bindUniform("ProjectionViewModel", pvm);
	//// Draw dragon
	//m_dragonMesh.draw();

	// ------------------- Textured shader for quad --------------------

	//// Bind shader
	//m_texturedShader.bind();
	//// Bind transform
	//auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_quadTransform;
	//m_texturedShader.bindUniform("ProjectionViewModel", pvm);
	//// Bind texture location
	//m_texturedShader.bindUniform("diffuseTexture", 0);
	//// Bind texture to specified location
	//m_gridTexture.bind(0);
	//// Draw quad
	//m_quadMesh.draw();

	// ------------------- Textured shader for spear --------------------

	//// Bind shader
	//m_texturedShader.bind();
	//// Bind transform
	//auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_spearTransform;
	//m_texturedShader.bindUniform("ProjectionViewModel", pvm);
	//// Draw mesh
	//m_spearMesh.draw();

	// ------------------- Lighting with phong shader for quad (only works when bunny is also drawn?) --------------------

	//// bind phong shader program
	//m_phongShader.bind();
	//// bind light
	//m_phongShader.bindUniform("Ia", m_ambientLight);
	//m_phongShader.bindUniform("Id", m_light.diffuse);
	//m_phongShader.bindUniform("Is", m_light.specular);
	//m_phongShader.bindUniform("LightDirection", m_light.direction);
	//// bind transform
	//auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_quadTransform;
	//m_phongShader.bindUniform("ProjectionViewModel", pvm);
	//// bind transforms for lighting
	//m_phongShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_quadTransform)));
	//// draw quad
	//m_quadMesh.draw();

	// ------------------- Lighting with phong shader for bunny --------------------

	//// bind phong shader program
	//m_phongShader.bind();
	//// bind light
	//m_phongShader.bindUniform("Ia", m_ambientLight);
	//m_phongShader.bindUniform("Id", m_light.diffuse);
	//m_phongShader.bindUniform("Is", m_light.specular);
	//m_phongShader.bindUniform("LightDirection", m_light.direction);
	//// bind transform
	//auto pvm2 = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_bunnyTransform;
	//m_phongShader.bindUniform("ProjectionViewModel", pvm2);
	//// bind transforms for lighting
	//m_phongShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_bunnyTransform)));
	//// draw bunny
	//m_bunnyMesh.draw();

	// ------------------- Lighting with phong shader for dragon --------------------

	//// bind phong shader program
	//m_phongShader.bind();
	//// bind light
	//m_phongShader.bindUniform("Ia", m_ambientLight);
	//m_phongShader.bindUniform("Id", m_light.diffuse);
	//m_phongShader.bindUniform("Is", m_light.specular);
	//m_phongShader.bindUniform("LightDirection", m_light.direction);
	//// bind transform
	//auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_dragonTransform;
	//m_phongShader.bindUniform("ProjectionViewModel", pvm);
	//// bind transforms for lighting
	//m_phongShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_dragonTransform)));
	//// draw dragon
	//m_dragonMesh.draw();

	// ------------------- Lighting with phong shader for lucy --------------------

	//// bind phong shader program
	//m_phongShader.bind();
	//// bind light
	//m_phongShader.bindUniform("Ia", m_ambientLight);
	//m_phongShader.bindUniform("Id", m_light.diffuse);
	//m_phongShader.bindUniform("Is", m_light.specular);
	//m_phongShader.bindUniform("LightDirection", m_light.direction);
	//// bind transform
	//auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_lucyTransform;
	//m_phongShader.bindUniform("ProjectionViewModel", pvm);
	//// bind transforms for lighting
	//m_phongShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_lucyTransform)));
	//// draw lucy
	//m_lucyMesh.draw();

	// ------------------- Multiple lighting with phong shader for buddha --------------------

	//// bind phong shader program
	//m_phongShader.bind();
	//// bind light
	//m_phongShader.bindUniform("Ia", m_ambientLight);

	//m_phongShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	//m_phongShader.bindUniform("m_lightCount", m_lightCount);
	//m_phongShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	//m_phongShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	//m_phongShader.bindUniform("specularPower", 0.5f);

	//// bind transform
	//auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_buddhaTransform;
	//m_phongShader.bindUniform("ProjectionViewModel", pvm);
	//// bind transforms for lighting
	//m_phongShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_buddhaTransform)));
	//m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	//// draw buddha
	//m_buddhaMesh.draw();

	// ------------------- Normal map shader for spear (make changes within .frag file to change colours to normal, etc.) -------------------- 

	//// Bind shader
	//m_normalMapShader.bind();
	//// Bind light 
	//m_normalMapShader.bindUniform("Ia", m_ambientLight);
	//m_normalMapShader.bindUniform("Id", m_light.diffuse);
	//m_normalMapShader.bindUniform("Is", m_light.specular);
	//m_normalMapShader.bindUniform("LightDirection", m_light.direction);
	//// Bind transform
	//auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_spearTransform;
	//m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	//m_normalMapShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_spearTransform)));
	//// Bind camera
	//m_normalMapShader.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	//// Draw spear
	//m_spearMesh.draw();

	// ------------------- Phong shader with texture for dragon -------------------- 

	//// Bind shader
	//m_phongShader.bind();
	//// Bind light 
	//m_phongShader.bindUniform("Ia", m_ambientLight);
	//m_phongShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	//m_phongShader.bindUniform("m_lightCount", m_lightCount);
	//m_phongShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	//m_phongShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	//m_phongShader.bindUniform("specularPower", 0.5f);
	//// Bind transform
	//auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_dragonTransform;
	//m_phongShader.bindUniform("ProjectionViewModel", pvm);
	//m_phongShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_dragonTransform)));
	//// Bind camera
	//m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	//// Bind texture location
	//m_phongShader.bindUniform("diffuseTex", 0);
	//// Bind texture to specified location
	//m_denimTexture.bind(0);
	//// Draw dragon
	//m_dragonMesh.draw();

	// ------------------- (Shiny look) Oren-Nayar BRDF for dragon -------------------- 

	//// Bind Oren-Nayar BDRF shader program
	//m_physicBasedShadar.bind();
	//// bind light
	//m_physicBasedShadar.bindUniform("Ia", m_ambientLight);

	//m_physicBasedShadar.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	//m_physicBasedShadar.bindUniform("m_lightCount", m_lightCount);
	//m_physicBasedShadar.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	//m_physicBasedShadar.bindUniform("m_lightPower", 4, &m_lightPower[0]);

	//m_physicBasedShadar.bindUniform("Roughness", 0.05f);
	//m_physicBasedShadar.bindUniform("ReflectionCoefficient", 0.5f);
	//// bind transform
	//auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_dragonTransform;
	//m_physicBasedShadar.bindUniform("ProjectionViewModel", pvm);
	//// bind transforms for lighting
	//m_physicBasedShadar.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_dragonTransform)));
	//// Bind camera
	//m_physicBasedShadar.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	//// draw dragon
	//m_dragonMesh.draw();

	// ------------------- Render Target -------------------- 

	//// bind our render target
	//m_renderTarget.bind();

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//// Bind Oren-Nayar BDRF shader program
	//m_physicBasedShadar.bind();

	//// bind light
	//m_physicBasedShadar.bindUniform("Ia", m_ambientLight);

	//m_physicBasedShadar.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	//m_physicBasedShadar.bindUniform("m_lightCount", m_lightCount);
	//m_physicBasedShadar.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	//m_physicBasedShadar.bindUniform("m_lightPower", 4, &m_lightPower[0]);

	//m_physicBasedShadar.bindUniform("Roughness", 0.05f);
	//m_physicBasedShadar.bindUniform("ReflectionCoefficient", 0.5f);

	//// bind transform
	//auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_dragonTransform;
	//m_physicBasedShadar.bindUniform("ProjectionViewModel", pvm);

	//// bind transforms for lighting
	//m_physicBasedShadar.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_dragonTransform)));

	//// Bind camera
	//m_physicBasedShadar.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));

	//// draw dragon
	//m_dragonMesh.draw();

	//// unbind target to return to backbuffer
	//m_renderTarget.unbind();

	//// clear the backbuffer
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//// bind texturing shader
	//m_texturedShader.bind();
	//pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_quadTransform;
	//m_texturedShader.bindUniform("ProjectionViewModel", pvm);
	//m_texturedShader.bindUniform("diffuseTexture", 0);
	//m_renderTarget.getTarget(0).bind(0);

	//// draw quad
	//m_quadMesh.draw();

	Gizmos::draw(m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix());

	// So does our render code!
	glfwSwapBuffers(m_window);
	glfwPollEvents();
	
	return (glfwWindowShouldClose(m_window) == false && glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS);
}

// Returns the width of the screen
unsigned int MyApplication::getWindowWidth()  
{
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	return width;
}

// Returns the height of the screen
unsigned int MyApplication::getWindowHeight() 
{
	int width = 0, height = 0;
	glfwGetWindowSize(m_window, &width, &height);
	return height;
}

