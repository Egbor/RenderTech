#ifndef IWINDOW_H
#define IWINDOW_H

#include "Engine/EngineDef.h"

namespace Engine {
	class IWindow {
	public:
		virtual ~IWindow() = default;
		virtual Int32 GetWidth() const = 0;
		virtual Int32 GetHeight() const = 0;
		virtual UInt64 WinId() const = 0;

		virtual void Show() const = 0;
		virtual bool HasQuit() const = 0;

		virtual void InvokeErrorMessageBox(const String& message) = 0;
	};
}

#endif // !IWINDOW_H
