#ifndef INPUT_DEF_H
#define INPUT_DEF_H

#include "Engine/EngineDef.h"

namespace Engine {
    enum class InputKey {
        IK_A = 0x41,
        IK_B = 0x42,
        IK_C = 0x43,
        IK_D = 0x44,
        IK_E = 0x45,
        IK_F = 0x46,
        IK_G = 0x47,
        IK_H = 0x48,
        IK_I = 0x49,
        IK_J = 0x4A,
        IK_K = 0x4B,
        IK_L = 0x4C,
        IK_M = 0x4D,
        IK_N = 0x4E,
        IK_O = 0x4F,
        IK_P = 0x50,
        IK_Q = 0x51,
        IK_R = 0x52,
        IK_S = 0x53,
        IK_T = 0x54,
        IK_U = 0x55,
        IK_V = 0x56,
        IK_W = 0x57,
        IK_X = 0x58,
        IK_Y = 0x59,
        IK_Z = 0x5A,
        IK_MOUSE_BUTTON_LEFT = 0x01,
        IK_MOUSE_BUTTON_RIGHT = 0x04,
        IK_MOUSE_BUTTON_MIDDLE = 0x10,
        IK_MOUSE_XBUTTON1 = 0x40,
        IK_MOUSE_XBUTTON2 = 0x100,
        IK_MOUSE_WHEEL = 0x400,
        IK_MOUSE_X = 0xFF00,
        IK_MOUSE_Y = 0xFE00,
        IK_GAMEPAD_THUMB_LEFT_X = 0xFD00,
        IK_GAMEPAD_THUMB_LEFT_Y = 0xFC00,
        IK_GAMEPAD_THUMB_RIGHT_X = 0xFB00,
        IK_GAMEPAD_THUMB_RIGHT_Y = 0xFA00,
        IK_GAMEPAD_TRIGGER_LEFT = 0xF900,
        IK_GAMEPAD_TRIGGER_RIGHT = 0xF800,
        IK_GAMEPAD_DPAD_UP = 0x10001,
        IK_GAMEPAD_DPAD_DOWN = 0x10002,
        IK_GAMEPAD_DPAD_LEFT = 0x10004,
        IK_GAMEPAD_DPAD_RIGHT = 0x10008,
        IK_GAMEPAD_START = 0x10010,
        IK_GAMEPAD_BACK = 0x10020,
        IK_GAMEPAD_LEFT_THUMB = 0x10040,
        IK_GAMEPAD_RIGHT_THUMB = 0x10080,
        IK_GAMEPAD_LEFT_SHOULDER = 0x10100,
        IK_GAMEPAD_RIGHT_SHOULDER = 0x10200,
        IK_GAMEPAD_A = 0x11000,
        IK_GAMEPAD_B = 0x12000,
        IK_GAMEPAD_X = 0x14000,
        IK_GAMEPAD_Y = 0x18000
    };

    enum class InputEvent {
        IE_RELEASED = 0x00,
        IE_PRESSED = 0x3f800000,
    };

    template<class TBindValue>
    struct InputBinding {
        InputKey inputKey;
        TBindValue inputValue;
    };

    struct InputValue {
        InputValue() : axisValue(0.0f) {};
        InputValue(Float value) : axisValue(value) {};
        InputValue(InputEvent value) : actionValue(value) {};

        union {
            Float axisValue;
            InputEvent actionValue;
        };
    };
}

#endif // !INPUT_DEF_H
