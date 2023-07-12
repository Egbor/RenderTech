#include "Engine/Object/World/WorldMode.h"

namespace Engine {
    //GENERATE_RTTI_DEFINITIONS(WorldMode)
    GENERATE_INSTANTIATION(WorldMode)

    WorldMode::WorldMode(const ObjectArgument& argument)
        : Super(argument) {

    }

    void WorldMode::SetActor(IClass* actorClass) {
        m_actorClass = actorClass;
    }

    void WorldMode::SetController(IClass* controllerClass) {
        m_controllerClass = controllerClass;
    }

    IClass* WorldMode::GetActorClass() const {
        return m_actorClass;
    }

    IClass* WorldMode::GetControllerClass() const {
        return m_controllerClass;
    }
}
