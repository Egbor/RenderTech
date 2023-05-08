#ifndef SCENECOMPONENT_H
#define SCENECOMPONENT_H

#include "Engine/Object/Component/EntityComponent.h"
#include "Engine/Math/Vector.h"
#include "Engine/Math/Rotator.h"

namespace Engine {
    class SceneComponent : public EntityComponent {
        GENERATE_BODY(SceneComponent, EntityComponent)

    public:
        Vector3 position;
        Vector3 scale;
        Rotator rotation;

    private:
        SceneComponent* m_parent;
        List<SceneComponent*> m_children;
        ListIterator<SceneComponent*> m_childIterator;

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

        const List<SceneComponent*>& GetChildComponents() const;

    protected:
        virtual UInt64 GetBehaviorID() const override;
    };
}

#endif // SCENECOMPONENT_H
