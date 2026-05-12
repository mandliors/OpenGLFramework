#pragma once

#include "glm/glm.hpp"

template<class T>
struct Dnum
{
	float f;
	T d;

	Dnum(float f0 = 0, T d0 = T(0)) { f = f0, d = d0; }

	friend auto operator+(Dnum const& lhs, Dnum const& rhs) -> Dnum { return Dnum(lhs.f + rhs.f, lhs.d + rhs.d); }
	friend auto operator-(Dnum const& lhs, Dnum const& rhs) -> Dnum { return Dnum(lhs.f - rhs.f, lhs.d - rhs.d); }
	friend auto operator*(Dnum const& lhs, Dnum const& rhs) -> Dnum { return Dnum(lhs.f * rhs.f, lhs.f * rhs.d + lhs.d * rhs.f); }
	friend auto operator/(Dnum const& lhs, Dnum const& rhs) -> Dnum { return Dnum(lhs.f / rhs.f, (rhs.f * lhs.d - rhs.d * lhs.f) / rhs.f / rhs.f); }
};

template<class T> auto Exp(Dnum<T> g) -> Dnum<T> { return Dnum<T>(expf(g.f), expf(g.f) * g.d); }
template<class T> auto Sin(Dnum<T> g) -> Dnum<T> { return  Dnum<T>(sinf(g.f), cosf(g.f) * g.d); }
template<class T> auto Cos(Dnum<T>  g) -> Dnum<T> { return  Dnum<T>(cosf(g.f), -sinf(g.f) * g.d); }
template<class T> auto Tan(Dnum<T>  g) -> Dnum<T> { return Sin(g) / Cos(g); }
template<class T> auto Sinh(Dnum<T> g) -> Dnum<T> { return  Dnum<T>(sinh(g.f), cosh(g.f) * g.d); }
template<class T> auto Cosh(Dnum<T> g) -> Dnum<T> { return  Dnum<T>(cosh(g.f), sinh(g.f) * g.d); }
template<class T> auto Tanh(Dnum<T> g) -> Dnum<T> { return Sinh(g) / Cosh(g); }
template<class T> auto Log(Dnum<T> g) -> Dnum<T> { return  Dnum<T>(logf(g.f), g.d / g.f); }
template<class T> auto Pow(Dnum<T> g, float n) -> Dnum<T> { return Dnum<T>(powf(g.f, n), n * powf(g.f, n - 1) * g.d); }