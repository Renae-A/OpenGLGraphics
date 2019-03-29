#pragma once
#include "gl_core_4_4.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"
#include "RenderTarget.h"

class MyApplication
{
public:
	MyApplication();
	virtual ~MyApplication();

	int startup();
	void shutdown();
	bool update();
	void loadShaders();
	bool loadTextures();
	bool intialiseRenderTarget();
	void setUpTransforms();
	bool loadStanfordModels();
	void setUpLighting();

	void updateTime();
	void checkIMGUIValues();
	void updateLighting();

	void simpleShaderQuad();
	void simpleShaderBunny();
	void simpleShaderDragon();
	void simpleShaderBuddha();
	void simpleShaderLucy();
	void simpleShaderSpear();

	void texturedShaderQuad();
	void texturedShaderBunny();
	void texturedShaderDragon();
	void texturedShaderBuddha();
	void texturedShaderLucy();
	void texturedShaderSpear();

	void phongShaderQuad();
	void phongShaderBunny();
	void phongShaderDragon();
	void phongShaderBuddha();
	void phongShaderLucy();
	void phongShaderSpear();

	void normalMapShaderQuad();
	void normalMapShaderBunny();
	void normalMapShaderDragon();
	void normalMapShaderBuddha();
	void normalMapShaderLucy();
	void normalMapShaderSpear();

	void physicsBasedShaderQuad();
	void physicsBasedShaderBunny();
	void physicsBasedShaderDragon();
	void physicsBasedShaderBuddha();
	void physicsBasedShaderLucy();
	void physicsBasedShaderSpear();

	unsigned int getWindowWidth();
	unsigned int getWindowHeight();

	void IMGUITools();

private:

	GLFWwindow*		m_window;
	Camera			m_camera;

	double			m_prevTime;
	double			m_currTime;
	double			m_deltaTime;

	struct Light 
	{
		glm::vec3 direction;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	aie::RenderTarget	m_renderTarget;

	// Lighting
	glm::vec3			m_ambientLight;
	glm::vec3			m_pointLightPos[4];
	glm::vec3			m_lightColors[4];
	float				m_lightPower[4];
	int					m_lightCount;



	// Shaders
	aie::ShaderProgram	m_shader;
	aie::ShaderProgram	m_texturedShader;
	aie::ShaderProgram	m_phongShader;
	aie::ShaderProgram	m_normalMapShader;
	aie::ShaderProgram	m_physicBasedShadar;

	// Textures
	aie::Texture		m_gridTexture;
	aie::Texture		m_denimTexture;
	aie::Texture		m_carpetTexture;
	aie::Texture		m_lightningTexture;
	aie::Texture		m_tartanTexture;

	// Meshes and their respective transforms
	Mesh				m_quadMesh;
	glm::mat4			m_quadTransform;

	aie::OBJMesh		m_bunnyMesh;
	glm::mat4			m_bunnyTransform;

	aie::Texture		m_spearTexture;

	aie::OBJMesh		m_spearMesh;
	glm::mat4			m_spearTransform;

	aie::OBJMesh		m_dragonMesh;
	glm::mat4			m_dragonTransform;

	aie::OBJMesh		m_lucyMesh;
	glm::mat4			m_lucyTransform;

	aie::OBJMesh		m_buddhaMesh;
	glm::mat4			m_buddhaTransform;

	// IMGUI
	int imgui_renderTarget = 0;
	int imgui_shader = 0;
	int imgui_model = 0;
	int imgui_texture = 0;

	int imgui_light1 = 0;
	int imgui_light2 = 0;
	int imgui_light3 = 0;
	int imgui_light4 = 0;
};
