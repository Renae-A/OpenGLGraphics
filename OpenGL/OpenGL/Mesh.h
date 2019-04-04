#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Mesh
{
public:
	Mesh() : m_triCount(0), m_vao(0), m_vbo(0), m_ibo(0) {}	// Mesh constructor intialises default values of 0 for m_vao, m_vbo and m_ibo
	virtual ~Mesh();										// Mesh destructor destroys vertex arrays and buffers

	// the Vertex struct has a position, normal and texture coordinates
	struct Vertex 
	{
		glm::vec4 m_position;
		glm::vec4 m_normal;
		glm::vec2 m_texCoord;
	};

	void initialiseQuad();	// Intialises a quad with vertices and the normal directed up
	virtual void draw();	// Draw() counts the triangles in the mesh and draws the mesh in the position specified which also reads rotation and the vertices

protected:
	unsigned int m_triCount;
	unsigned int m_vao, m_vbo, m_ibo;
};

