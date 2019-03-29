#include "MyApplication.h"
#include "Input.h"
#include <GLFW/glfw3.h>
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include "Shader.h"
#include <imgui.h>
#include <imgui_glfw3.h>

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

	// imgui
	ImGui_Init(m_window, true);

	loadShaders();

	loadTextures();

	intialiseRenderTarget();

	m_quadMesh.initialiseQuad();

	setUpTransforms();

	loadStanfordModels();

	setUpLighting();

	return 0;
}

void MyApplication::shutdown()
{
	ImGui_Shutdown();
	Gizmos::destroy();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool MyApplication::update()
{

	Input::getInstance()->clearStatus();

	m_camera.update();

	updateTime();

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

	
	// clear imgui
	ImGui_NewFrame();
	//ImGui::ShowTestWindow();
	IMGUITools();		

	checkIMGUIValues();

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

	// ------------------- Lighting with phong shader for bunny --------------------

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
	//m_phongShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	//m_phongShader.bindUniform("m_lightCount", m_lightCount);
	//m_phongShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	//m_phongShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	//m_phongShader.bindUniform("specularPower", 0.5f);
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
	//m_phongShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	//m_phongShader.bindUniform("m_lightCount", m_lightCount);
	//m_phongShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	//m_phongShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	//m_phongShader.bindUniform("specularPower", 0.5f);
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
	//m_normalMapShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	//m_normalMapShader.bindUniform("m_lightCount", m_lightCount);
	//m_normalMapShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	//m_normalMapShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	//m_normalMapShader.bindUniform("specularPower", 0.5f);
	//// Bind transform
	//auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_spearTransform;
	//m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	//m_normalMapShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_spearTransform)));
	//// Bind camera
	//m_normalMapShader.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	//// Draw spear
	//m_spearMesh.draw();



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

	// draw IMGUI last
	ImGui::Render();

	// So does our render code!
	glfwSwapBuffers(m_window);
	glfwPollEvents();
	
	return (glfwWindowShouldClose(m_window) == false && glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS);
}

void MyApplication::loadShaders()
{
	// load vertex simple shader from file
	m_shader.loadShader(aie::eShaderStage::VERTEX,
		"./shaders/simple.vert");
	// load fragment simple shader from file
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
}

bool MyApplication::loadTextures()
{
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

	// Carpet texture
	if (m_carpetTexture.load("./textures/carpet.jpg") == false) {
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

	return true;
}

bool MyApplication::intialiseRenderTarget()
{
	if (m_renderTarget.initialise(1, getWindowWidth(),
		getWindowHeight()) == false) {
		printf("Render Target Error!\n");
		return false;
	}

	return true;
}

void MyApplication::setUpTransforms()
{
	m_quadTransform = {
		10, 0, 0, 0,
		0, 10, 0, 0,
		0, 0, 10, 0,
		0, 0, 0, 1 };

	m_bunnyTransform = {
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1 };

	m_dragonTransform = {
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1 };

	m_lucyTransform = {
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1 };

	m_buddhaTransform = {
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1 };

	m_spearTransform = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };
}

bool MyApplication::loadStanfordModels()
{
	// Bunny -----------------------------------------------
	if (m_bunnyMesh.load("./stanford/bunny.obj") == false) {
		printf("Bunny Mesh Error!\n");
		return false;
	}

	// Dragon -----------------------------------------------
	if (m_dragonMesh.load("./stanford/dragon.obj") == false) {
		printf("Dragon Mesh Error!\n");
		return false;
	}

	// Lucy ----------------------------------------------
	if (m_lucyMesh.load("./stanford/lucy.obj") == false) {
		printf("Lucy Mesh Error!\n");
		return false;
	}

	// Buddha ------------------------------------------------
	if (m_buddhaMesh.load("./stanford/buddha.obj") == false) {
		printf("Buddha Mesh Error!\n");
		return false;
	}

	// Spear ---------------------------------------
	if (m_spearMesh.load("./soulspear/soulspear.obj",
		true, true) == false) {
		printf("Soulspear Mesh Error!\n");
		return false;
	}

	return true;
}

void MyApplication::setUpLighting()
{
	// Light settings
	m_ambientLight = { 0.25f, 0.25f, 0.25f };

	// Multiple Lights Settings

	// Positions
	m_pointLightPos[0] = vec3(5, 5, 5);
	m_pointLightPos[1] = vec3(-5, 5, 5);
	m_pointLightPos[2] = vec3(-5, 5, -5);
	m_pointLightPos[3] = vec3(5, 5, -5);

	// Intensity
	m_lightPower[0] = 100.0f;
	m_lightPower[1] = 100.0f;
	m_lightPower[2] = 100.0f;
	m_lightPower[3] = 100.0f;

	// Number of lights
	m_lightCount = 4;
}

void MyApplication::updateTime()
{
	// Update deltaTime
	m_currTime = glfwGetTime();
	m_deltaTime = m_currTime - m_prevTime;
	m_prevTime = m_currTime;
}

void MyApplication::checkIMGUIValues()
{
	updateLighting();

	if (imgui_renderTarget == 0)
	{
		switch (imgui_shader)
		{
		case 0:	// If simple
			switch (imgui_model)
			{
			case 0:	// If quad
				simpleShaderQuad();
				break;
			case 1:	// If bunny
				simpleShaderBunny();
				break;
			case 2:	// If dragon
				simpleShaderDragon();
				break;
			case 3:	// If buddha
				simpleShaderBuddha();
				break;
			case 4:	// If lucy
				simpleShaderLucy();
				break;
			case 5:	// If spear
				simpleShaderSpear();
				break;
			}
			break;
		case 1: // If textured
			switch (imgui_model)
			{
			case 0:	// If quad
				texturedShaderQuad();
				break;
			case 1:	// If bunny
				texturedShaderBunny();
				break;
			case 2:	// If dragon
				texturedShaderDragon();
				break;
			case 3:	// If buddha
				texturedShaderBuddha();
				break;
			case 4:	// If lucy
				texturedShaderLucy();
				break;
			case 5:	// If spear
				texturedShaderSpear();
				break;
			}
			break;
		case 2: // If phong
			switch (imgui_model)
			{
			case 0:	// If quad
				phongShaderQuad();
				break;
			case 1:	// If bunny
				phongShaderBunny();
				break;
			case 2:	// If dragon
				phongShaderDragon();
				break;
			case 3:	// If buddha
				phongShaderBuddha();
				break;
			case 4:	// If lucy
				phongShaderLucy();
				break;
			case 5:	// If spear
				phongShaderSpear();
				break;
			}
			break;
		case 3: // If normal map
			switch (imgui_model)
			{
			case 0:	// If quad
				normalMapShaderQuad();
				break;
			case 1:	// If bunny
				normalMapShaderBunny();
				break;
			case 2:	// If dragon
				normalMapShaderDragon();
				break;
			case 3:	// If buddha
				normalMapShaderBuddha();
				break;
			case 4:	// If lucy
				normalMapShaderLucy();
				break;
			case 5:	// If spear
				normalMapShaderSpear();
				break;
			}
			break;
		case 4: // If physics based
			switch (imgui_model)
			{
			case 0:	// If quad
				physicsBasedShaderQuad();
				break;
			case 1:	// If bunny
				physicsBasedShaderBunny();
				break;
			case 2:	// If dragon
				physicsBasedShaderDragon();
				break;
			case 3:	// If buddha
				physicsBasedShaderBuddha();
				break;
			case 4:	// If lucy
				physicsBasedShaderLucy();
				break;
			case 5:	// If spear
				physicsBasedShaderSpear();
				break;
			}
			break;
		}
			
	}
}

void MyApplication::updateLighting()
{
	if (imgui_light1 == 0)
		m_lightColors[0] = vec3(1, 1, 1);
	if (imgui_light1 == 1)
		m_lightColors[0] = vec3(1, 0, 0);
	if (imgui_light1 == 2)
		m_lightColors[0] = vec3(1, 0.5f, 0);
	if (imgui_light1 == 3)
		m_lightColors[0] = vec3(1, 1, 0);
	if (imgui_light1 == 4)
		m_lightColors[0] = vec3(0, 1, 0);
	if (imgui_light1 == 5)
		m_lightColors[0] = vec3(0, 0, 1);
	if (imgui_light1 == 6)
		m_lightColors[0] = vec3(1, 0, 1);

	if (imgui_light2 == 0)
		m_lightColors[1] = vec3(1, 1, 1);
	if (imgui_light2 == 1)
		m_lightColors[1] = vec3(1, 0, 0);
	if (imgui_light2 == 2)
		m_lightColors[1] = vec3(1, 0.5f, 0);
	if (imgui_light2 == 3)
		m_lightColors[1] = vec3(1, 1, 0);
	if (imgui_light2 == 4)
		m_lightColors[1] = vec3(0, 1, 0);
	if (imgui_light2 == 5)
		m_lightColors[1] = vec3(0, 0, 1);
	if (imgui_light2 == 6)
		m_lightColors[1] = vec3(1, 0, 1);


	if (imgui_light3 == 0)
		m_lightColors[2] = vec3(1, 1, 1);
	if (imgui_light3 == 1)
		m_lightColors[2] = vec3(1, 0, 0);
	if (imgui_light3 == 2)
		m_lightColors[2] = vec3(1, 0.5f, 0);
	if (imgui_light3 == 3)
		m_lightColors[2] = vec3(1, 1, 0);
	if (imgui_light3 == 4)
		m_lightColors[2] = vec3(0, 1, 0);
	if (imgui_light3 == 5)
		m_lightColors[2] = vec3(0, 0, 1);
	if (imgui_light3 == 6)
		m_lightColors[2] = vec3(1, 0, 1);


	if (imgui_light4 == 0)
		m_lightColors[3] = vec3(1, 1, 1);
	if (imgui_light4 == 1)
		m_lightColors[3] = vec3(1, 0, 0);
	if (imgui_light4 == 2)
		m_lightColors[3] = vec3(1, 0.5f, 0);
	if (imgui_light4 == 3)
		m_lightColors[3] = vec3(1, 1, 0);
	if (imgui_light4 == 4)
		m_lightColors[3] = vec3(0, 1, 0);
	if (imgui_light4 == 5)
		m_lightColors[3] = vec3(0, 0, 1);
	if (imgui_light4 == 6)
		m_lightColors[3] = vec3(1, 0, 1);
}

void MyApplication::simpleShaderQuad()
{
	// Bind shader
	m_shader.bind();
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_quadTransform;
	m_shader.bindUniform("ProjectionViewModel", pvm);
	// Draw quad
	m_quadMesh.draw();
}

void MyApplication::simpleShaderBunny()
{
	// Bind shader
	m_shader.bind();
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_bunnyTransform;
	m_shader.bindUniform("ProjectionViewModel", pvm);
	// Draw bunny
	m_bunnyMesh.draw();
}

void MyApplication::simpleShaderDragon()
{
	// Bind shader
	m_shader.bind();
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_dragonTransform;
	m_shader.bindUniform("ProjectionViewModel", pvm);
	// Draw dragon
	m_dragonMesh.draw();
}

void MyApplication::simpleShaderBuddha()
{
	// Bind shader
	m_shader.bind();
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_buddhaTransform;
	m_shader.bindUniform("ProjectionViewModel", pvm);
	// Draw buddha
	m_buddhaMesh.draw();
}

void MyApplication::simpleShaderLucy()
{
	// Bind shader
	m_shader.bind();
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_lucyTransform;
	m_shader.bindUniform("ProjectionViewModel", pvm);
	// Draw lucy
	m_lucyMesh.draw();
}

void MyApplication::simpleShaderSpear()
{
	// Bind shader
	m_shader.bind();
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_spearTransform;
	m_shader.bindUniform("ProjectionViewModel", pvm);
	// Draw spear
	m_spearMesh.draw();
}

void MyApplication::texturedShaderQuad()
{
	// Bind shader
	m_texturedShader.bind();
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_quadTransform;
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);
	// Bind texture location
	m_texturedShader.bindUniform("diffuseTexture", 0);
	// Bind texture to specified location
	m_gridTexture.bind(0);
	// Draw quad
	m_quadMesh.draw();
}

void MyApplication::texturedShaderBunny()
{
	// Bind shader
	m_texturedShader.bind();
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_bunnyTransform;
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);
	// Bind texture location
	m_texturedShader.bindUniform("diffuseTexture", 0);
	// Bind texture to specified location
	m_gridTexture.bind(0);
	// Draw bunny
	m_bunnyMesh.draw();
}

void MyApplication::texturedShaderDragon()
{
	// Bind shader
	m_texturedShader.bind();
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_dragonTransform;
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);
	// Bind texture location
	m_texturedShader.bindUniform("diffuseTexture", 0);
	// Bind texture to specified location
	m_gridTexture.bind(0);
	// Draw dragon
	m_dragonMesh.draw();
}

void MyApplication::texturedShaderBuddha()
{
	// Bind shader
	m_texturedShader.bind();
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_buddhaTransform;
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);
	// Bind texture location
	m_texturedShader.bindUniform("diffuseTexture", 0);
	// Bind texture to specified location
	m_gridTexture.bind(0);
	// Draw buddha
	m_buddhaMesh.draw();
}

void MyApplication::texturedShaderLucy()
{
	// Bind shader
	m_texturedShader.bind();
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_lucyTransform;
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);
	// Bind texture location
	m_texturedShader.bindUniform("diffuseTexture", 0);
	// Bind texture to specified location
	m_gridTexture.bind(0);
	// Draw lucy
	m_lucyMesh.draw();
}

void MyApplication::texturedShaderSpear()
{
	// Bind shader
	m_texturedShader.bind();
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_spearTransform;
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);
	// Draw mesh
	m_spearMesh.draw();
}

void MyApplication::phongShaderQuad()
{
	// bind phong shader program
	m_phongShader.bind();
	// bind light
	m_phongShader.bindUniform("Ia", m_ambientLight);

	m_phongShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_phongShader.bindUniform("m_lightCount", m_lightCount);
	m_phongShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_phongShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	m_phongShader.bindUniform("specularPower", 0.5f);

	// bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_quadTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);
	// bind transforms for lighting
	m_phongShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_quadTransform)));
	m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	// draw quad
	m_quadMesh.draw();
}

void MyApplication::phongShaderBunny()
{
	// bind phong shader program
	m_phongShader.bind();
	// bind light
	m_phongShader.bindUniform("Ia", m_ambientLight);

	m_phongShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_phongShader.bindUniform("m_lightCount", m_lightCount);
	m_phongShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_phongShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	m_phongShader.bindUniform("specularPower", 0.5f);

	// bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_bunnyTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);
	// bind transforms for lighting
	m_phongShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_bunnyTransform)));
	m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	// draw bunny
	m_bunnyMesh.draw();
}

void MyApplication::phongShaderDragon()
{
	// bind phong shader program
	m_phongShader.bind();
	// bind light
	m_phongShader.bindUniform("Ia", m_ambientLight);

	m_phongShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_phongShader.bindUniform("m_lightCount", m_lightCount);
	m_phongShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_phongShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	m_phongShader.bindUniform("specularPower", 0.5f);

	// bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_dragonTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);
	// bind transforms for lighting
	m_phongShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_dragonTransform)));
	m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	// draw dragon
	m_dragonMesh.draw();
}

void MyApplication::phongShaderBuddha()
{
	// bind phong shader program
	m_phongShader.bind();
	// bind light
	m_phongShader.bindUniform("Ia", m_ambientLight);
	
	m_phongShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_phongShader.bindUniform("m_lightCount", m_lightCount);
	m_phongShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_phongShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	m_phongShader.bindUniform("specularPower", 0.5f);
	
	// bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_buddhaTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);
	// bind transforms for lighting
	m_phongShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_buddhaTransform)));
	m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	// draw buddha
	m_buddhaMesh.draw();
}

void MyApplication::phongShaderLucy()
{
	// bind phong shader program
	m_phongShader.bind();
	// bind light
	m_phongShader.bindUniform("Ia", m_ambientLight);

	m_phongShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_phongShader.bindUniform("m_lightCount", m_lightCount);
	m_phongShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_phongShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	m_phongShader.bindUniform("specularPower", 0.5f);

	// bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_lucyTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);
	// bind transforms for lighting
	m_phongShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_lucyTransform)));
	m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	// draw lucy
	m_lucyMesh.draw();
}

void MyApplication::phongShaderSpear()
{
	// bind phong shader program
	m_phongShader.bind();
	// bind light
	m_phongShader.bindUniform("Ia", m_ambientLight);

	m_phongShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_phongShader.bindUniform("m_lightCount", m_lightCount);
	m_phongShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_phongShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	m_phongShader.bindUniform("specularPower", 0.5f);

	// bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_spearTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);
	// bind transforms for lighting
	m_phongShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_spearTransform)));
	m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	// draw spear
	m_spearMesh.draw();
}

void MyApplication::normalMapShaderQuad()
{
	// Bind shader
	m_normalMapShader.bind();
	// Bind light 
	m_normalMapShader.bindUniform("Ia", m_ambientLight);
	m_normalMapShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_normalMapShader.bindUniform("m_lightCount", m_lightCount);
	m_normalMapShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_normalMapShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	m_normalMapShader.bindUniform("specularPower", 0.5f);
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_quadTransform;
	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	m_normalMapShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_quadTransform)));
	// Bind camera
	m_normalMapShader.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	// Draw quad
	m_quadMesh.draw();
}

void MyApplication::normalMapShaderBunny()
{
	// Bind shader
	m_normalMapShader.bind();
	// Bind light 
	m_normalMapShader.bindUniform("Ia", m_ambientLight);
	m_normalMapShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_normalMapShader.bindUniform("m_lightCount", m_lightCount);
	m_normalMapShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_normalMapShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	m_normalMapShader.bindUniform("specularPower", 0.5f);
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_bunnyTransform;
	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	m_normalMapShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_bunnyTransform)));
	// Bind camera
	m_normalMapShader.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	// Draw bunny
	m_bunnyMesh.draw();
}

void MyApplication::normalMapShaderDragon()
{
	// Bind shader
	m_normalMapShader.bind();
	// Bind light 
	m_normalMapShader.bindUniform("Ia", m_ambientLight);
	m_normalMapShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_normalMapShader.bindUniform("m_lightCount", m_lightCount);
	m_normalMapShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_normalMapShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	m_normalMapShader.bindUniform("specularPower", 0.5f);
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_dragonTransform;
	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	m_normalMapShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_dragonTransform)));
	// Bind camera
	m_normalMapShader.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	// Draw dragon
	m_dragonMesh.draw();
}

void MyApplication::normalMapShaderBuddha()
{
	// Bind shader
	m_normalMapShader.bind();
	// Bind light 
	m_normalMapShader.bindUniform("Ia", m_ambientLight);
	m_normalMapShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_normalMapShader.bindUniform("m_lightCount", m_lightCount);
	m_normalMapShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_normalMapShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	m_normalMapShader.bindUniform("specularPower", 0.5f);
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_buddhaTransform;
	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	m_normalMapShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_buddhaTransform)));
	// Bind camera
	m_normalMapShader.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	// Draw buddha
	m_buddhaMesh.draw();
}

void MyApplication::normalMapShaderLucy()
{
	// Bind shader
	m_normalMapShader.bind();
	// Bind light 
	m_normalMapShader.bindUniform("Ia", m_ambientLight);
	m_normalMapShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_normalMapShader.bindUniform("m_lightCount", m_lightCount);
	m_normalMapShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_normalMapShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	m_normalMapShader.bindUniform("specularPower", 0.5f);
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_lucyTransform;
	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	m_normalMapShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_lucyTransform)));
	// Bind camera
	m_normalMapShader.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	// Draw lucy
	m_lucyMesh.draw();
}

void MyApplication::normalMapShaderSpear()
{
	// Bind shader
	m_normalMapShader.bind();
	// Bind light 
	m_normalMapShader.bindUniform("Ia", m_ambientLight);
	m_normalMapShader.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_normalMapShader.bindUniform("m_lightCount", m_lightCount);
	m_normalMapShader.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_normalMapShader.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	m_normalMapShader.bindUniform("specularPower", 0.5f);
	// Bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_spearTransform;
	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	m_normalMapShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_spearTransform)));
	// Bind camera
	m_normalMapShader.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	// Draw spear
	m_spearMesh.draw();
}

void MyApplication::physicsBasedShaderQuad()
{
	// Bind Oren-Nayar BDRF shader program
	m_physicBasedShadar.bind();
	// bind light
	m_physicBasedShadar.bindUniform("Ia", m_ambientLight);

	m_physicBasedShadar.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_physicBasedShadar.bindUniform("m_lightCount", m_lightCount);
	m_physicBasedShadar.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_physicBasedShadar.bindUniform("m_lightPower", 4, &m_lightPower[0]);

	m_physicBasedShadar.bindUniform("Roughness", 0.05f);
	m_physicBasedShadar.bindUniform("ReflectionCoefficient", 0.5f);
	// bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_quadTransform;
	m_physicBasedShadar.bindUniform("ProjectionViewModel", pvm);
	// bind transforms for lighting
	m_physicBasedShadar.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_quadTransform)));
	// Bind camera
	m_physicBasedShadar.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));

	m_physicBasedShadar.bindUniform("diffuseTex", 0);
	// Bind texture to specified location
	m_gridTexture.bind(0);

	// Draw quad
	m_quadMesh.draw();
}

void MyApplication::physicsBasedShaderBunny()
{
	// Bind Oren-Nayar BDRF shader program
	m_physicBasedShadar.bind();
	// bind light
	m_physicBasedShadar.bindUniform("Ia", m_ambientLight);

	m_physicBasedShadar.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_physicBasedShadar.bindUniform("m_lightCount", m_lightCount);
	m_physicBasedShadar.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_physicBasedShadar.bindUniform("m_lightPower", 4, &m_lightPower[0]);

	m_physicBasedShadar.bindUniform("Roughness", 0.05f);
	m_physicBasedShadar.bindUniform("ReflectionCoefficient", 0.5f);
	// bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_bunnyTransform;
	m_physicBasedShadar.bindUniform("ProjectionViewModel", pvm);
	// bind transforms for lighting
	m_physicBasedShadar.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_bunnyTransform)));
	// Bind camera
	m_physicBasedShadar.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));

	m_physicBasedShadar.bindUniform("diffuseTex", 0);
	// Bind texture to specified location
	m_gridTexture.bind(0);

	// draw bunny
	m_bunnyMesh.draw();
}

void MyApplication::physicsBasedShaderDragon()
{
	// Bind Oren-Nayar BDRF shader program
	m_physicBasedShadar.bind();
	// bind light
	m_physicBasedShadar.bindUniform("Ia", m_ambientLight);
	
	m_physicBasedShadar.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_physicBasedShadar.bindUniform("m_lightCount", m_lightCount);
	m_physicBasedShadar.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_physicBasedShadar.bindUniform("m_lightPower", 4, &m_lightPower[0]);
	
	m_physicBasedShadar.bindUniform("Roughness", 0.05f);
	m_physicBasedShadar.bindUniform("ReflectionCoefficient", 0.5f);
	// bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_dragonTransform;
	m_physicBasedShadar.bindUniform("ProjectionViewModel", pvm);
	// bind transforms for lighting
	m_physicBasedShadar.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_dragonTransform)));
	// Bind camera
	m_physicBasedShadar.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));

	m_physicBasedShadar.bindUniform("diffuseTex", 0);
	// Bind texture to specified location
	m_gridTexture.bind(0);

	// draw dragon
	m_dragonMesh.draw();
}

void MyApplication::physicsBasedShaderBuddha()
{
	// Bind Oren-Nayar BDRF shader program
	m_physicBasedShadar.bind();
	// bind light
	m_physicBasedShadar.bindUniform("Ia", m_ambientLight);

	m_physicBasedShadar.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_physicBasedShadar.bindUniform("m_lightCount", m_lightCount);
	m_physicBasedShadar.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_physicBasedShadar.bindUniform("m_lightPower", 4, &m_lightPower[0]);

	m_physicBasedShadar.bindUniform("Roughness", 0.05f);
	m_physicBasedShadar.bindUniform("ReflectionCoefficient", 0.5f);
	// bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_buddhaTransform;
	m_physicBasedShadar.bindUniform("ProjectionViewModel", pvm);
	// bind transforms for lighting
	m_physicBasedShadar.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_buddhaTransform)));
	// Bind camera
	m_physicBasedShadar.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));

	m_physicBasedShadar.bindUniform("diffuseTex", 0);
	// Bind texture to specified location
	m_gridTexture.bind(0);

	// draw buddha
	m_buddhaMesh.draw();
}

void MyApplication::physicsBasedShaderLucy()
{
	// Bind Oren-Nayar BDRF shader program
	m_physicBasedShadar.bind();
	// bind light
	m_physicBasedShadar.bindUniform("Ia", m_ambientLight);

	m_physicBasedShadar.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_physicBasedShadar.bindUniform("m_lightCount", m_lightCount);
	m_physicBasedShadar.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_physicBasedShadar.bindUniform("m_lightPower", 4, &m_lightPower[0]);

	m_physicBasedShadar.bindUniform("Roughness", 0.05f);
	m_physicBasedShadar.bindUniform("ReflectionCoefficient", 0.5f);
	// bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_lucyTransform;
	m_physicBasedShadar.bindUniform("ProjectionViewModel", pvm);
	// bind transforms for lighting
	m_physicBasedShadar.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_lucyTransform)));
	// Bind camera
	m_physicBasedShadar.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));

	m_physicBasedShadar.bindUniform("diffuseTex", 0);
	// Bind texture to specified location
	m_gridTexture.bind(0);
	
	// draw lucy
	m_lucyMesh.draw();
}

void MyApplication::physicsBasedShaderSpear()
{
	// Bind Oren-Nayar BDRF shader program
	m_physicBasedShadar.bind();
	// bind light
	m_physicBasedShadar.bindUniform("Ia", m_ambientLight);

	m_physicBasedShadar.bindUniform("m_pointLightPos", 4, &m_pointLightPos[0]);
	m_physicBasedShadar.bindUniform("m_lightCount", m_lightCount);
	m_physicBasedShadar.bindUniform("m_lightColors", 4, &m_lightColors[0]);
	m_physicBasedShadar.bindUniform("m_lightPower", 4, &m_lightPower[0]);

	m_physicBasedShadar.bindUniform("Roughness", 0.05f);
	m_physicBasedShadar.bindUniform("ReflectionCoefficient", 0.5f);
	// bind transform
	auto pvm = m_camera.GetProjectionMatrix(getWindowWidth(), getWindowHeight()) * m_camera.GetViewMatrix() * m_spearTransform;
	m_physicBasedShadar.bindUniform("ProjectionViewModel", pvm);
	// bind transforms for lighting
	m_physicBasedShadar.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_spearTransform)));
	// Bind camera
	m_physicBasedShadar.bindUniform("CameraPosition", vec3(glm::inverse(m_camera.GetViewMatrix())[3]));
	// draw spear
	m_spearMesh.draw();
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

void MyApplication::IMGUITools()
{
	if (ImGui::CollapsingHeader("Renderer Target"))
	{
		ImGui::RadioButton("Off", &imgui_renderTarget, 0); ImGui::SameLine();
		ImGui::RadioButton("On", &imgui_renderTarget, 1);
	}

	if (ImGui::CollapsingHeader("Shaders"))
	{
		ImGui::Combo("Current Shader", &imgui_shader, "Simple\0Textured\0Phong\0Normal Map\0Physics Based\0\0");   // Combo using values packed in a single constant string (for really quick combo)
	}

	if (ImGui::CollapsingHeader("Model"))
	{
		ImGui::Combo("Current Model", &imgui_model, "Quad\0Bunny\0Dragon\0Buddha\0Lucy\0Spear\0\0");   // Combo using values packed in a single constant string (for really quick combo)
	}

	if (ImGui::CollapsingHeader("Lighting"))
	{
		ImGui::Combo("Light 1 Color", &imgui_light1, "White\0Red\0Orange\0Yellow\0Green\0Blue\0Purple\0\0");   // Combo using values packed in a single constant string (for really quick combo)

		ImGui::Combo("Light 2 Color", &imgui_light2, "White\0Red\0Orange\0Yellow\0Green\0Blue\0Purple\0\0");   // Combo using values packed in a single constant string (for really quick combo)

		ImGui::Combo("Light 3 Color", &imgui_light3, "White\0Red\0Orange\0Yellow\0Green\0Blue\0Purple\0\0");   // Combo using values packed in a single constant string (for really quick combo)

		ImGui::Combo("Light 4 Color", &imgui_light4, "White\0Red\0Orange\0Yellow\0Green\0Blue\0Purple\0\0");   // Combo using values packed in a single constant string (for really quick combo)
	}


}

