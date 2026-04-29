# Particles

Particles is a modern C++20 application that demonstrates real-time particle rendering using Win32, WGL, and OpenGL 4.5 with GLAD for context and extension management. The project features a clean architecture with mathematically focused types, encapsulated operations, and a modular design. It serves as a practical example of efficient rendering, camera control, and performance measurement in a native Windows environment, making it ideal for learning or extending advanced graphics programming techniques.

## Project Guidelines

- Uses Win32, WGL, and OpenGL 4.5 with GLAD.
- Architectural pattern: mathematical types separated by responsibility, each type (e.g., `Vertex3`, `Matrix4x4`) in its own header, with mathematical operations encapsulated as operators or static methods—no global utility functions.
- Static utility methods (e.g., `Renderer::GetTotalParticleCount`) must be declared as `static` in headers and kept consistent between header and source.
- CMake-based build system (minimum version 3.10), using C++20 standard.

## Build Instructions

1. Install [CMake](https://cmake.org/) (version 3.10 or higher) and [Ninja](https://ninja-build.org/).
2. Clone this repository.
3. Generate build files with CMake:
> cmake -G Ninja -B build
4. Build the project:
> cmake --build build

Or simply open with Visual Studio 2022/2026.
