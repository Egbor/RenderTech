#include "Engine/Object/World/WorldMode.h"

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(WorldMode)

    WorldMode::WorldMode(const ObjectArgument& argument)
        : Super(argument) {

    }

    void WorldMode::SetActor(MetaClass* actorClass) {
        m_actorClass = actorClass;
    }

    void WorldMode::SetController(MetaClass* controllerClass) {
        m_controllerClass = controllerClass;
    }

    MetaClass* WorldMode::GetActorClass() const {
        return m_actorClass;
    }

    MetaClass* WorldMode::GetControllerClass() const {
        return m_controllerClass;
    }
}
