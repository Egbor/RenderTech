#include "Engine/Object/Class/Mesh.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    GENERATE_INSTANTIATION(StaticMesh)

    StaticMesh::StaticMesh(const ObjectArgument& argument) 
        : Super(argument), m_submeshes(), m_materials() {

    }

    StaticMesh::~StaticMesh() {
        for (Size i = 0; i < m_submeshes.size(); i++) {
            DELETE_OBJECT(m_submeshes[i].vertexBuffer);
            DELETE_OBJECT(m_submeshes[i].indexBuffer);
        }
    }

    void StaticMesh::SetMaterial(Material* material, Int32 index) {
        if (index >= m_materials.size()) {
            throw EngineException("[Mesh] A material index out of range");
        }
        m_materials[index] = material;
    }

    const MeshUnit& StaticMesh::GetSubmesh(Int32 index) const {
        if (index >= m_submeshes.size()) {
            throw EngineException("[Mesh] A submesh index out of range");
        }

        assert(m_submeshes[index].indexBuffer != nullptr);
        assert(m_submeshes[index].vertexBuffer != nullptr);

        return m_submeshes[index];
    }

    const Material* StaticMesh::GetMaterial(Int32 index) const {
        if (index >= m_materials.size()) {
            throw EngineException("[Mesh] A material index out of range");
        }
        return m_materials[index];
    }

    Int32 StaticMesh::GetNumberOfElements() const {
        assert(m_submeshes.size() == m_materials.size());
        return static_cast<Int32>(m_submeshes.size());
    }

    StaticMesh::Metadata::Metadata() 
        : m_name(), m_submeshes(), m_hasMeshOptimization(false), m_hasLeftHandedOptiization(false) {

    }

    StaticMesh::Metadata* StaticMesh::Metadata::AddSubmesh(MeshDescription* description) {
        assert(!m_name.empty());

        m_submeshes.push_back({
            description->BuildVertexBuffer(m_name + "_vbufRes"),
            description->BuildIndexBuffer(m_name + "_ibufRes")
        });
        return this;
    }

    StaticMesh::Metadata* StaticMesh::Metadata::SetName(const String& value) {
        return dynamic_cast<StaticMesh::Metadata*>(_SetName(value));
    }

    StaticMesh::Metadata* StaticMesh::Metadata::SetMeshOptimization(bool value) {
        m_hasMeshOptimization = value;
        return this;
    }

    StaticMesh::Metadata* StaticMesh::Metadata::SetLeftHandedOptimization(bool value) {
        m_hasLeftHandedOptiization = value;
        return this;
    }

    bool StaticMesh::Metadata::HasMeshOptimization() const {
        return m_hasMeshOptimization;
    }

    bool StaticMesh::Metadata::HasLeftHandedOptimization() const {
        return m_hasLeftHandedOptiization;
    }

    IResourceMetadata* StaticMesh::Metadata::_SetName(const String& value) {
        m_name = value;
        return this;
    }

    Object* StaticMesh::Metadata::Build() {
        StaticMesh* mesh = ClassType<StaticMesh>::CreateObject(ObjectArgument::Dummy());
        mesh->m_submeshes = m_submeshes;
        mesh->m_materials.resize(m_submeshes.size());
        return mesh;
    }
}
