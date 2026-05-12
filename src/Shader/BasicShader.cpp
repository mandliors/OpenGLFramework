#include "glad/glad.h"

#include "BasicShader.hpp"

#include <array>
#include <print>

BasicShader::BasicShader()
{
	if (!Create()) return;

	constexpr auto shaders = std::array<std::pair<GLenum, std::string_view>, 2>{ {
		{ GL_VERTEX_SHADER, s_vertexSource },
		{ GL_FRAGMENT_SHADER, s_fragmentSource }
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
auto BasicShader::Bind(const RenderState& state, bool bindLights) const -> void
{
	Use();

	constexpr int textureUnit = 0;
	SetUniform(textureUnit, "tex");
	if (state.Texture)
		state.Texture->Bind(textureUnit);
}