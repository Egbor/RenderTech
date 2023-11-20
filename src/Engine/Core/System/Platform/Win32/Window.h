#include "Engine/Core/System/Platform/Interface/IWindow.h"

#include <Windows.h>

namespace Engine {
	class Win32Window : public IWindow {
	public:
		Win32Window(HINSTANCE hInstance, Int32 width, Int32 height);
		virtual ~Win32Window() = default;

		Int32 GetWidth() const override;
		Int32 GetHeight() const override;
		UInt64 WinId() const override;

		void Show() override;
		bool HasQuit() const override;

	private:
		HWND m_winId;
		String m_title;
	};
}