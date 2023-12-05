#include "Engine.h"

#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Platform/Win32/Window.h"

#include "Engine/Core/Render/Api/DX11/DX11Def.h"

//#include "imgui/imgui.h"
//
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//
//constexpr static const char* gWindowClass = "RenderTechClass";
//constexpr static const char* gWindowName = "RenderTech";
//
//constexpr static const UINT gWindowWidth = 800;
//constexpr static const UINT gWindowHeight = 600;
//
//WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };
//
//using namespace Engine;
//
//void SwitchWindowToFullscreen(HWND hWnd) {
//	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
//	if (dwStyle & WS_OVERLAPPEDWINDOW) {
//		MONITORINFO mi = { sizeof(mi) };
//		if (GetWindowPlacement(hWnd, &g_wpPrev) &&
//			GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &mi)) {
//			SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
//			SetWindowPos(hWnd, HWND_TOP,
//				mi.rcMonitor.left, mi.rcMonitor.top,
//				mi.rcMonitor.right - mi.rcMonitor.left,
//				mi.rcMonitor.bottom - mi.rcMonitor.top,
//				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
//		}
//	} else {
//		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
//		SetWindowPlacement(hWnd, &g_wpPrev);
//		SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
//			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
//	}
//}
//
//LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
//	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
//		return true;
//	}
//
//	switch (msg) {
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	}
//	return DefWindowProc(hWnd, msg, wParam, lParam);
//}

#include "Scripts/Spectator.h"
#include "Scripts/VisibleObject.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Engine::IWindow* window = new Engine::Win32Window(hInstance, 800, 600);
	Engine::IContext* context = new Engine::DX11Context(window);

	int width = window->GetWidth();
	int height = window->GetHeight();

	Engine::EngineClass* engine = new Engine::EngineClass(window, context);
	Engine::World* world = Engine::ClassType<Engine::World>::CreateObject(Engine::ObjectArgument::Dummy());

	world->SpawnEntity(Engine::ClassOf<Engine::Spectator>::value, Engine::Vector3(0.0f, 0.0f, -1.0f), Engine::Rotator(0.0f, 0.0f, 0.0f));
	world->SpawnEntity(Engine::ClassOf<Engine::VisibleObject>::value, Engine::Vector3(0.0f, 0.0f, 0.0f), Engine::Rotator(0.0f, 0.0f, 0.0f));

	window->Show();
	engine->Run(world);

	DELETE_OBJECT(world);
	DELETE_OBJECT(engine);

	DELETE_OBJECT(context);
	DELETE_OBJECT(window);

#if defined(DEBUG) || defined(_DEBUG)
	ComPtr<IDXGIDebug1> dxgiDebug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_IDXGIDebug1, &dxgiDebug))) {
		dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
	}
#endif

	//WNDCLASSEX wndClass;
	//ZeroMemory(&wndClass, sizeof(wndClass));

	//wndClass.cbSize = sizeof(wndClass);
	//wndClass.style = CS_HREDRAW | CS_VREDRAW;
	//wndClass.lpfnWndProc = WndProc;
	//wndClass.hInstance = hInstance;
	//wndClass.lpszClassName = gWindowClass;
	//wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wndClass.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);

	//RegisterClassEx(&wndClass);
	//HWND hWnd = CreateWindowEx(NULL, gWindowClass, gWindowName, WS_OVERLAPPEDWINDOW, 0, 0, gWindowWidth, gWindowHeight, NULL, NULL, hInstance, NULL);

	//if (!hWnd) {
	//	MessageBox(NULL, TEXT("Window creation error occure!"), TEXT("Error"), MB_OK | MB_ICONERROR);
	//	exit(-1);
	//}

	//ShowWindow(hWnd, SW_SHOWDEFAULT);
	//SwitchWindowToFullscreen(hWnd);
	//UpdateWindow(hWnd);

	//RECT rectWindow;
	//GetWindowRect(hWnd, &rectWindow);
	//UINT currWindowWidth = rectWindow.right - rectWindow.left;
	//UINT currWindowHeight = rectWindow.bottom - rectWindow.top;

	//EngineClass engine((UInt64)hWnd, currWindowWidth, currWindowHeight);

	//MSG msg;
	//ZeroMemory(&msg, sizeof(msg));

	//engine.InvokeStartStage();
	//while (msg.message != WM_QUIT) {
	//	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	} else {
	//		engine.InvokeUpdateStage();
	//		engine.InvokeRenderStage();
	//	}
	//}

	return 0;
}
