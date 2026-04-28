//
// Particles (c) 2026 Lucas Mendes
//	https://github.com/LucasMendesOrg
//

#pragma once

#include <windows.h>

class Time
{
public:
	static Time* GetSingleton(void);

	void Update(void);
	float GetDelta(void) const;

private:
	Time(void);
	~Time(void) = default;
	Time(const Time&) = delete;
	Time& operator=(const Time&) = delete;

	LARGE_INTEGER m_frequency{};
	LARGE_INTEGER m_previousTime{};
	float m_deltaTime{};
};
