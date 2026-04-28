//
// Particles (c) 2026 Lucas Mendes
//	https://github.com/LucasMendesOrg
//

#include <Particles/Particles.h>

#include <cstdlib>

struct PackedVec4
{
	float x;
	float y;
	float z;
	float w;
};

void Particles::BuildInitialData(int count)
{
	m_positions.resize(static_cast<std::size_t>(count));
	m_colors.resize(static_cast<std::size_t>(count));

	for (int i = 0; i < count; ++i) {
		float fx = (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * 200.0f - 100.0f;
		float fy = (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * 200.0f - 100.0f;
		float fz = -((static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) * 200.0f + 1.0f);

		m_positions[static_cast<std::size_t>(i)] = { fx, fy, fz };

		float r = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
		float g = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
		float b = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

		m_colors[static_cast<std::size_t>(i)] = { r, g, b };
	}
}

bool Particles::Initialize(int count)
{
	m_count = count;
	BuildInitialData(count);

	std::vector<PackedVec4> positionsPacked(static_cast<std::size_t>(count));
	std::vector<PackedVec4> colorsPacked(static_cast<std::size_t>(count));

	for (int i = 0; i < count; ++i) {
		const Vertex3& p = m_positions[static_cast<std::size_t>(i)];
		const Vertex3& c = m_colors[static_cast<std::size_t>(i)];

		positionsPacked[static_cast<std::size_t>(i)] = { p.x, p.y, p.z, 1.0f };
		colorsPacked[static_cast<std::size_t>(i)] = { c.x, c.y, c.z, 1.0f };
	}

	glGenBuffers(1, &m_positionsSsbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_positionsSsbo);
	glBufferData(
		GL_SHADER_STORAGE_BUFFER,
		static_cast<GLsizeiptr>(positionsPacked.size() * sizeof(PackedVec4)),
		positionsPacked.data(),
		GL_STATIC_DRAW
	);

	glGenBuffers(1, &m_colorsSsbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_colorsSsbo);
	glBufferData(
		GL_SHADER_STORAGE_BUFFER,
		static_cast<GLsizeiptr>(colorsPacked.size() * sizeof(PackedVec4)),
		colorsPacked.data(),
		GL_STATIC_DRAW
	);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	m_positions.clear();
	m_colors.clear();
	m_positions.shrink_to_fit();
	m_colors.shrink_to_fit();

	return m_positionsSsbo != 0 && m_colorsSsbo != 0;
}

void Particles::BindSsbo(void) const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_positionsSsbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_colorsSsbo);
}

void Particles::Cleanup(void)
{
	if (m_positionsSsbo) {
		glDeleteBuffers(1, &m_positionsSsbo);
		m_positionsSsbo = 0;
	}

	if (m_colorsSsbo) {
		glDeleteBuffers(1, &m_colorsSsbo);
		m_colorsSsbo = 0;
	}

	m_positions.clear();
	m_colors.clear();
	m_count = 0;
}

int Particles::GetCount(void) const
{
	return m_count;
}
