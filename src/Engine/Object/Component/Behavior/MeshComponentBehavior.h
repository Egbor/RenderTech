#ifndef MESHCOMPONENTBEHAVIOR_H
#define MESHCOMPONENTBEHAVIOR_H

#include "Engine/Object/Component/Behavior/EntityComponentBehavior.h"
#include "Engine/Object/Component/MeshComponent.h"

#include "Engine/Math/Matrix.h"

namespace Engine {
    class MeshComponentBehavior : public EntityComponentBehavior<MeshComponent> {
        GENERATE_BODY(MeshComponentBehavior, EntityComponentBehavior<MeshComponent>)

    private:
        ConstListIterator<MeshComponent*> m_componentsInterators;

    public:
        MeshComponentBehavior(const ObjectArgument& argument);
        MeshComponentBehavior(const MeshComponentBehavior&) = default;
        virtual ~MeshComponentBehavior() = default;

        Mesh* GetCurrentMesh() const;
        Matrix4x4 GetCurrentMeshWorldMatrix() const;

        void Init();
        void ToNext();
        bool HasMore();
    };
}

#endif // MESHCOMPONENTBEHAVIOR_H
