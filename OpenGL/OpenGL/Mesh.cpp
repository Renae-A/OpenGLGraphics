#include "Mesh.h"
#include "gl_core_4_4.h"

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void Mesh::initialiseQuad()
{
	// check that the mesh is not initialized already
	assert(m_vao == 0);

	// generate buffers
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);
	// bind vertex array aka a mesh wrapper
	glBindVertexArray(m_vao);
	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// define 6 vertices for 2 triangles
	Vertex vertices[6];
	vertices[0].m_position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].m_position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].m_position = { -0.5f, 0, -0.5f, 1 };

	vertices[3].m_position = { -0.5f, 0, -0.5f, 1 };
	vertices[4].m_position = { 0.5f, 0, 0.5f, 1 };
	vertices[5].m_position = { 0.5f, 0, -0.5f, 1 };

	vertices[0].m_texCoord = { 0, 1 }; // bottom left
	vertices[1].m_texCoord = { 1, 1 }; // bottom right
	vertices[2].m_texCoord = { 0, 0 }; // top left
	vertices[3].m_texCoord = { 0, 0 }; // top left
	vertices[4].m_texCoord = { 1, 1 }; // bottom right
	vertices[5].m_texCoord = { 1, 0 }; // top right


	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// enable third element as texture
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (void*)32);

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// quad has 2 triangles
	m_triCount = 2;
}

void Mesh::draw()
{
	glBindVertexArray(m_vao);
	// using indices or just vertices?
	if (m_ibo != 0)
		glDrawElements(GL_TRIANGLES, 3 * m_triCount,
			GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, 3 * m_triCount);
}
