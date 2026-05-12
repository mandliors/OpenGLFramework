#pragma once

#include "glm/glm.hpp"

#include "Material.hpp"
#include "Light.hpp"
#include "Texture/Texture.hpp"

#include <memory>
#include <vector>

struct RenderState
{
	glm::mat4 MVP{ 1.0f };
	glm::mat4 Model{ 1.0f };
	glm::mat4 ModelInverse{ 1.0f };
	glm::mat4 View{ 1.0f };
	glm::mat4 Projection{ 1.0f };

	Material* Material{ nullptr };
	std::vector<Light>* Lights;
	Texture* Texture{ nullptr };

	glm::vec3 CameraPosition{ 0.0f, 0.0f, 0.0f };
};