#ifndef WIN32_INPUT_H
#define WIN32_INPUT_H

#include <Windows.h>
#include "Engine/Core/System/Platform/Common/Input.h"

namespace Engine {
	class Win32Input : public Input {
	public:
		Win32Input() = default;
		virtual ~Win32Input() = default;

		void InitializeInputDevices();
		void ResetInputDevices(const MSG* msg);
		void UpdateInputDevices(WPARAM wParam, LPARAM lParam);
	};
}

#endif // !WIN32_INPUT_H
