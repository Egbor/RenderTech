#include "Engine/Object/Component/Behavior/MeshComponentBehavior.h"

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(MeshComponentBehavior)

    MeshComponentBehavior::MeshComponentBehavior(const ObjectArgument& argument)
        : Super(argument), m_componentsInterators() {
    }

    Mesh* MeshComponentBehavior::GetCurrentMesh() const {
        return (*m_componentsInterators)->GetMesh();
    }

    Matrix4x4 MeshComponentBehavior::GetCurrentMeshWorldMatrix() const {
        return Matrix4x4::CreateMatrixWorld(
            (*m_componentsInterators)->GetWorldPosition(),
            (*m_componentsInterators)->GetWorldRotation(),
            (*m_componentsInterators)->GetWorldScale()
        );
    }

    void MeshComponentBehavior::Init() {
        m_componentsInterators = GetComponents().begin();
    }

    void MeshComponentBehavior::ToNext() {
        m_componentsInterators++;
    }

    bool MeshComponentBehavior::HasMore() {
        if (m_componentsInterators == GetComponents().end()) {
            return false;
        }
        return true;
    }
}
