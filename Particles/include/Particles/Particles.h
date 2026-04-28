//
// Particles (c) 2026 Lucas Mendes
//	https://github.com/LucasMendesOrg
//

#pragma once

#include <vector>
#include <glad/glad.h>
#include <Particles/Vertex3.h>

class Particles
{
public:
	bool Initialize(int count);
	void BindSsbo(void) const;
	void Cleanup(void);

	int GetCount(void) const;

private:
	void BuildInitialData(int count);

	GLuint m_positionsSsbo{};
	GLuint m_colorsSsbo{};
	int m_count{};

	std::vector<Vertex3> m_positions;
	std::vector<Vertex3> m_colors;
};
