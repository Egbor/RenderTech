#include "Engine/Object/Component/InputComponent.h"

namespace Engine {
    GENERATE_INSTANTIATION(InputComponent)

    InputComponent::InputComponent(const ObjectArgument& argument)
        : Super(argument) {

    }

    void InputComponent::ResetBinds() {
        m_actions.clear();
        m_axis.clear();
    }
}
