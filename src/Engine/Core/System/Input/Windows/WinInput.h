#ifndef WININPUT_H
#define WININPUT_H

#include "Engine/Core/System/Input/Input.h"

namespace Engine {
    class WinInput : public Input {
    public:
        WinInput();
        virtual ~WinInput() = default;

        UInt32 GetGamepadId() const;

    private:
        void Initialize();
        void InitializeRawDevices();
        void InitializeXInputDevices();
        void InitializeGlobalCallbacks();
    };
}

#endif // WININPUT_H
