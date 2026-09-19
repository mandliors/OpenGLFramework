#pragma once

#include "Renderables/Renderable.hpp"
#include "Geometry/Geometry.hpp"
#include "MeshRenderer/SpecialVertexDatas.hpp"

class Renderable3D : public Geometry<glm::vec3, Vertex3DUniformedAttribs>, public Renderable
{
public:
	Renderable3D(Shader &shader, GLenum type, const glm::vec4 &color)
		: Renderable(shader, type, color)
	{
	}

	auto Draw(const RenderState &renderState) const -> void override
	{
		m_shader.Use();
		m_shader.SetUniform(renderState.MVP, "MVP");
		m_shader.SetUniform(m_color, "color");
		Geometry::Draw(m_type);
	}
};