#include "Engine/Object/Component/SceneComponent.h"

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(SceneComponent)

    SceneComponent::SceneComponent(const ObjectArgument& argument)
        : Super(argument), position(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f)
        , rotation(0.0f, 0.0f, 0.0f), m_parent(nullptr), m_children(), m_childIterator() {
        argument.Pull(argTagLocation, position);
        argument.Pull(argTagRotation, rotation);
    }

    SceneComponent::~SceneComponent() {
        DetachFromComponent();
    }

    void SceneComponent::AttachToComponent(SceneComponent* parent) {
        if (m_parent == nullptr) {
            m_parent = parent;
            m_parent->m_children.push_back(this);
            m_childIterator = --m_parent->m_children.end();

            Register();
        }
    }

    void SceneComponent::DetachFromComponent() {
        if (m_parent != nullptr) {
            m_parent->m_children.erase(m_childIterator);
            m_parent = nullptr;

            UnRegister();
        }

        for (SceneComponent* child : m_children) {
            child->DetachFromComponent();
        }
    }

    void SceneComponent::SetLocalPosition(const Vector3& position) {
        this->position = position;
    }

    void SceneComponent::SetLocalRotation(const Rotator& rotator) {
        this->rotation = rotator;
    }

    void SceneComponent::SetLocalScale(const Vector3& scale) {
        this->scale = scale;
    }

    Vector3 SceneComponent::GetLocalPosition() const {
        return this->position;
    }

    Rotator SceneComponent::GetLocalRotation() const {
        return this->rotation;
    }

    Vector3 SceneComponent::GetLocalScale() const {
        return this->scale;
    }

    Vector3 SceneComponent::GetWorldPosition() const {
        if (m_parent == nullptr) {
            return this->position;
        }
        return m_parent->GetWorldPosition() + GetWorldRotation().RotateVector(this->position);
    }

    Rotator SceneComponent::GetWorldRotation() const {
        if (m_parent == nullptr) {
            return this->rotation;
        }
        return m_parent->GetWorldRotation() + this->rotation;
    }

    Vector3 SceneComponent::GetWorldScale() const {
        if (m_parent == nullptr) {
            return this->scale;
        }
        return m_parent->GetWorldScale() * this->scale;
    }

    Vector3 SceneComponent::GetForward() const {
        return GetWorldRotation().RotateVector(Vector3::forword);
    }

    Vector3 SceneComponent::GetRight() const {
        return GetWorldRotation().RotateVector(Vector3::right);
    }

    Vector3 SceneComponent::GetUp() const {
        return GetWorldRotation().RotateVector(Vector3::up);
    }

    UInt64 SceneComponent::GetBehaviorID() const {
        return SceneComponent::TypeIdClass();
    }
}
