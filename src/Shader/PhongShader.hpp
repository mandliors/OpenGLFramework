#pragma once

#include "glad/glad.h"

#include "Shader.hpp"
#include "Rendering/RenderState.hpp"

class PhongShader : public Shader
{
public:
	PhongShader();

	auto Bind(const RenderState& state, bool bindLights = true) const -> void override;

private:
	static constexpr const char* s_vertexSource =
		R"(
		#version 330

		layout(location = 0) in vec3 pos;
		layout(location = 1) in vec3 norm;
		layout(location = 2) in vec3 col;
		layout(location = 3) in vec2 texCoord;

		uniform mat4 MVP;
		uniform mat4 M;
		uniform mat4 Minv;

		uniform vec3 camPos;

		out vec4 position;
		out vec3 normal;
		out vec3 view;
		out vec3 color;
		out vec2 textureCoords;

		void main()
		{
			gl_Position = MVP * vec4(pos, 1);

			position = M * vec4(pos, 1);
			normal = (vec4(norm, 0) * Minv).xyz;
			view = camPos - position.xyz / position.w;
			color = col;
			textureCoords = texCoord;
		}
	)";
	static constexpr const char* s_fragmentSource =
		R"(
		#version 330

		struct Light
		{
			vec4 position;
			vec3 ambient, emission;
			float constant, linear, quadratic;
		};

		struct Material
		{
			vec3 ambient, diffuse, specular;
			float shininess, emission;
		};

		uniform Material material;

		uniform int nLights;
		uniform Light lights[10];

		uniform sampler2D tex;

		in vec4 position;
		in vec3 normal;
		in vec3 view;
		in vec3 color;
		in vec2 textureCoords;

		out vec4 fragmentColor;

		void main()
		{
			vec3 N = normalize(normal);
			vec3 V = normalize(view);
			if (dot(N, V) < 0) N = -N;
			vec3 texColor = texture(tex, textureCoords).rgb;

			vec3 ambient = material.ambient * texColor * color;
			vec3 diffuse = material.diffuse * texColor * color;
			vec3 emission = material.emission * texColor * color;

			vec3 radiance = emission;

			for (int i = 0; i < nLights; ++i)
			{
				vec3 dir = lights[i].position.xyz * position.w - position.xyz * lights[i].position.w;

				vec3 L = normalize(dir);
				vec3 H = normalize(L + V);
				float cost = max(dot(N, L), 0);
				float cosd = max(dot(N, H), 0);
				float dist = length(dir);
				float attenuation = 1.0f / (lights[i].constant + lights[i].linear * dist + lights[i].quadratic * dist * dist);

				radiance += 
					(ambient * lights[i].ambient
					+ (diffuse * cost + material.specular * pow(cosd, material.shininess))
					* lights[i].emission)
					* attenuation;
			}

			fragmentColor = vec4(radiance, 1);
		}
	)";
};