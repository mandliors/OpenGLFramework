#pragma once

#include "glm/glm.hpp"

#include "Dnum/Dnum.hpp"
#include "ParamSurfaces/ParamSurface.hpp"

#include <tuple>
#include <numbers>
#include <cmath>

using Dnum2 = Dnum<glm::vec2>;

class Cylinder : public ParamSurface
{
public:
	Cylinder(uint32_t n, uint32_t m, const glm::vec3& color) : ParamSurface(color) { Create(n, m); }

	auto Evaluate(const Dnum2& U, const Dnum2& V) const -> std::tuple<Dnum2, Dnum2, Dnum2> override
	{
		auto _U = U * 2.0f * std::numbers::pi_v<float> -std::numbers::pi_v<float>;

		return {
			Cos(_U),
			V * 2.0f - 1.0f,
			Sin(_U)
		};
	}
};