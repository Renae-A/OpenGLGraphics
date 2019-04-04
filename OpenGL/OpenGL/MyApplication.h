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
	MyApplication();				// Default constructor initialises time member variables
	virtual ~MyApplication();		// Virtual destructor

	int startup();					//
	void shutdown();
	bool update();
	void loadShaders();				// Loads in the different shaders for use - will display error is issues occur
	bool loadTextures();			// Loads in the different textures for use - will display error is issues occur
	bool intialiseRenderTarget();	// Initialises the render target for use - will display error is issues occur
	void setUpTransforms();			// Assigns each matrix4 member variable for object transforms to similar sizes
	bool loadStanfordModels();		// Loads in the stanford models from the data folder
	void setUpLighting();			// Creates four light sources and gives them an equal power of 100 and positions them around the mesh position

	void updateTime();				// Ensures the current, previous and delta time are updated accordingly
	void checkIMGUIValues();		// Checks for changes made by the user the imGui tools on 
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

	void renderTargetStart();
	void renderTargetEnd();

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

	// Meshes and their respective transforms
	Mesh				m_quadMesh;
	Mesh				m_quadMesh2;
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
