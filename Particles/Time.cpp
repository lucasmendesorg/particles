//
// Particles (c) 2026 Lucas Mendes
//	https://github.com/LucasMendesOrg
//

#include <Particles/Time.h>

Time* Time::GetSingleton(void)
{
	static Time instance{};
	return &instance;
}

Time::Time(void)
{
	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_previousTime);

	m_deltaTime = 0.0f;
}

void Time::Update(void)
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);

	m_deltaTime = static_cast<float>(now.QuadPart - m_previousTime.QuadPart) / static_cast<float>(m_frequency.QuadPart);
	m_previousTime = now;
}

float Time::GetDelta(void) const
{
	return m_deltaTime;
}
