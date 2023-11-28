#ifndef SCENECOMPONENT_H
#define SCENECOMPONENT_H

#include "Engine/Object/Component/EntityComponent.h"
#include "Engine/Math/Vector.h"
#include "Engine/Math/Rotator.h"

namespace Engine {
    CLASSTYPE(SceneComponent)
    class SceneComponent : public EntityComponent {
        GENERATE_BODY(SceneComponent, EntityComponent)

    private:
        PROPERTY(ValueType<Vector3>, position)
        Vector3 m_position;
        PROPERTY(ValueType<Vector3>, scale)
        Vector3 m_scale;
        PROPERTY(ValueType<Rotator>, rotation)
        Rotator m_rotation;

    public:
        SceneComponent(const ObjectArgument& argument);
        SceneComponent(const SceneComponent&) = default;
        virtual ~SceneComponent();

        void AttachToComponent(SceneComponent* parent);
        void DetachFromComponent();

        void SetLocalPosition(const Vector3& position);
        void SetLocalRotation(const Rotator& rotator);
        void SetLocalScale(const Vector3& scale);

        Vector3 GetLocalPosition() const;
        Rotator GetLocalRotation() const;
        Vector3 GetLocalScale() const;

        Vector3 GetWorldPosition() const;
        Rotator GetWorldRotation() const;
        Vector3 GetWorldScale() const;

        Vector3 GetForward() const;
        Vector3 GetRight() const;
        Vector3 GetUp() const;

        SceneComponent* GetChildComponent(Int32 childIndex) const;
        void GetChildrenComponents(Array<SceneComponent*>& components) const;

    private:
        SceneComponent* m_parent;
        Array<SceneComponent*> m_children;

        Int32 m_componentIndex;
    };
}

#endif // SCENECOMPONENT_H
