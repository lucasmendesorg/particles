//
// Particles (c) 2026 Lucas Mendes
//	https://github.com/LucasMendesOrg
//

#include <Particles/Camera.h>

#include <cmath>

Camera::Camera(void)
{
	RebuildViewProjection();
}

void Camera::SetPerspective(float fovDegrees, float aspect, float nearPlane, float farPlane)
{
	m_fovDegrees = fovDegrees;
	m_aspect = aspect;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;

	RebuildViewProjection();
}

void Camera::SetLookAt(const Vertex3& eye, const Vertex3& target, const Vertex3& up)
{
	m_eye = eye;
	m_target = target;
	m_up = up;

	RebuildViewProjection();
}

const Matrix4x4& Camera::GetViewProjection(void) const
{
	return m_viewProjection;
}

void Camera::RebuildViewProjection(void)
{
	Vertex3 f = Vertex3::Normalize(m_target - m_eye);
	Vertex3 s = Vertex3::Normalize(Vertex3::Cross(f, m_up));
	Vertex3 u = Vertex3::Cross(s, f);

	m_view = {
		s.x, u.x, -f.x, 0.0f,
		s.y, u.y, -f.y, 0.0f,
		s.z, u.z, -f.z, 0.0f,
		-Vertex3::Dot(s, m_eye), -Vertex3::Dot(u, m_eye), Vertex3::Dot(f, m_eye), 1.0f
	};

	float fovRad = m_fovDegrees * 0.01745329251994329577f;
	float t = 1.0f / std::tan(fovRad * 0.5f);

	m_projection = {
		t / m_aspect, 0.0f, 0.0f, 0.0f,
		0.0f, t, 0.0f, 0.0f,
		0.0f, 0.0f, -(m_farPlane + m_nearPlane) / (m_farPlane - m_nearPlane), -1.0f,
		0.0f, 0.0f, -(2.0f * m_farPlane * m_nearPlane) / (m_farPlane - m_nearPlane), 0.0f
	};

	m_viewProjection = m_projection * m_view;
}
