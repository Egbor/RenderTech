#include "Engine/Object/Entity/Entity.h"

namespace Engine {
    GENERATE_INSTANTIATION(Entity)

    Entity::Entity(const ObjectArgument& argument)
        : Super(argument), m_rootComponent(new SceneComponent(argument)) {
    }

    Entity::~Entity() {
        DELETE_OBJECT(m_rootComponent);
    }

    void Entity::OnStart() {

    }

    void Entity::OnUpdate(Float deltaTime) {

    }

    SceneComponent* Entity::GetRootComponent() const {
        return m_rootComponent;
    }

    void Entity::SetEntityLocation(const Vector3& location) {
        m_rootComponent->SetLocalPosition(location);
    }

    void Entity::SetEntityRotation(const Rotator& rotation) {
        m_rootComponent->SetLocalRotation(rotation);
    }

    void Entity::SetEntityScale(const Vector3& scale) {
        m_rootComponent->SetLocalScale(scale);
    }

    void Entity::AddEntityLocation(const Vector3& offset) {
        Vector3 oldPosition = m_rootComponent->GetLocalPosition();
        m_rootComponent->SetLocalPosition(oldPosition + offset);
    }

    void Entity::AddEntityRotation(const Rotator& offset) {
        Rotator oldRotation = m_rootComponent->GetLocalRotation();
        m_rootComponent->SetLocalRotation(oldRotation + offset);
    }

    void Entity::AddEntityScale(const Vector3& offset) {
        Vector3 oldScale = m_rootComponent->GetLocalScale();
        m_rootComponent->SetLocalScale(oldScale + offset);
    }

    Vector3 Entity::GetEntityForword() const {
        return m_rootComponent->GetForward();
    }

    Vector3 Entity::GetEntityRight() const {
        return m_rootComponent->GetRight();
    }

    Vector3 Entity::GetEntityUp() const {
        return m_rootComponent->GetUp();
    }
}
