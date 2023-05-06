#ifndef WORLD_H
#define WORLD_H

#include "Engine/Object/Object.h"
#include "Engine/Object/World/WorldMode.h"

#include "Engine/Object/Entity/Entity.h"
#include "Engine/Object/EntityController/EntityController.h"

namespace Engine {
    class World : public Object {
        GENERATE_BODY(World, Object)

    private:
        List<EntityController*> m_controllers;
        List<Entity*> m_entities;

        Event<Entity*> m_eventOnEntitySpawn;
        Event<Entity*> m_eventOnEntityDestroy;

    public:
        World(const ObjectArgument& argument);
        World(const World&) = default;
        virtual ~World();

        void Start(WorldMode* mode);
        void Update(Float deltaTime);

        Entity* SpawnEntity(MetaClass* entityClass, const Vector3& location, const Rotator& rotation);
        void DestroyEntity(Entity* entity);

        EntityController* AddController(MetaClass* controllerClass);
        void RemoveController(EntityController* controller);

        void AddOnEntitySpawnEvent(EventBase<Entity*>& callback);
        void AddOnEntityDestroyEvent(EventBase<Entity*>& callback);

        void RemoveOnEntitySpawnEvent(EventBase<Entity*>& callback);
        void RemoveOnEntityDestroyEvent(EventBase<Entity*>& callback);

    private:
        void SetupWorld(WorldMode* mode);
        void NotifyEntityControllers();
        void NotifyEntitiesAboutStart();
        void NotifyEntitiesAboutUpdate(Float deltaTime);
    };
}

#endif // WORLD_H
