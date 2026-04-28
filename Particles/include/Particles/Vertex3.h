//
// Particles (c) 2026 Lucas Mendes
//	https://github.com/LucasMendesOrg
//

#pragma once

#include <cmath>

struct Vertex3
{
	float x{};
	float y{};
	float z{};

	Vertex3 operator-(const Vertex3& rhs) const
	{
		return { x - rhs.x, y - rhs.y, z - rhs.z };
	}

	static float Dot(const Vertex3& a, const Vertex3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	static Vertex3 Cross(const Vertex3& a, const Vertex3& b)
	{
		return {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}

	static Vertex3 Normalize(const Vertex3& v)
	{
		float len = std::sqrt(Dot(v, v));
		if (len <= 0.0f) {
			return { 0.0f, 0.0f, 0.0f };
		}

		float inv = 1.0f / len;
		return { v.x * inv, v.y * inv, v.z * inv };
	}
};
