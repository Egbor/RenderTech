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

    void MeshComponent::CreateRenderState(IRenderPass* pass) {
        if (pass->Is(RenderPassType::RP_BASE)) {
            BaseRenderPass* baseRenderPass = dynamic_cast<BaseRenderPass*>(pass);
            Int32 meshResourceId = baseRenderPass->InitializeNewMeshResource();

            if (meshResourceId > -1) {
                m_meshResourceId = meshResourceId;
            }

            BRPMaterialResource* materialResource;
            BRPPositionResource* positionResource;

            for (Int32 i = 0; i < m_mesh->GetNumMeshElements(); i++) {
                if (baseRenderPass->GetAccessToMaterialResource(&materialResource, m_meshResourceId, i)) {
                    MeshElement* element = m_mesh->GetMeshElement(i);
                    Material* material = m_mesh->GetMaterial(i);

                    materialResource->vertex = element->GetVertexBuffer();
                    materialResource->index = element->GetIndexBuffer();
                    materialResource->textures = material->GetNativeTextureResources();
                }
            }

            if (baseRenderPass->GetAccessToPositionResource(&positionResource, m_meshResourceId)) {
                positionResource->world = Matrix4x4::CreateMatrixWorld(GetWorldPosition(), GetWorldRotation(), GetWorldScale());
            }
        }
    }
}
