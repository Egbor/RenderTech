#include "Engine/Core/System/Input/InputMap.h"

#include <cmath>

namespace Engine {
    void InputMap::SetKeyInteraction(InputKey key, Float inValue) {
        if (m_generalKeyMap.find(key) != m_generalKeyMap.end()) {
            m_generalKeyMap[key] = inValue;
        }
    }

    bool InputMap::HasKeyInteraction(InputKey key, Float& outValue) {
        if (m_generalKeyMap.find(key) != m_generalKeyMap.end()) {
            outValue = m_generalKeyMap[key];
            m_generalKeyMap[key] = static_cast<Float>(static_cast<Int32>(outValue));
            return std::abs(outValue) > 0.0f;
        }
        return false;
    }

    void InputMap::RegisterKey(InputKey key) {
        m_generalKeyMap[key] = 0.0f;
    }

    void InputMap::UnRegisterKey(InputKey key) {
        m_generalKeyMap.erase(key);
    }
}
