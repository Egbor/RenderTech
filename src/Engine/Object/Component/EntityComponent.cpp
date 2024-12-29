#include "Engine/Object/Component/EntityComponent.h"

namespace Engine {
    GENERATE_INSTANTIATION(EntityComponent)

    EntityComponent::EntityComponent(const ObjectArgument& argument)
        : Super(argument) {
    }
}
