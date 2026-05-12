#pragma once

#include "Shader/Shader.hpp"
#include "Rendering/RenderState.hpp"

class FaceShader : public Shader
{
public:
	FaceShader();

	auto Bind(const RenderState& state, bool bindLights = true) const -> void override;
};