//
// Particles (c) 2026 Lucas Mendes
//	https://github.com/LucasMendesOrg
//

#pragma once

class FpsCounter
{
public:
	void Update(float deltaTime);

	float GetFps(void) const;
	bool ConsumeChanged(void);

private:
	float m_accumulatedTime{};
	float m_fps{};
	int m_frameCount{};
	bool m_changed{};
};
