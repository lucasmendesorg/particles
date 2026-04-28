//
// Particles (c) 2026 Lucas Mendes
//	https://github.com/LucasMendesOrg
//

#include <windows.h>
#include <tchar.h>
#include <Particles/Core.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	auto mainWindow = MainWindow::Initialize(hInstance, nCmdShow);

	if (!mainWindow) {
		return -1;
	}

	if (!Renderer::Initialize()) {
		MessageBox(NULL, TEXT("Cannot initialize Renderer"), TEXT("Error"), MB_OK);
		MainWindow::Shutdown();
		return -1;
	}

	FpsCounter fpsCounter{};

	MSG msg{};

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		Time::GetSingleton()->Update();

		float deltaTime = Time::GetSingleton()->GetDelta();
		auto visibleCount = Renderer::GetSingleton()->Render(deltaTime);
		auto particleCount = Renderer::GetSingleton()->GetTotalParticleCount();

		fpsCounter.Update(deltaTime);

		if (fpsCounter.ConsumeChanged()) {
			TCHAR statusText[160];
			_stprintf_s(statusText, TEXT("FPS: %.02f | Visible particles = %d of %d"),
					    fpsCounter.GetFps(), visibleCount, particleCount);

			mainWindow->SetStatusText(statusText);
		}

		mainWindow->Present();
	}

	Renderer::Shutdown();
	MainWindow::Shutdown();

	return static_cast<int>(msg.wParam);
}
