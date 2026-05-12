#include "glad/glad.h"

#include "PointShader.hpp"

#include <array>
#include <print>

PointShader::PointShader()
{
	if (!Create()) return;

	const auto shaders = std::array<std::pair<GLenum, fs::path>, 2>{ {
		{ GL_VERTEX_SHADER, "assets/shaders/3dsimple.vs" },
		{ GL_FRAGMENT_SHADER, "assets/shaders/3dpoint.fs" }
	} };

	for (const auto& [type, source] : shaders)
	{
		if (auto result = AttachShader(type, source); result != LoadShaderError::NONE)
		{
			std::println("failed to attach shader with type {}: {}", type, LoadShaderError_ToString(result));
			glDeleteProgram(m_shaderId);
			m_shaderId = 0;
			return;
		}
	}

	if (!Link())
	{
		glDeleteProgram(m_shaderId);
		m_shaderId = 0;
	}
}
auto PointShader::Bind(const RenderState& state, bool bindLights) const -> void
{
	Use();
	SetUniform(state.MVP, "MVP");
}