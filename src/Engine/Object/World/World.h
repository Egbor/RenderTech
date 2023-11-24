#ifndef WORLD_H
#define WORLD_H

#include "Engine/Object/Object.h"
#include "Engine/Object/Entity/Entity.h"

#include "Engine/Core/Utils/Event.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    CLASSTYPE(World)
    class World : public Object {
        GENERATE_BODY(World, Object)

    private:
        List<Entity*> m_entities;

    public:
        World(const ObjectArgument& argument);
        virtual ~World();

        Entity* SpawnEntity(IClass* entityClass, const Vector3& location, const Rotator& rotation);
        void DestroyEntity(Entity* entity);

        void ForEachEntity(EventBase<Entity*>& callback);
    };
}

#endif // WORLD_H
