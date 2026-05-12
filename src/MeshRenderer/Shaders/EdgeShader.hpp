#pragma once

#include "Shader/Shader.hpp"
#include "Rendering/RenderState.hpp"

class EdgeShader : public Shader
{
public:
	EdgeShader();

	auto Bind(const RenderState& state, bool bindLights = true) const -> void override;
};