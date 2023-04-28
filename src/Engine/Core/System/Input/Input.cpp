#include "Engine/Core/System/Input/Input.h"

#include <cmath>

namespace Engine {
    Input::Input()
        : m_inputMap(), m_inputSettings() {

    }

    InputMap& Input::GetInputMap() {
        return m_inputMap;
    }

    void Input::SetupAction(const String& tag, InputKey key, InputEvent event) {
        m_inputMap.RegisterKey(key);
        m_inputSettings.SetAction(tag, key, event);
    }

    void Input::SetupAxis(const String& tag, InputKey key, Float value) {
        m_inputMap.RegisterKey(key);
        m_inputSettings.SetAxis(tag, key, value);
    }

    void Input::HandleAction(const String& tag, Event<>& event) {
        Float value = 0.0f;
        Map<InputKey, InputEvent> actionMap;
        m_inputSettings.GetActions(tag, actionMap);

        for (auto item : actionMap) {
            if (m_inputMap.HasKeyInteraction(item.first, value) &&
                (static_cast<Int32>(item.second) == static_cast<Int32>(value))) {
                event();
            }
        }
    }

    void Input::HandleAxis(const String& tag, Event<Float>& event) {
        Float value = 0.0f;
        Map<InputKey, Float> axisMap;
        m_inputSettings.GetAxis(tag, axisMap);

        for (auto item : axisMap) {
            if (m_inputMap.HasKeyInteraction(item.first, value) && (std::abs(value) > 0.001f)) {
                event(item.second * value);
            }
        }
    }
}
