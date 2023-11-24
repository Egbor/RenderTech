#include "Engine/Object/Component/EntityComponent.h"

namespace Engine {
    GENERATE_INSTANTIATION(EntityComponent)

    EntityComponent::EntityComponent(const ObjectArgument& argument)
        : Super(argument), m_register(nullptr), m_behaviorID(0) {
    }

    void EntityComponent::CreateRenderState(IRenderPass* pass) {

    }

    HandlerPtr& EntityComponent::GetHandler() {
        return m_register;
    }

    UInt64 EntityComponent::GetBehaviorID() const {
        return EntityComponent::TypeIdClass();
    }
}
