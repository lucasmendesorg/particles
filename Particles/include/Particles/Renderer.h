//
// Particles (c) 2026 Lucas Mendes
//	https://github.com/LucasMendesOrg
//

#pragma once

#include <memory>
#include <glad/glad.h>
#include <Particles/Camera.h>
#include <Particles/Particles.h>

class Renderer
{
public:
	static Renderer* Initialize(void);
	static Renderer* GetSingleton(void);
	static void Shutdown(void);

	int GetTotalParticleCount(void);
	int Render(float deltaTime);

	~Renderer(void);
	Renderer(void) = default;

private:
	static constexpr int particleCount{ 1000000 };
	static constexpr int visibleReadbackInterval{ 15 };

	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	bool InitializeOpenGL(void);
	void Cleanup(void);

	GLuint CompileShader(const char* source, GLenum type);
	GLuint CreateProgram(void);

	void UpdateCameraFromViewport(void);
	int ReadVisibleCount(void);

	GLuint m_vao{};
	GLuint m_program{};
	GLuint m_counterBuffer{}; // SSBO atomic counter
	GLint m_uViewProjection{ -1 };

	Camera m_camera;
	Particles m_particles;

	int m_instancesCount{};
	float m_cameraMoveTime{};
	int m_readbackFrameCounter{};
	int m_lastVisibleCount{};

protected:
	inline static std::unique_ptr<Renderer> m_instance{};
};
