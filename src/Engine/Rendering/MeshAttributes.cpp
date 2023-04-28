#include "Engine/Rendering/MeshAttributes.h"

namespace Engine {
    MeshAttributes::MeshAttributes(MeshDescription& desc)
        : m_desc(desc) {
    }

    VertexAttributeRef<Vector3> MeshAttributes::GetVertexPositions() {
        return VertexAttributeRef<Vector3>(reinterpret_cast<MeshVertex*>(&m_desc.Vertices()[0].position));
    }

    VertexAttributeRef<Vector3> MeshAttributes::GetVertexTangents() {
        return VertexAttributeRef<Vector3>(reinterpret_cast<MeshVertex*>(&m_desc.Vertices()[0].tangent));
    }

    VertexAttributeRef<Vector3> MeshAttributes::GetVertexBitangents() {
        return VertexAttributeRef<Vector3>(reinterpret_cast<MeshVertex*>(&m_desc.Vertices()[0].bitangent));
    }

    VertexAttributeRef<Vector3> MeshAttributes::GetVertexNormals() {
        return VertexAttributeRef<Vector3>(reinterpret_cast<MeshVertex*>(&m_desc.Vertices()[0].normal));
    }

    VertexAttributeRef<Vector2> MeshAttributes::GetVertexUVs() {
        return VertexAttributeRef<Vector2>(reinterpret_cast<MeshVertex*>(&m_desc.Vertices()[0].uv));
    }
}
