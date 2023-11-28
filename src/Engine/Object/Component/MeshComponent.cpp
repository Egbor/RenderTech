#include "Engine/Object/Component/MeshComponent.h"
#include "Engine/Rendering/Engine/RenderPass/BaseRenderPass.h"

namespace Engine {
    GENERATE_INSTANTIATION(MeshComponent)

    MeshComponent::MeshComponent(const ObjectArgument& argument)
        : Super(argument), m_mesh(nullptr), m_meshResourceId(-1) {
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

    void MeshComponent::CreateRenderState(AbstractRenderPass* pass) {
        if (pass->Is(RenderPassType::RP_BASE)) {
            Matrix4x4 world = Matrix4x4::CreateMatrixWorld(GetWorldPosition(), GetWorldRotation(), GetWorldScale());

            BaseRenderPass* baseRenderPass = dynamic_cast<BaseRenderPass*>(pass);
            baseRenderPass->SetModel(world, m_mesh);
        }
    }
}
