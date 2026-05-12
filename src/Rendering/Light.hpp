#pragma once

#include "glm/glm.hpp"

struct Light
{
	glm::vec4 Position{ 0.0f };
	glm::vec3 Ambient{ 0.0f };
	glm::vec3 Emission{ 0.0f };

	float Constant;
	float Linear;
	float Quadratic;

	Light() = default;
	Light(const glm::vec4& position, const glm::vec3& ambient, const glm::vec3& emission, float constant, float linear, float quadratic)
		: Position(position), Ambient(ambient), Emission(emission), Constant(constant), Linear(linear), Quadratic(quadratic) {
	}
};