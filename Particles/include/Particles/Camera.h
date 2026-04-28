//
// Particles (c) 2026 Lucas Mendes
//	https://github.com/LucasMendesOrg
//

#pragma once

#include <Particles/Vertex3.h>
#include <Particles/Matrix4x4.h>

class Camera
{
public:
	Camera(void);

	void SetPerspective(float fovDegrees, float aspect, float nearPlane, float farPlane);
	void SetLookAt(const Vertex3& eye, const Vertex3& target, const Vertex3& up);

	const Matrix4x4& GetViewProjection(void) const;

private:
	void RebuildViewProjection(void);

	Vertex3 m_eye{ 0.0f, 0.0f, 4.0f };
	Vertex3 m_target{ 0.0f, 0.0f, 0.0f };
	Vertex3 m_up{ 0.0f, 1.0f, 0.0f };

	float m_fovDegrees{ 60.0f };
	float m_aspect{ 4.0f / 3.0f };
	float m_nearPlane{ 0.1f };
	float m_farPlane{ 100.0f };

	Matrix4x4 m_view{};
	Matrix4x4 m_projection{};
	Matrix4x4 m_viewProjection{};
};
