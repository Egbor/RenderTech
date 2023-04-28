#include "Engine/Rendering/MeshDescription.h"

namespace Engine {
    MeshVertex::MeshVertex()
        : position(), tangent(), bitangent(), normal(), uv() {

    }

    MeshFace::MeshFace(const Array<VertexID>& vertexIDs) {
        memcpy(vertices, vertexIDs.data(), sizeof(vertices));
    }

    MeshDescription::MeshDescription()
        : m_vertices(), m_faces() {

    }

    VertexID MeshDescription::CreateVertex() {
        m_vertices.push_back({});
        return static_cast<VertexID>(m_vertices.size() - 1);
    }

    FaceID MeshDescription::CreateFace(Array<VertexID> vertexIDs) {
        m_faces.push_back(vertexIDs);
        return static_cast<FaceID>(m_faces.size() - 1);
    }

    BufferInfo MeshDescription::GetMeshInfo() {
        return { m_vertices.data(), static_cast<UInt32>(m_vertices.size()), sizeof(MeshVertex) };
    }

    BufferInfo MeshDescription::GetFaceInfo() {
        return { m_faces.data(), static_cast<UInt32>(m_faces.size()), sizeof(MeshFace) };
    }

    Array<MeshVertex>& MeshDescription::Vertices() {
        return m_vertices;
    }

    Array<MeshFace>& MeshDescription::Faces() {
        return m_faces;
    }
}
