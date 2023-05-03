#include "Engine/Core/System/Input/Windows/WinInput.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include <Windows.h>
#include <Xinput.h>
#include <functional>

#include <algorithm>

#include <sstream>

#define MOUSE_SACLER 0.01f

namespace Engine {
    DWORD nativeScreenWidth;
    DWORD nativeScreenHeight;
    DWORD gGamepadId;

    std::function<void(InputKey, Float)> gInputMapInvoker;

    void HandleKeyboardInput(RAWKEYBOARD& keyboard) {
        InputKey key = static_cast<InputKey>(keyboard.VKey);
        Float value = static_cast<Float>(!keyboard.Flags);
        gInputMapInvoker(key, (value < 1.0f) ? value + 0.0001f : value);
    }

    void HandleMouseInput(RAWMOUSE& mouse) {
        if (mouse.usButtonFlags == 0) {
      //std::stringstream ss;
      // ss << "X: " << mouse.lLastX << std::endl
      //    << "Y: " << mouse.lLastY << std::endl;
      // OutputDebugStringA(ss.str().c_str());
            Float xValue = std::clamp(mouse.lLastX * MOUSE_SACLER, -1.0f, 1.0f);
            Float yValue = std::clamp(mouse.lLastY * MOUSE_SACLER, -1.0f, 1.0f);
            gInputMapInvoker(InputKey::IK_MOSUE_X, xValue);
            gInputMapInvoker(InputKey::IK_MOSUE_Y, yValue);
        } else if (mouse.usButtonFlags < RI_MOUSE_WHEEL) {
            InputKey key = static_cast<InputKey>(mouse.usButtonFlags >> (!!(mouse.usButtonFlags & 0x2AA)));
            Float value = static_cast<Float>(!(mouse.usButtonFlags & 0x2AA));
            gInputMapInvoker(key, value);
        } else if (mouse.usButtonFlags == RI_MOUSE_WHEEL) {
            Float wheelValue = static_cast<Int16>(mouse.usButtonData) / static_cast<Float>(WHEEL_DELTA);
            gInputMapInvoker(InputKey::IK_MOSUE_WHEEL, wheelValue);
        }
    }

    void HandleGamepadInput() {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        XInputGetState(gGamepadId, &state);

        gInputMapInvoker(InputKey::IK_GAMEPAD_THUMB_LEFT_X, fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767));
        gInputMapInvoker(InputKey::IK_GAMEPAD_THUMB_LEFT_Y, fmaxf(-1, (float)state.Gamepad.sThumbLY / 32767));
        gInputMapInvoker(InputKey::IK_GAMEPAD_THUMB_RIGHT_X, fmaxf(-1, (float)state.Gamepad.sThumbRX / 32767));
        gInputMapInvoker(InputKey::IK_GAMEPAD_THUMB_RIGHT_Y, fmaxf(-1, (float)state.Gamepad.sThumbRY / 32767));
        gInputMapInvoker(InputKey::IK_GAMEPAD_TRIGGER_LEFT, (float)state.Gamepad.bLeftTrigger / 255);
        gInputMapInvoker(InputKey::IK_GAMEPAD_TRIGGER_RIGHT, (float)state.Gamepad.bRightTrigger / 255);

//        std::stringstream ss;
//        ss << "X: " << fmaxf(-1, (float)state.Gamepad.sThumbRX / 32767) << std::endl
//           << "Y: " << fmaxf(-1, (float)state.Gamepad.sThumbRY / 32767) << std::endl;
//        OutputDebugStringA(ss.str().c_str());

        for (Int32 mask = 0x0001; mask < 0x10000; mask <<= 1) {
            InputKey key = static_cast<InputKey>(0x10000 + mask);
            Float value = static_cast<Float>(!!(state.Gamepad.wButtons & mask));
            gInputMapInvoker(key, value);
        }
    }

    LRESULT CALLBACK HookWndProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode < 0) {
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }

        switch (nCode) {
        case HC_ACTION:
        {
            LPMSG wndProcArgs = (LPMSG)lParam;
            if (wndProcArgs->message == WM_INPUT) {
                UINT dataSize;
                GetRawInputData(reinterpret_cast<HRAWINPUT>(wndProcArgs->lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
                if (dataSize > 0) {
                    std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
                    if (GetRawInputData(reinterpret_cast<HRAWINPUT>(wndProcArgs->lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize) {
                        RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
                        if (raw->header.dwType == RIM_TYPEKEYBOARD) {
                            HandleKeyboardInput(raw->data.keyboard);
                        } else if (raw->header.dwType == RIM_TYPEMOUSE) {
                            HandleMouseInput(raw->data.mouse);
                        } else {
                            HandleGamepadInput();
                        }
                    }
                }
            }
            break;
        }
        default:
            break;
        }

        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    WinInput::WinInput() {
        Initialize();
    }

    UInt32 WinInput::GetGamepadId()  const {
        return gGamepadId;
    }

    void WinInput::Initialize() {
        InitializeRawDevices();
        InitializeXInputDevices();
        InitializeGlobalCallbacks();

        SetWindowsHookExA(WH_GETMESSAGE, HookWndProc, NULL, GetCurrentThreadId());
    }

    void WinInput::InitializeRawDevices() {
        RAWINPUTDEVICE Rid[3];

        Rid[0].usUsagePage = 0x01;
        Rid[0].usUsage = 0x02;
        Rid[0].dwFlags = 0;
        Rid[0].hwndTarget = 0;

        Rid[1].usUsagePage = 0x01;
        Rid[1].usUsage = 0x05;
        Rid[1].dwFlags = 0;
        Rid[1].hwndTarget = 0;

        Rid[2].usUsagePage = 0x01;
        Rid[2].usUsage = 0x06;
        Rid[2].dwFlags = 0;
        Rid[2].hwndTarget = 0;

        if (RegisterRawInputDevices(Rid, 3, sizeof(Rid[0])) == FALSE) {
            throw EngineException("[WinInput] RegisterRawInputDevice() failed");
        }
    }

    void WinInput::InitializeXInputDevices() {
        DWORD dwResult;
        for (gGamepadId = 0; gGamepadId < XUSER_MAX_COUNT; gGamepadId++ ) {
            XINPUT_STATE state;
            ZeroMemory( &state, sizeof(XINPUT_STATE) );

            dwResult = XInputGetState( gGamepadId, &state );
            if(dwResult == ERROR_SUCCESS) {
                break;
            }
        }
    }

    void WinInput::InitializeGlobalCallbacks() {
        nativeScreenWidth = GetSystemMetrics(SM_CYSCREEN);
        nativeScreenHeight = GetSystemMetrics(SM_CXSCREEN);

        gInputMapInvoker = [&](InputKey key, Float value) {
            GetInputMap().SetKeyInteraction(key, value);
        };
    }
}
