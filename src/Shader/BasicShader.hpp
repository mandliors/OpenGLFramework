#pragma once

#include "glad/glad.h"

#include "Shader.hpp"
#include "Rendering/RenderState.hpp"

class BasicShader : public Shader
{
public:
	BasicShader();

	auto Bind(const RenderState& state, bool bindLights = true) const -> void override;

private:
	static constexpr const char* s_vertexSource =
		R"(
		#version 330

		layout(location = 0) in vec2 pos;
		layout(location = 1) in vec3 col;
		layout(location = 2) in vec2 texCoord;

		out vec3 color;
		out vec2 textureCoords;

		void main()
		{
			gl_Position = vec4(pos, 0, 1);
	
			color = col;
			textureCoords = texCoord;
		}
	)";
	static constexpr const char* s_fragmentSource =
		R"(
		#version 330

		in vec3 color;
		in vec2 textureCoords;

		uniform sampler2D tex;

		out vec4 fragmentColor;

		void main()
		{
			fragmentColor = texture(tex, textureCoords) * vec4(color, 1);
		}
	)";
};