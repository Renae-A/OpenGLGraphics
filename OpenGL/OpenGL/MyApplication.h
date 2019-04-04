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

	int startup();					// Sets up a window, transforms, and lighting. Loads all shaders, textures and meshes. Initialises quads and render target. Creates imgui window. Returns an int determining success
	void shutdown();				// Destroys imgui window, gizmos and window
	bool update();					// Updates everything on screen - returns true for if the user hits escape to exit the application (stops updating)
	void loadShaders();				// Loads in the different shaders for use - will display error is issues occur
	bool loadTextures();			// Loads in the different textures for use - will display error is issues occur
	bool intialiseRenderTarget();	// Initialises the render target for use - will display error is issues occur
	void setUpTransforms();			// Assigns each matrix4 member variable for object transforms to similar sizes
	bool loadStanfordModels();		// Loads in the stanford models from the data folder
	void setUpLighting();			// Creates four light sources and gives them an equal power of 100 and positions them around the mesh position

	void updateTime();				// Ensures the current, previous and delta time are updated accordingly
	void checkIMGUIValues();		// Checks for changes made by the user on the imGui tool and runs which demonstration the user has selected
	void updateLighting();			// Checks for changes made by the user on the imGui tool and changes color of lights to user's choice

	void simpleShaderQuad();		// Draws a quad using the simple shader (checks if render target is on and applies accordingly)
	void simpleShaderBunny();		// Draws a bunny using the simple shader (checks if render target is on and applies accordingly)
	void simpleShaderDragon();		// Draws a dragon using the simple shader (checks if render target is on and applies accordingly)
	void simpleShaderBuddha();		// Draws a buddha using the simple shader (checks if render target is on and applies accordingly)
	void simpleShaderLucy();		// Draws a lucy using the simple shader (checks if render target is on and applies accordingly)
	void simpleShaderSpear();		// Draws a spear using the simple shader (checks if render target is on and applies accordingly)

	void texturedShaderQuad();		// Draws a quad using the textured shader (checks if render target is on and applies accordingly)
	void texturedShaderSpear();		// Draws a spear using the textured shader (checks if render target is on and applies accordingly)

	void phongShaderQuad();			// Draws a quad using the phong shader (checks if render target is on and applies accordingly)	
	void phongShaderBunny();		// Draws a bunny using the phong shader (checks if render target is on and applies accordingly)
	void phongShaderDragon();		// Draws a dragon using the phong shader (checks if render target is on and applies accordingly)
	void phongShaderBuddha();		// Draws a buddha using the phong shader (checks if render target is on and applies accordingly)
	void phongShaderLucy();			// Draws a lucy using the phong shader (checks if render target is on and applies accordingly)
	void phongShaderSpear();		// Draws a spear using the phong shader (checks if render target is on and applies accordingly)

	void normalMapShaderSpear();	// Draws a spear using the normal map shader (checks if render target is on and applies accordingly)

	void physicsBasedShaderQuad();	// Draws a quad using the physicsBased shader (checks if render target is on and applies accordingly)	
	void physicsBasedShaderBunny();	// Draws a bunny using the physicsBased shader (checks if render target is on and applies accordingly)
	void physicsBasedShaderDragon();// Draws a dragon using the physicsBased shader (checks if render target is on and applies accordingly)
	void physicsBasedShaderBuddha();// Draws a buddha using the physicsBased shader (checks if render target is on and applies accordingly)
	void physicsBasedShaderLucy();	// Draws a lucy using the physicsBased shader (checks if render target is on and applies accordingly)
	void physicsBasedShaderSpear();	// Draws a spear using the physicsBased shader (checks if render target is on and applies accordingly)

	void renderTargetStart();		// Binds the render target and clears the screen
	void renderTargetEnd();			// Unbinds the render target, clears the screen and draws a textured quad with the rendered image

	unsigned int getWindowWidth();	// Returns unsigned int of the window's width
	unsigned int getWindowHeight();	// Returns unsigned int of the window's height

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

	// IMGUI variables
	int imgui_renderTarget = 0;
	int imgui_shader = 0;
	int imgui_model = 0;
	int imgui_texture = 0;

	int imgui_light1 = 0;
	int imgui_light2 = 0;
	int imgui_light3 = 0;
	int imgui_light4 = 0;
};
