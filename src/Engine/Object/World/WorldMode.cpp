#include "Engine/Object/World/WorldMode.h"

namespace Engine {
    //GENERATE_RTTI_DEFINITIONS(WorldMode)
    GENERATE_INSTANTIATION(WorldMode)

    WorldMode::WorldMode(const ObjectArgument& argument)
        : Super(argument) {

    }

    void WorldMode::SetActor(ObjectType* actorClass) {
        m_actorClass = actorClass;
    }

    void WorldMode::SetController(ObjectType* controllerClass) {
        m_controllerClass = controllerClass;
    }

    ObjectType* WorldMode::GetActorClass() const {
        return m_actorClass;
    }

    ObjectType* WorldMode::GetControllerClass() const {
        return m_controllerClass;
    }
}
