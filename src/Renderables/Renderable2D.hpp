#pragma once

#include "Renderables/Renderable.hpp"
#include "Geometry/Geometry.hpp"
#include "MeshRenderer/SpecialVertexDatas.hpp"

class Renderable2D : public Geometry<glm::vec2, Vertex2DUniformedAttribs>, Renderable
{
public:
	Renderable2D(Shader &shader, GLenum type, const glm::vec4 &color)
		: Renderable(shader, type, color)
	{
	}

	auto Draw(const RenderState &renderState) const -> void override
	{
		m_shader.Use();
		m_shader.SetUniform(m_color, "color");
		m_shader.SetUniform(m_type == GL_POINTS, "point");
		Geometry::Draw(m_type);
	}
};