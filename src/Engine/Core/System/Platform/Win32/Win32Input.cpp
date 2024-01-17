#include "Engine/Core/System/Platform/Win32/Win32Input.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include <sstream>

namespace Engine {
	void Win32Input::InitializeInputDevices() {
		RAWINPUTDEVICE rid[2];

		rid[0].usUsagePage = 0x01;
		rid[0].usUsage = 0x02;
		rid[0].dwFlags = 0;
		rid[0].hwndTarget = 0;

		rid[1].usUsagePage = 0x01;
		rid[1].usUsage = 0x06;
		rid[1].dwFlags = 0;
		rid[1].hwndTarget = 0;

		if (!RegisterRawInputDevices(rid, sizeof(rid) / sizeof(rid[0]), sizeof(rid[0]))) {
			throw EngineException("[Win32Input] RegisterRawInputDevices() failed");
		}
	}

	void Win32Input::ResetInputDevices() {
		SetValueInKeymap(InputKey::IK_MOUSE_X, static_cast<InputValue>(0.0f));
		SetValueInKeymap(InputKey::IK_MOUSE_Y, static_cast<InputValue>(0.0f));
	}

	void Win32Input::UpdateInputDevices(WPARAM wParam, LPARAM lParam) {
		UINT dataSize = 0;

		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
		if (dataSize <= 0) {
			return;
		}

		Array<BYTE> rawdata(dataSize);
		if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, &rawdata[0], &dataSize, sizeof(RAWINPUTHEADER)) == dataSize) {
			RAWINPUT* rawinput = reinterpret_cast<RAWINPUT*>(&rawdata[0]);
			if (rawinput->header.dwType == RIM_TYPEKEYBOARD) {
				SetValueInKeymap(static_cast<InputKey>(rawinput->data.keyboard.VKey), static_cast<InputValue>(!rawinput->data.keyboard.Flags));
			} else if (rawinput->header.dwType == RIM_TYPEMOUSE) {
				SetValueInKeymap(InputKey::IK_MOUSE_X, static_cast<InputValue>(rawinput->data.mouse.lLastX));
				SetValueInKeymap(InputKey::IK_MOUSE_Y, static_cast<InputValue>(rawinput->data.mouse.lLastY));
			}
		}
	}

	void Win32Input::Refresh() {
		ResetInputDevices();
	}
}