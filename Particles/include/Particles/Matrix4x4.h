//
// Particles (c) 2026 Lucas Mendes
//	https://github.com/LucasMendesOrg
//

#pragma once

struct Matrix4x4
{
	float m[16]{};

	Matrix4x4 operator*(const Matrix4x4& rhs) const
	{
		Matrix4x4 result{};

		for (int c = 0; c < 4; ++c) {
			for (int row = 0; row < 4; ++row) {
				result.m[c * 4 + row] =
					m[0 * 4 + row] * rhs.m[c * 4 + 0] +
					m[1 * 4 + row] * rhs.m[c * 4 + 1] +
					m[2 * 4 + row] * rhs.m[c * 4 + 2] +
					m[3 * 4 + row] * rhs.m[c * 4 + 3];
			}
		}

		return result;
	}
};
