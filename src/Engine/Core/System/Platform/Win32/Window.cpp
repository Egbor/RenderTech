#include "Engine/Core/System/Platform/Win32/Window.h"

constexpr static const char* gWindowClass = "RenderTech";

namespace Engine {
	LRESULT WINAPI Win32Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	Win32Window::Win32Window(HINSTANCE hInstance, Int32 width, Int32 height) 
		: m_title("RTDemo") {
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

		RECT rect = { 0, 0, width, height };
		AdjustWindowRect(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false);
		m_winId = CreateWindowEx(NULL, gWindowClass, m_title.c_str(), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

		if (!m_winId) {
			MessageBox(NULL, TEXT("Window creation error occure!"), TEXT("Error"), MB_OK | MB_ICONERROR);
			exit(-1);
		}
	}

	Int32 Win32Window::GetWidth() const {
		RECT rectWindow;
		GetClientRect(m_winId, &rectWindow);
		return static_cast<Int32>(rectWindow.right - rectWindow.left);
	}

	Int32 Win32Window::GetHeight() const {
		RECT rectWindow;
		GetClientRect(m_winId, &rectWindow);
		return static_cast<Int32>(rectWindow.bottom - rectWindow.top);
	}

	UInt64 Win32Window::WinId() const {
		return reinterpret_cast<UInt64>(m_winId);
	}

	void Win32Window::Show() {
		ShowWindow(m_winId, SW_SHOWDEFAULT);
		UpdateWindow(m_winId);
	}

	bool Win32Window::HasQuit() const {
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return msg.message == WM_QUIT;
	}
}