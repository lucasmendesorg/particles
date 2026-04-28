//
// Particles (c) 2026 Lucas Mendes
//	https://github.com/LucasMendesOrg
//

#include <Particles/FpsCounter.h>

void FpsCounter::Update(float deltaTime)
{
	++m_frameCount;
	m_accumulatedTime += deltaTime;

	if (m_accumulatedTime >= 0.25f) {
		m_fps = (m_accumulatedTime > 0.0f)
			? (static_cast<float>(m_frameCount) / m_accumulatedTime)
			: 0.0f;

		m_frameCount = 0;
		m_accumulatedTime = 0.0f;
		m_changed = true;
	}
}

float FpsCounter::GetFps(void) const
{
	return m_fps;
}

bool FpsCounter::ConsumeChanged(void)
{
	bool changed = m_changed;
	m_changed = false;
	return changed;
}
