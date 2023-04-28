#include "Engine/Object/EntityController/EntityController.h"
#include "Engine/Object/Global/EngineConfig.h"

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(EntityController)

    EntityController::EntityController(const ObjectArgument& argument)
        : Super(argument), m_actor(nullptr) {
        m_component = CreateDefaultSubobject<InputComponent>();
    }

    void EntityController::AttachActor(Actor* actor) {
        m_actor = actor;
        m_actor->SetupInputComponent(m_component);
    }

    void EntityController::DetachActor() {
        m_actor = nullptr;
        m_component->ResetBinds();
    }

    void EntityController::Execute() {
        Input* input = EngineConfig::GetInstance().GetInput();
        m_component->ForEachAction([&](const String& tag, Event<>& action){ input->HandleAction(tag, action); });
        m_component->ForEachAxis([&](const String& tag, Event<Float>& axis){ input->HandleAxis(tag, axis); });
    }
}
