#pragma once

#include "glm/glm.hpp"

struct Material
{
	glm::vec3 Ambient{ 0.0f };
	glm::vec3 Diffuse{ 0.0f };
	glm::vec3 Specular{ 0.0f };
	float Shininess = 32.0f;
	float Emission = 0.0f;

	Material() = default;
	Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, float emission)
		: Ambient(ambient), Diffuse(diffuse), Specular(specular), Shininess(shininess), Emission(emission) {
	}
};