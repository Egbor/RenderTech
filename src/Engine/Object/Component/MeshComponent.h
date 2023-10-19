#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "Engine/Object/Component/SceneComponent.h"
#include "Engine/Object/Class/Mesh.h"

namespace Engine {
    CLASSTYPE(MeshComponent)
    class MeshComponent : public SceneComponent {
        GENERATE_BODY(MeshComponent, SceneComponent)

    private:
        PROPERTY(ClassType<Mesh>, mesh)
        Mesh* m_mesh;

    public:
        MeshComponent(const ObjectArgument& argument);
        MeshComponent(const MeshComponent&) = default;
        virtual ~MeshComponent() = default;

        void SetMesh(Mesh* mesh);
        Mesh* GetMesh() const;

    protected:
        virtual UInt64 GetBehaviorID() const override;

        void CreateRenderState(IRenderPass* pass) override;
    };
}

#endif // MESHCOMPONENT_H
