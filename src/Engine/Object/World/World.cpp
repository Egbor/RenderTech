#include "Engine/Object/World/World.h"

namespace Engine {
    //GENERATE_RTTI_DEFINITIONS(World)
    GENERATE_INSTANTIATION(World)

    World::World(const ObjectArgument& argument)
        : Super(argument) {
    }

    World::~World() {
        DELETE_ARRAY_OF_OBJECTS(m_controllers)
        DELETE_ARRAY_OF_OBJECTS(m_entities);
    }

    void World::Start(WorldMode* mode) {
        SetupWorld(mode);
        NotifyEntitiesAboutStart();
    }

    void World::Update(Float deltaTime) {
        NotifyEntityControllers();
        NotifyEntitiesAboutUpdate(deltaTime);
    }

    Entity* World::SpawnEntity(ObjectType* entityClass, const Vector3& location, const Rotator& rotation) {
        Entity* entity = reinterpret_cast<Entity*>(entityClass->CreateInstance());
        entity->SetName(entityClass->GetName());
        entity->SetEntityLocation(location);
        entity->SetEntityRotation(rotation);

        m_entities.push_back(entity);
        m_eventOnEntitySpawn(entity);

        return entity;
    }

    void World::DestroyEntity(Entity* entity) {
        ListIterator<Entity*> it = std::find_if(m_entities.begin(), m_entities.end(), [&](Entity* obj){ return entity == obj; });
        if (it != m_entities.end()) {
            m_entities.erase(it);
            m_eventOnEntityDestroy(*it);
        }
    }

    EntityController* World::AddController(ObjectType* controllerClass) {
        EntityController* controller = reinterpret_cast<EntityController*>(controllerClass->CreateInstance());
        m_controllers.push_back(controller);
        return controller;
    }

    void World::RemoveController(EntityController* controller) {
        ListIterator<EntityController*> it = std::find_if(m_controllers.begin(), m_controllers.end(), [&](EntityController* obj){
                return controller == obj;
        });
        if (it != m_controllers.end()) {
            m_controllers.erase(it);
        }
    }

    void World::AddOnEntitySpawnEvent(EventBase<Entity*>& callback) {
        m_eventOnEntitySpawn += callback;
    }

    void World::AddOnEntityDestroyEvent(EventBase<Entity*>& callback) {
        m_eventOnEntityDestroy += callback;
    }

    void World::RemoveOnEntitySpawnEvent(EventBase<Entity*>& callback) {
        m_eventOnEntitySpawn -= callback;
    }

    void World::RemoveOnEntityDestroyEvent(EventBase<Entity*>& callback) {
        m_eventOnEntityDestroy -= callback;
    }

    void World::SetupWorld(WorldMode* mode) {
        Actor* actor = SpawnEntity(mode->GetActorClass(), Vector3(0.0f, 0.0f, 0.0f), Rotator(0.0f, 0.0f, 0.0f))->As<Actor>();
        EntityController* controller = AddController(mode->GetControllerClass());
        controller->AttachActor(actor);
    }

    void World::NotifyEntityControllers() {
        for (EntityController* controller : m_controllers) {
            controller->Execute();
        }
    }

    void World::NotifyEntitiesAboutStart() {
        for (Entity* entity : m_entities) {
            entity->OnStart();
        }
    }

    void World::NotifyEntitiesAboutUpdate(Float deltaTime) {
        for (Entity* entity : m_entities) {
            entity->OnUpdate(deltaTime);
        }
    }
}
