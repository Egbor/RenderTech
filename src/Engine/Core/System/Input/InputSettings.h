#ifndef INPUTSETTINGS_H
#define INPUTSETTINGS_H

#include "Engine/EngineDef.h"

namespace Engine {
    class InputSettings {
    private:
        Map<String, Map<InputKey, InputEvent>> m_actions;
        Map<String, Map<InputKey, Float>> m_axis;

    public:
        InputSettings() = default;
        virtual ~InputSettings() = default;

        void SetAction(const String& tag, InputKey key, InputEvent event);
        void SetAxis(const String& tag, InputKey key, Float value);

        bool GetActions(const String& tag, Map<InputKey, InputEvent>& outActions);
        bool GetAxis(const String& tag, Map<InputKey, Float>& outAxis);
    };
}

#endif // INPUTSETTINGS_H
