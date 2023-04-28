#include "Engine/Core/System/Input/InputSettings.h"

namespace Engine {
    void InputSettings::SetAction(const String& tag, InputKey key, InputEvent event) {
        m_actions[tag][key] = event;
    }

    void InputSettings::SetAxis(const String& tag, InputKey key, Float value) {
        m_axis[tag][key] = value;
    }

    bool InputSettings::GetActions(const String& tag, Map<InputKey, InputEvent>& outActions) {
        if (m_actions.find(tag) != m_actions.end()) {
            outActions = m_actions[tag];
            return true;
        }
        return false;
    }

    bool InputSettings::GetAxis(const String& tag, Map<InputKey, Float>& outAxis) {
        if (m_axis.find(tag) != m_axis.end()) {
            outAxis = m_axis[tag];
            return true;
        }
        return false;
    }
}
