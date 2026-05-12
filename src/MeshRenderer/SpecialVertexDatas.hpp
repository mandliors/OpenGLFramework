#pragma once

#include "glm/glm.hpp"

#include "VertexData/VertexData.hpp"

inline constexpr std::array<VertexAttribute, 1> Vertex2DUniformedAttribs{ {
	{ 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, position) }
} };
inline constexpr std::array<VertexAttribute, 1> Vertex3DUniformedAttribs{ {
	{ 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position) }
} };

struct PosVertex
{
	glm::vec3 position;

	PosVertex() = default;
	PosVertex(const glm::vec3& pos)
		: position(pos) {
	}
};
struct PosNormVertex
{
	glm::vec3 position;
	glm::vec3 normal;

	PosNormVertex() = default;
	PosNormVertex(const glm::vec3& pos, const glm::vec3& norm)
		: position(pos), normal(norm) {
	}
};
inline constexpr std::array<VertexAttribute, 2> PosVertexAttribs{ {
	{ 0, 3, GL_FLOAT, GL_FALSE, offsetof(PosNormVertex, position) }
} };
inline constexpr std::array<VertexAttribute, 2> PosNormVertexAttribs{ {
	{ 0, 3, GL_FLOAT, GL_FALSE, offsetof(PosNormVertex, position) },
	{ 1, 3, GL_FLOAT, GL_FALSE, offsetof(PosNormVertex, normal) },
} };