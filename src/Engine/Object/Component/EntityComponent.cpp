#include "Engine/Object/Component/EntityComponent.h"
#include "Engine/Object/Global/ECBStorage.h"

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(EntityComponent)

    EntityComponent::EntityComponent(const ObjectArgument& argument)
        : Super(argument), m_register(nullptr), m_behaviorID(0) {
    }

    HandlerPtr& EntityComponent::GetHandler() {
        return m_register;
    }

    void EntityComponent::Register() {
        m_behaviorID = this->GetBehaviorID();

        IEntityComponentBehavior* behavior = ECBStorage::GetInstance().FindComponentBehavior(m_behaviorID);
        if (behavior != nullptr) {
            m_register = behavior->RegisterComponent(this);
        }
    }

    void EntityComponent::UnRegister() {
        IEntityComponentBehavior* behavior = ECBStorage::GetInstance().FindComponentBehavior(m_behaviorID);
        if (behavior != nullptr) {
            behavior->UnRegisterComponent(this);
        }
        m_behaviorID = 0;
    }

    UInt64 EntityComponent::GetBehaviorID() const {
        return EntityComponent::TypeIdClass();
    }
}
