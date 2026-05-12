#include "glad/glad.h"
#include "glm/glm.hpp"

#include "ParamSurface.hpp"

#include <tuple>

auto ParamSurface::Create(uint32_t n, uint32_t m) -> void
{
	m_nStrips = n;
	m_nVtxPerStrip = (m + 1) * 2;

	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j <= m; j++)
		{
			m_vertices.push_back(GenVertexData(static_cast<float>(j) / m, static_cast<float>(i) / n, m_color));
			m_vertices.push_back(GenVertexData(static_cast<float>(j) / m, (static_cast<float>(i) + 1) / n, m_color));
		}
	}
	glBufferData(GL_ARRAY_BUFFER, m_nVtxPerStrip * m_nStrips * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
}
auto ParamSurface::Draw() -> void
{
	glBindVertexArray(m_vao);
	for (uint32_t i = 0; i < m_nStrips; i++)
		glDrawArrays(GL_TRIANGLE_STRIP, i * m_nVtxPerStrip, m_nVtxPerStrip);
}

auto ParamSurface::GenVertexData(float u, float v, const glm::vec3& color) -> Vertex
{
	auto U = Dnum2{ u, { 1, 0 } };
	auto V = Dnum2{ v, { 0, 1 } };
	auto [X, Y, Z] = Evaluate(U, V);

	auto drdU = glm::vec3{ X.d.x, Y.d.x, Z.d.x };
	auto drdV = glm::vec3{ X.d.y, Y.d.y, Z.d.y };

	return
	{
		glm::vec3{X.f, Y.f, Z.f},
		cross(drdU, drdV),
		color,
		{ u, v }
	};
}