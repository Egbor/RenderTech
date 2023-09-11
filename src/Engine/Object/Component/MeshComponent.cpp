#include "Engine/Object/Component/MeshComponent.h"

#include "Engine/Core/System/Import/MeshImport.h"

namespace Engine {
    GENERATE_INSTANTIATION(MeshComponent)

    MeshComponent::MeshComponent(const ObjectArgument& argument)
        : Super(argument), m_mesh(nullptr) {
    }

    void MeshComponent::SetMesh(Mesh* mesh) {
        m_mesh = mesh;
    }

    Mesh* MeshComponent::GetMesh() const {
        return m_mesh;
    }

    UInt64 MeshComponent::GetBehaviorID() const {
        return MeshComponent::TypeIdClass();
    }
}
