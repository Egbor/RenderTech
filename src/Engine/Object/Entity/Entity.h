#ifndef ENTITY_H
#define ENTITY_H

#include "Engine/Object/Object.h"
#include "Engine/Object/Component/SceneComponent.h"

#include "Engine/Core/System/Time/Time.h"

namespace Engine {
    CLASSTYPE(Entity)
    class Entity : public Object {
        GENERATE_BODY(Entity, Object)

    private:
        PROPERTY(ObjectClassType<SceneComponent>, rootComponent)
        SceneComponent* m_rootComponent;

    public:
        Entity(const ObjectArgument& argument);
        Entity(const Entity&) = default;
        virtual ~Entity();

        virtual void OnStart();
        virtual void OnUpdate(Float deltaTime);

        SceneComponent* GetRootComponent() const;

        void SetEntityLocation(const Vector3& location);
        void SetEntityRotation(const Rotator& rotation);
        void SetEntityScale(const Vector3& scale);

        void AddEntityLocation(const Vector3& offset);
        void AddEntityRotation(const Rotator& offset);
        void AddEntityScale(const Vector3& offset);

        Vector3 GetEntityForword() const;
        Vector3 GetEntityRight() const;
        Vector3 GetEntityUp() const;

        void Serialize(ISerializer* serializer) override;
        void Deserialize(ISerializer* serializer) override;
    };
}

#endif // ENTITY_H
