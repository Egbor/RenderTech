#ifndef IENTITYCOMPONENTBEHAVIOR_H
#define IENTITYCOMPONENTBEHAVIOR_H

#include "Engine/EngineDef.h"

namespace Engine {
    class EntityComponent;

    class IEntityComponentBehavior {
    public:
        IEntityComponentBehavior() = default;
        virtual ~IEntityComponentBehavior() = default;

        virtual HandlerPtr RegisterComponent(EntityComponent* component) = 0;
        virtual void UnRegisterComponent(EntityComponent* component) = 0;

        virtual Int64 Id() const = 0;
    };
}

#endif // IENTITYCOMPONENTBEHAVIOR_H
