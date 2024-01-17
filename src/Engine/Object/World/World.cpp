#include "Engine/Object/World/World.h"

namespace Engine {
    GENERATE_INSTANTIATION(World)

    World::World(const ObjectArgument& argument)
        : Super(argument) {
    }

    World::~World() {
        DELETE_ARRAY_OF_OBJECTS(m_entities);
    }

    Entity* World::SpawnEntity(IClass* entityClass, const Vector3& location, const Rotator& rotation) {
        Entity* entity = entityClass->CreateDefaultObject(ObjectArgument::Dummy())->As<Entity>();
        entity->SetName(entityClass->IClass_GetName());
        entity->SetEntityLocation(location);
        entity->SetEntityRotation(rotation);

        m_entities.push_back(entity);

        return entity;
    }

    void World::DestroyEntity(Entity* entity) {
        ListIterator<Entity*> it = std::find_if(m_entities.begin(), m_entities.end(), [&](Entity* obj){ return entity == obj; });
        if (it != m_entities.end()) {
            m_entities.erase(it);
        }
    }

    void World::ForEachEntity(EventBase<Entity*>& callback) {
        for (Entity* entity : m_entities) {
            callback.Invoke(entity);
        }
    }

    void World::Start() {
        for (Entity* entity : m_entities) {
            entity->OnStart();
        }
    }

    void World::Update(Float deltaTime) {
        for (Entity* entity : m_entities) {
            entity->OnUpdate(deltaTime);
        }
    }
}
