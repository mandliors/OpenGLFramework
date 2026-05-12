#pragma once

#include "glm/glm.hpp"

#include "Dnum/Dnum.hpp"
#include "ParamSurfaces/ParamSurface.hpp"

#include <tuple>
#include <numbers>
#include <cmath>

using Dnum2 = Dnum<glm::vec2>;

class Cone : public ParamSurface
{
public:
	Cone(uint32_t n, uint32_t m, const glm::vec3& color, float alpha) : ParamSurface(color), m_alpha(alpha* std::numbers::pi_v<float> / 180) { Create(n, m); }

	auto Evaluate(const Dnum2& U, const Dnum2& V) const -> std::tuple<Dnum2, Dnum2, Dnum2> override
	{
		auto _U = U * 2.0f * std::numbers::pi_v<float>;
		const float alpha = 0.5f * m_alpha;

		return {
			V * std::tan(alpha) * Cos(_U),
			V * -1.0f + 0.5f,
			V * std::tan(alpha) * Sin(_U)
		};
	}

private:
	float m_alpha;
};