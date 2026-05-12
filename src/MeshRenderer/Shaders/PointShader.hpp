#pragma once

#include "Shader/Shader.hpp"
#include "Rendering/RenderState.hpp"

class PointShader : public Shader
{
public:
	PointShader();

	auto Bind(const RenderState& state, bool bindLights = true) const -> void override;
};