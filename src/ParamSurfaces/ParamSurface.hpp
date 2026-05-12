#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Geometry/Geometry.hpp"
#include "VertexData/VertexData.hpp"
#include "Dnum/Dnum.hpp"

#include <tuple>

using Dnum2 = Dnum<glm::vec2>;

class ParamSurface : public Geometry<Vertex, VertexAttribs>
{
public:
	ParamSurface(const glm::vec3& color = glm::vec3{ 1.0f }) : m_color(color) {}

	auto Create(uint32_t n, uint32_t m) -> void;
	auto Draw() -> void;

protected:
	virtual auto Evaluate(const Dnum2& U, const Dnum2& V) const -> std::tuple<Dnum2, Dnum2, Dnum2> = 0;

private:
	auto GenVertexData(float u, float v, const glm::vec3& color) -> Vertex;

private:
	uint32_t m_nStrips = 0;
	uint32_t m_nVtxPerStrip = 0;
	glm::vec3 m_color;
};