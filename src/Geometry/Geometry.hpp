#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Shader/Shader.hpp"
#include "VertexData/VertexAttribute.hpp"

#include <vector>

template<typename T, auto& Attribs>
class Geometry
{
public:
	Geometry()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		for (auto const& a : Attribs) {
			glEnableVertexAttribArray(a.location);
			glVertexAttribPointer(
				a.location,
				a.size,
				a.type,
				a.normalized,
				sizeof(T),
				reinterpret_cast<void*>(a.offset)
			);
		}
	}
	virtual ~Geometry()
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

	auto UpdateGPU() const -> void
	{
		if (m_vertices.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(T), &m_vertices[0], GL_DYNAMIC_DRAW);
		}
	}
	auto Draw(GLint type) const -> void
	{
		if (m_vertices.size() > 0)
		{
			glBindVertexArray(m_vao);
			glDrawArrays(type, 0, (int)m_vertices.size());
		}
	}
	auto Vtx() -> std::vector<T>& { return m_vertices; }

protected:
	GLuint m_vao;
	GLuint m_vbo;
	std::vector<T> m_vertices;
};