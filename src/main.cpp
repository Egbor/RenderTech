#include <Windows.h>

#include "Engine.h"

constexpr static const char* gWindowClass = "RenderTechClass";
constexpr static const char* gWindowName = "RenderTech";

constexpr static const UINT gWindowWidth = 800;
constexpr static const UINT gWindowHeight = 600;

WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };

using namespace Engine;

void SwitchWindowToFullscreen(HWND hWnd) {
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	if (dwStyle & WS_OVERLAPPEDWINDOW) {
		MONITORINFO mi = { sizeof(mi) };
		if (GetWindowPlacement(hWnd, &g_wpPrev) &&
			GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &mi)) {
			SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(hWnd, HWND_TOP,
				mi.rcMonitor.left, mi.rcMonitor.top,
				mi.rcMonitor.right - mi.rcMonitor.left,
				mi.rcMonitor.bottom - mi.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	} else {
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(hWnd, &g_wpPrev);
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));

	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = gWindowClass;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);

	RegisterClassEx(&wndClass);
	HWND hWnd = CreateWindowEx(NULL, gWindowClass, gWindowName, WS_OVERLAPPEDWINDOW, 0, 0, gWindowWidth, gWindowHeight, NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		MessageBox(NULL, TEXT("Window creation error occure!"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(-1);
	}

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	SwitchWindowToFullscreen(hWnd);
	UpdateWindow(hWnd);

	RECT rectWindow;
	GetWindowRect(hWnd, &rectWindow);
	UINT currWindowWidth = rectWindow.right - rectWindow.left;
	UINT currWindowHeight = rectWindow.bottom - rectWindow.top;

	EngineClass engine((UInt64)hWnd, currWindowWidth, currWindowHeight);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	engine.InvokeStartStage();
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			engine.InvokeUpdateStage();
			engine.InvokeRenderStage();
		}
	}

	return 0;
}
