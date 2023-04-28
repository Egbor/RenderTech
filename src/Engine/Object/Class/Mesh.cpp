#include "Engine/Object/Class/Mesh.h"
#include "Engine/Object/Global/EngineConfig.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    GENERATE_RTTI_DEFINITIONS(Submesh)

    Submesh::Submesh(const ObjectArgument& argument)
        : Super(argument), m_material(nullptr) {
        Context* context = EngineConfig::GetInstance().GetContext();
        BufferInfo vertexInfo;
        BufferInfo indexInfo;

        argument.Pull(argVertexBufferInfo, vertexInfo);
        argument.Pull(argIndexBufferInfo, indexInfo);

        m_vertexBuffer = context->CreateBuffer(tagREALTIME_BUFFER_VERTEX, vertexInfo);
        m_indexBuffer = context->CreateBuffer(tagREALTIME_BUFFER_INDEX, indexInfo);
    }

    Submesh::~Submesh() {
        DELETE_OBJECT(m_vertexBuffer);
        DELETE_OBJECT(m_indexBuffer);
    }

    UInt32 Submesh::GetNumIndexies() const {
        return m_indexBuffer->GetBufferNumElements();
    }

    Buffer* Submesh::GetVertexBuffer() const {
        return m_vertexBuffer;
    }

    Buffer* Submesh::GetIndexBuffer() const {
        return m_indexBuffer;
    }

    Material* Submesh::GetMaterial() const {
        return m_material;
    }

    void Submesh::SetMaterial(Material* material) {
        m_material = material;
    }

    GENERATE_RTTI_DEFINITIONS(Mesh)

    Mesh::Mesh(const ObjectArgument& argument)
        : Super(argument) {

    }

    Mesh::~Mesh() {
        DELETE_ARRAY_OF_OBJECTS(m_submeshes);
    }

    void Mesh::AddSubmesh(MeshDescription* meshDesc) {
        ObjectArgument argument;
        argument.Put(argVertexBufferInfo, meshDesc->GetMeshInfo());
        argument.Put(argIndexBufferInfo, meshDesc->GetFaceInfo());
        m_submeshes.push_back(Submesh::TypeMetaClass()->CreateDefaultObject<Submesh>(argument));
    }

    void Mesh::SetMaterial(Int32 slot, Material* material) {
        if (m_submeshes.size() < slot) {
            throw new EngineException("[Mesh] Mesh::SetMaterial() failed (Out of range)");
        }
        m_submeshes[slot]->SetMaterial(material);
    }

    Submesh* Mesh::GetSubmesh(Int32 slot) const {
        if (m_submeshes.size() < slot) {
            throw new EngineException("[Mesh] Mesh::GetMeshDescription() failed (Out of range)");
        }
        return m_submeshes[slot];
    }

    Material* Mesh::GetMaterial(Int32 slot) const {
        if (m_submeshes.size() < slot) {
            throw new EngineException("[Mesh] Mesh::GetMaterial() failed (Out of range)");
        }
        return m_submeshes[slot]->GetMaterial();
    }

    Int32 Mesh::GetNumSubmeshes() const {
        return static_cast<Int32>(m_submeshes.size());
    }

    Int32 Mesh::GetNumMaterials() const {
        return GetNumSubmeshes();
    }
}
