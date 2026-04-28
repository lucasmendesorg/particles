#
# Particles (c) 2026 Lucas Mendes
#	https://github.com/LucasMendesOrg
#

add_executable (Particles WIN32
	"main.cpp"
	"MainWindow.cpp"
	"Renderer.cpp"
	"Camera.cpp"
	"Particles.cpp"
	"Time.cpp"
	"FpsCounter.cpp"
	"include/Particles/Core.h"
	"include/Particles/MainWindow.h"
	"include/Particles/Renderer.h"
	"include/Particles/Camera.h"
	"include/Particles/Particles.h"
	"include/Particles/Vertex3.h"
	"include/Particles/Matrix4x4.h"
	"include/Particles/Time.h"
	"include/Particles/FpsCounter.h"
)

set_property(TARGET Particles PROPERTY C_STANDARD 99)
set_property(TARGET Particles PROPERTY C_STANDARD_REQUIRED ON)
set_property(TARGET Particles PROPERTY CXX_STANDARD 20)
set_property(TARGET Particles PROPERTY CXX_STANDARD_REQUIRED ON)

target_compile_definitions(Particles PRIVATE UNICODE _UNICODE)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_options(Particles PRIVATE /DEBUG)
endif()

add_library(glad STATIC ../glad/src/glad.c)

target_include_directories(glad PUBLIC include ../glad/include)

target_link_libraries(Particles glad opengl32)
