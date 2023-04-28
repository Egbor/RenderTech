#include "Engine/Object/Component/InputComponent.h"

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(InputComponent)

    InputComponent::InputComponent(const ObjectArgument& argument)
        : Super(argument) {

    }

    void InputComponent::ResetBinds() {
        m_actions.clear();
        m_axis.clear();
    }
}
