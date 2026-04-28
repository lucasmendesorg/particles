//
// Particles (c) 2026 Lucas Mendes
//	https://github.com/LucasMendesOrg
//

#include <Particles/MainWindow.h>

MainWindow::~MainWindow(void)
{
	Cleanup();
}

MainWindow* MainWindow::Initialize(HINSTANCE hInstance, int nCmdShow)
{
	if (!m_instance) {
		m_instance = std::make_unique<MainWindow>();
		m_instance->CreateMainWindow(hInstance, nCmdShow);
	}

	return m_instance.get();
}

MainWindow* MainWindow::GetSingleton(void)
{
	return m_instance.get();
}

void MainWindow::Shutdown(void)
{
	m_instance.reset();
}

void MainWindow::Present(void)
{
	if (m_hDC) {
		SwapBuffers(m_hDC);
	}
}

void MainWindow::SetStatusText(const wchar_t* text)
{
	if (m_statusHwnd) {
		SetWindowTextW(m_statusHwnd, text ? text : L"");
	}
}

HWND MainWindow::GetHandle(void) const
{
	return m_hwnd;
}

HWND MainWindow::GetRenderHandle(void) const
{
	return m_renderHwnd;
}

HDC MainWindow::GetDeviceContext(void) const
{
	return m_hDC;
}

void MainWindow::CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	static bool registered{};
	if (!registered) {
		WNDCLASS wc{};
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = MainWindow::WindowProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = TEXT("MainWindow");

		RegisterClass(&wc);
		registered = true;
	}

	m_hwnd = ::CreateWindow(
		TEXT("MainWindow"), TEXT("Particles"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 1024,
		NULL, NULL, hInstance, this
	);

	if (!m_hwnd) {
		PostQuitMessage(-1);
		return;
	}

	ShowWindow(m_hwnd, nCmdShow);
}

void MainWindow::CreateChildWindows(void)
{
	m_renderHwnd = ::CreateWindowEx(
		0,
		TEXT("STATIC"),
		TEXT(""),
		WS_CHILD | WS_VISIBLE,
		0, 0, 100, 100,
		m_hwnd,
		NULL,
		reinterpret_cast<HINSTANCE>(GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE)),
		NULL
	);

	m_statusHwnd = ::CreateWindowEx(
		0,
		TEXT("STATIC"),
		TEXT(""),
		WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP,
		0, 0, 100, 24,
		m_hwnd,
		NULL,
		reinterpret_cast<HINSTANCE>(GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE)),
		NULL
	);

	LayoutChildWindows();
}

void MainWindow::LayoutChildWindows(void)
{
	if (!m_hwnd || !m_renderHwnd || !m_statusHwnd) {
		return;
	}

	RECT rc{};
	GetClientRect(m_hwnd, &rc);

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	int statusHeight = 28;

	if (statusHeight > height) {
		statusHeight = height;
	}

	int renderHeight = height - statusHeight;
	if (renderHeight < 1) {
		renderHeight = 1;
	}

	MoveWindow(m_renderHwnd, 0, 0, width, renderHeight, TRUE);
	MoveWindow(m_statusHwnd, 0, height - statusHeight, width, statusHeight, TRUE);

	m_renderWidth = width;
	m_renderHeight = renderHeight;
}

bool MainWindow::InitializeWGL(void)
{
	static PIXELFORMATDESCRIPTOR pfd{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		24, 8, 0,
		PFD_MAIN_PLANE,
		0, 0, 0, 0
	};

	m_hDC = GetDC(m_renderHwnd);
	if (!m_hDC) {
		return false;
	}

	int pixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	if (!pixelFormat) {
		return false;
	}

	if (!SetPixelFormat(m_hDC, pixelFormat, &pfd)) {
		return false;
	}

	m_hRC = wglCreateContext(m_hDC);
	if (!m_hRC) {
		return false;
	}

	if (!wglMakeCurrent(m_hDC, m_hRC)) {
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
		return false;
	}

	return true;
}

void MainWindow::Cleanup(void)
{
	if (m_hRC) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}

	if (m_hDC && m_renderHwnd) {
		ReleaseDC(m_renderHwnd, m_hDC);
		m_hDC = NULL;
	}

	m_renderHwnd = NULL;
	m_statusHwnd = NULL;
	m_hwnd = NULL;
}

LRESULT CALLBACK MainWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MainWindow* instance = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	switch (uMsg) {
		case WM_CREATE: {
			auto createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			instance = static_cast<MainWindow*>(createStruct->lpCreateParams);

			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(instance));
			instance->m_hwnd = hwnd;

			instance->CreateChildWindows();

			if (!instance->InitializeWGL()) {
				MessageBox(NULL, TEXT("Cannot initialize WGL"), TEXT("Error"), MB_OK);
				PostQuitMessage(-1);
			}
			return 0;
		}

		case WM_SIZE:
			if (instance) {
				instance->LayoutChildWindows();
			}
			return 0;

		case WM_CLOSE:
			DestroyWindow(hwnd);
			return 0;

		case WM_DESTROY:
			if (instance) {
				instance->Cleanup();
			}

			SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);

			PostQuitMessage(0);
			return 0;

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
