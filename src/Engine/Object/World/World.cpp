#include "Engine/Object/World/World.h"

namespace Engine {
    GENERATE_INSTANTIATION(World)

    World::World(const ObjectArgument& argument)
        : Super(argument) {
        m_scene = new Scene();
    }

    World::~World() {
        DELETE_OBJECT(m_scene);
    }

    Entity* World::SpawnEntity(IClass* entityClass, const Vector3& location, const Rotator& rotation) {
        Entity* entity = entityClass->CreateDefaultObject(ObjectArgument::Dummy())->As<Entity>();
        entity->SetName(entityClass->IClass_GetName());
        entity->SetEntityLocation(location);
        entity->SetEntityRotation(rotation);

        m_scene->InsertEntity(entity);

        return entity;
    }

    void World::DestroyEntity(Entity* entity) {
        m_scene->RemoveEntity(entity);
    }

    void World::Start() {
        m_scene->DoTraversal([&](Entity* entity) { entity->OnStart(); });
    }

    void World::Update(Float deltaTime) {
        m_scene->DoTraversal([&](Entity* entity) { entity->OnUpdate(deltaTime); });
    }

    Scene* World::GetScene() const {
        return m_scene;
    }
}
