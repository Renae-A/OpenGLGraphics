#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Mesh
{
public:
	Mesh() : m_triCount(0), m_vao(0), m_vbo(0), m_ibo(0) {}
	virtual ~Mesh();

	struct Vertex {
		glm::vec4 m_position;
		glm::vec4 m_normal;
		glm::vec2 m_texCoord;
	};

	void initialiseQuad();
	virtual void draw();

protected:
	unsigned int m_triCount;
	unsigned int m_vao, m_vbo, m_ibo;
};

