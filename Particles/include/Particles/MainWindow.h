//
// Particles (c) 2026 Lucas Mendes
//	https://github.com/LucasMendesOrg
//

#pragma once

#include <windows.h>
#include <memory>

class MainWindow
{
public:
	static MainWindow* Initialize(HINSTANCE hInstance, int nCmdShow);
	static MainWindow* GetSingleton(void);
	static void Shutdown(void);

	inline int GetRenderWidth(void) const { return m_renderWidth; }
	inline int GetRenderHeight(void) const { return m_renderHeight; }

	void Present(void);
	void SetStatusText(const wchar_t* text);

	HWND GetHandle(void) const;
	HWND GetRenderHandle(void) const;
	HDC GetDeviceContext(void) const;

	~MainWindow(void);
	MainWindow(void) = default;

private:
	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(const MainWindow&) = delete;

	void CreateMainWindow(HINSTANCE hInstance, int nCmdShow);
	bool InitializeWGL(void);
	void CreateChildWindows(void);
	void LayoutChildWindows(void);
	void Cleanup(void);

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HWND m_hwnd{};
	HWND m_renderHwnd{};
	HWND m_statusHwnd{};
	HDC m_hDC{};
	HGLRC m_hRC{};
	int m_renderWidth{};
	int m_renderHeight{};

protected:
	inline static std::unique_ptr<MainWindow> m_instance{};
};
