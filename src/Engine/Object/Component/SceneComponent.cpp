#include "Engine/Object/Component/SceneComponent.h"

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(SceneComponent)

    SceneComponent::SceneComponent(const ObjectArgument& argument)
        : Super(argument), m_position(0.0f, 0.0f, 0.0f), m_scale(1.0f, 1.0f, 1.0f)
        , m_rotation(0.0f, 0.0f, 0.0f), m_parent(nullptr), m_children(), m_childIterator() {
        argument.Pull(argLocation, m_position);
        argument.Pull(argRotation, m_rotation);
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
        m_position = position;
    }

    void SceneComponent::SetLocalRotation(const Rotator& rotator) {
        m_rotation = rotator;
    }

    void SceneComponent::SetLocalScale(const Vector3& scale) {
        m_scale = scale;
    }

    Vector3 SceneComponent::GetLocalPosition() const {
        return m_position;
    }

    Rotator SceneComponent::GetLocalRotation() const {
        return m_rotation;
    }

    Vector3 SceneComponent::GetLocalScale() const {
        return m_scale;
    }

    Vector3 SceneComponent::GetWorldPosition() const {
        if (m_parent == nullptr) {
            return m_position;
        }
        return m_parent->GetWorldPosition() + GetWorldRotation().RotateVector(m_position);
    }

    Rotator SceneComponent::GetWorldRotation() const {
        if (m_parent == nullptr) {
            return m_rotation;
        }
        return m_parent->GetWorldRotation() + m_rotation;
    }

    Vector3 SceneComponent::GetWorldScale() const {
        if (m_parent == nullptr) {
            return m_scale;
        }
        return m_parent->GetWorldScale() * m_scale;
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
