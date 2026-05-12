#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <array>

#include "VertexAttribute.hpp"
#include "Geometry/Geometry.hpp"

struct Vertex2D
{
	glm::vec2 position;
	glm::vec3 color;
	glm::vec2 texCoord;

	Vertex2D() = default;
	Vertex2D(const glm::vec2& pos, const glm::vec3& col, const glm::vec2& tex)
		: position(pos), color(col), texCoord(tex) {
	}
};
inline constexpr std::array<VertexAttribute, 3> Vertex2DAttribs{ {
	{ 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, position) },
	{ 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, color) },
	{ 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, texCoord) }
} };

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoord;

	Vertex() = default;
	Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec3& col, const glm::vec2& tex)
		: position(pos), normal(norm), color(col), texCoord(tex) {
	}
};
inline constexpr std::array<VertexAttribute, 4> VertexAttribs{ {
	{ 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position) },
	{ 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal) },
	{ 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, color) },
	{ 3, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord) }
} };