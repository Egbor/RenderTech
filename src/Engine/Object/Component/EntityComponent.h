#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

#include "Engine/Object/Object.h"

namespace Engine {
    CLASSTYPE(EntityComponent)
    class EntityComponent : public Object {
        GENERATE_BODY(EntityComponent, Object)

    public:
        EntityComponent(const ObjectArgument& argument);
        virtual ~EntityComponent() = default;
    };
}

#endif // ENTITYCOMPONENT_H
