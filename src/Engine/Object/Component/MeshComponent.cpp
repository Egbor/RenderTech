#include "Engine/Object/Component/MeshComponent.h"
//#include "Engine/Rendering/Engine/RenderPass/BaseRenderPass.h"

namespace Engine {
    GENERATE_INSTANTIATION(MeshComponent)

    MeshComponent::MeshComponent(const ObjectArgument& argument)
        : Super(argument), m_mesh(nullptr) {
    }

    void MeshComponent::SetMesh(StaticMesh* mesh) {
        m_mesh = mesh;
    }

    StaticMesh* MeshComponent::GetMesh() const {
        return m_mesh;
    }
}
