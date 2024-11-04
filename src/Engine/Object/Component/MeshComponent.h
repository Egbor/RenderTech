#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "Engine/Object/Component/SceneComponent.h"
#include "Engine/Object/Class/Mesh.h"

namespace Engine {
    CLASSTYPE(MeshComponent)
    class MeshComponent : public SceneComponent {
        GENERATE_BODY(MeshComponent, SceneComponent)

    private:
        PROPERTY(ClassType<StaticMesh>, mesh)
        StaticMesh* m_mesh;

    public:
        MeshComponent(const ObjectArgument& argument);
        virtual ~MeshComponent() = default;

        void SetMesh(StaticMesh* mesh);
        StaticMesh* GetMesh() const;
    };
}

#endif // MESHCOMPONENT_H
