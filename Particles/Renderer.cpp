//
// Particles (c) 2026 Lucas Mendes
//	https://github.com/LucasMendesOrg
//

#include <Particles/Renderer.h>
#include <Particles/MainWindow.h>

#include <cstdio>
#include <cmath>

static const char vertexShaderSource[]{
	"#version 450 core\n"
	"layout(std430, binding = 0) readonly buffer PositionsBuffer {\n"
	"	vec4 positions[];\n"
	"};\n"
	"layout(std430, binding = 1) readonly buffer ColorsBuffer {\n"
	"	vec4 colors[];\n"
	"};\n"
	"layout(std430, binding = 2) buffer Counter {\n"
	"	uint visibleCount;\n"
	"};\n"
	"uniform mat4 uViewProjection;\n"
	"out vec4 vColor;\n"
	"void main() {\n"
	"	vec4 worldPos = positions[gl_InstanceID];\n"
	"	vec4 clipPos = uViewProjection * worldPos;\n"
	"	bool visible = (clipPos.w > 0.0)\n"
	"		&& (clipPos.x >= -clipPos.w && clipPos.x <= clipPos.w)\n"
	"		&& (clipPos.y >= -clipPos.w && clipPos.y <= clipPos.w)\n"
	"		&& (clipPos.z >= -clipPos.w && clipPos.z <= clipPos.w);\n"
	"	if (visible) {\n"
	"		atomicAdd(visibleCount, 1u);\n"
	"	}\n"
	"	vColor = colors[gl_InstanceID];\n"
	"	gl_PointSize = visible ? 24.0 : 0.0;\n"
	"	gl_Position = visible ? clipPos : vec4(2.0, 2.0, 2.0, 1.0);\n"
	"}\n"
};

static const char fragmentShaderSource[]{
	"#version 460 core\n"
	"in vec4 vColor;\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"	FragColor = vColor;\n"
	"}\n"
};

int Renderer::GetTotalParticleCount(void)
{
	return particleCount;
}

Renderer::~Renderer(void)
{
	Cleanup();
}

Renderer* Renderer::Initialize(void)
{
	if (!m_instance) {
		m_instance = std::make_unique<Renderer>();

		if (!m_instance->InitializeOpenGL()) {
			m_instance.reset();
			return nullptr;
		}
	}

	return m_instance.get();
}

Renderer* Renderer::GetSingleton(void)
{
	return m_instance.get();
}

void Renderer::Shutdown(void)
{
	m_instance.reset();
}

bool Renderer::InitializeOpenGL(void)
{
	if (!gladLoadGL()) {
		return false;
	}

	glGenVertexArrays(1, &m_vao);

	m_program = CreateProgram();
	if (!m_program) {
		return false;
	}

	m_uViewProjection = glGetUniformLocation(m_program, "uViewProjection");
	if (m_uViewProjection < 0) {
		return false;
	}

	if (!m_particles.Initialize(particleCount)) {
		return false;
	}

	m_instancesCount = m_particles.GetCount();
	m_readbackFrameCounter = 0;
	m_lastVisibleCount = 0;

	glGenBuffers(1, &m_counterBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_counterBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptr>(sizeof(GLuint)), nullptr, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_counterBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	UpdateCameraFromViewport();

	glClearColor(0.04f, 0.04f, 0.07f, 1.0f);

	return true;
}

int Renderer::Render(float deltaTime)
{
	m_cameraMoveTime += deltaTime;

	if (!m_program) {
		return 0;
	}

	UpdateCameraFromViewport();

	glClear(GL_COLOR_BUFFER_BIT);

	GLuint zero{};
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_counterBuffer);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, static_cast<GLsizeiptr>(sizeof(zero)), &zero);

	glUseProgram(m_program);
	glUniformMatrix4fv(m_uViewProjection, 1, GL_FALSE, m_camera.GetViewProjection().m);

	m_particles.BindSsbo();

	glBindVertexArray(m_vao);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_counterBuffer);

	glDrawArraysInstanced(GL_POINTS, 0, 1, m_instancesCount);

	++m_readbackFrameCounter;
	if (m_readbackFrameCounter >= visibleReadbackInterval) {
		m_lastVisibleCount = ReadVisibleCount();
		m_readbackFrameCounter = 0;
	}

	return m_lastVisibleCount;
}

void Renderer::Cleanup(void)
{
	m_particles.Cleanup();

	if (m_counterBuffer) {
		glDeleteBuffers(1, &m_counterBuffer);
		m_counterBuffer = 0;
	}

	if (m_program) {
		glDeleteProgram(m_program);
		m_program = 0;
	}

	if (m_vao) {
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}

	m_instancesCount = 0;
	m_cameraMoveTime = 0.0f;
	m_readbackFrameCounter = 0;
	m_lastVisibleCount = 0;
}

GLuint Renderer::CompileShader(const char* source, GLenum type)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	int status{};
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (!status) {
		char infoLog[1024]{};
		glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
		std::printf("Shader compile error: %s\n", infoLog);
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

GLuint Renderer::CreateProgram(void)
{
	GLuint vertexShader = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
	if (!vertexShader) {
		return 0;
	}

	GLuint fragmentShader = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
	if (!fragmentShader) {
		glDeleteShader(vertexShader);
		return 0;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	int linked{};
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	if (!linked) {
		char infoLog[1024]{};
		glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
		std::printf("Program link error: %s\n", infoLog);
		glDeleteProgram(program);
		return 0;
	}

	return program;
}

void Renderer::UpdateCameraFromViewport(void)
{
	MainWindow* mainWindow = MainWindow::GetSingleton();

	float width = static_cast<float>(mainWindow->GetRenderWidth());
	float height = static_cast<float>(mainWindow->GetRenderHeight());
	float aspect = (height > 0.0f) ? (width / height) : (4.0f / 3.0f);

	float cameraX = std::sin(m_cameraMoveTime * 0.20f) * 2.5f;
	float cameraY = std::cos(m_cameraMoveTime * 0.12f) * 1.2f;

	m_camera.SetPerspective(60.0f, aspect, 0.1f, 100.0f);
	m_camera.SetLookAt({ cameraX, cameraY, 4.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
}

int Renderer::ReadVisibleCount(void)
{
	GLuint visible = 0u;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_counterBuffer);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, static_cast<GLsizeiptr>(sizeof(visible)), &visible);
	return static_cast<int>(visible);
}
