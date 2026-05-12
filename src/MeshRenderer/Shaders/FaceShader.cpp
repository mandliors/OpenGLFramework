#include "glad/glad.h"

#include "FaceShader.hpp"

#include <array>
#include <print>

FaceShader::FaceShader()
{
	if (!Create()) return;

	const auto shaders = std::array<std::pair<GLenum, fs::path>, 2>{ {
		{ GL_VERTEX_SHADER, "assets/shaders/3dface.vs" },
		{ GL_FRAGMENT_SHADER, "assets/shaders/3dface.fs" }
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
auto FaceShader::Bind(const RenderState& state, bool bindLights) const -> void
{
	Use();

	SetUniform(state.MVP, "MVP");
	SetUniform(state.Model, "M");
	SetUniform(state.ModelInverse, "Minv");
	SetUniform(state.View, "V");
	SetUniform(state.Projection, "P");
	SetUniform(state.CameraPosition, "camPos");

	if (state.Material)
	{
		SetUniform(state.Material->Ambient, "material.ambient");
		SetUniform(state.Material->Diffuse, "material.diffuse");
		SetUniform(state.Material->Specular, "material.specular");
		SetUniform(state.Material->Shininess, "material.shininess");
		SetUniform(state.Material->Emission, "material.emission");
	}

	if (bindLights && state.Lights)
	{
		SetUniform(static_cast<int>(state.Lights->size()), "nLights");
		for (size_t i = 0; i < state.Lights->size(); i++)
		{
			const auto& light = (*state.Lights)[i];
			auto prefix = std::format("lights[{}].", i);
			SetUniform(light.Position, prefix + "position");
			SetUniform(light.Ambient, prefix + "ambient");
			SetUniform(light.Emission, prefix + "emission");
			SetUniform(light.Constant, prefix + "constant");
			SetUniform(light.Linear, prefix + "linear");
			SetUniform(light.Quadratic, prefix + "quadratic");
		}
	}

	constexpr int textureUnit = 0;
	SetUniform(textureUnit, "tex");
	if (state.Texture)
		state.Texture->Bind(textureUnit);
}