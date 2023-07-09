#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "Engine/Object/Component/SceneComponent.h"
#include "Engine/Object/Class/Mesh.h"

namespace Engine {
    CLASSTYPE(MeshComponent)
    class MeshComponent : public SceneComponent {
        GENERATE_BODY(MeshComponent, SceneComponent)

    private:
        PROPERTY(ObjectClassType<Mesh>, mesh)
        Mesh* m_mesh;

    public:
        MeshComponent(const ObjectArgument& argument);
        MeshComponent(const MeshComponent&) = default;
        virtual ~MeshComponent() = default;

        void SetMesh(Mesh* mesh);
        Mesh* GetMesh() const;

        void Serialize(ISerializer* serializer) override;
        void Deserialize(ISerializer* serializer) override;

    protected:
        virtual UInt64 GetBehaviorID() const override;
    };
}

#endif // MESHCOMPONENT_H
