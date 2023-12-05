#ifndef WINDOW_H
#define WINDOW_H

#include "Engine/Core/System/Platform/Interface/IWindow.h"
#include "Engine/Core/System/Platform/Interface/IInputable.h"
#include "Engine/Core/System/Platform/Win32/Win32Input.h"

#include <Windows.h>

namespace Engine {
	class Win32Window : public IWindow, public IInputable {
	public:
		Win32Window(HINSTANCE hInstance, Int32 width, Int32 height);
		virtual ~Win32Window() = default;

		Int32 GetWidth() const override;
		Int32 GetHeight() const override;
		UInt64 WinId() const override;

		void Show() override;
		bool HasQuit() const override;

		Input* GetInput() const override;

	private:
		static Win32Input& GetWin32Input();
		static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		HWND m_winId;
		String m_title;

		Win32Input* m_input;
	};
}

#endif // !WINDOW_H