#include "Engine/Object/Class/Mesh.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    GENERATE_INSTANTIATION(StaticMesh)

    StaticMesh::StaticMesh(const ObjectArgument& argument) 
        : Super(argument), m_submeshes(), m_materials() {

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

    GENERATE_INSTANTIATION(BasicMesh)

    BasicMesh::BasicMesh(const ObjectArgument& arguments)
        : Super(arguments), m_submeshes() {

    }

    BasicMesh::~BasicMesh() {
        for (Size i = 0; i < m_submeshes.size(); i++) {
            DELETE_OBJECT(m_submeshes[i].vertexBuffer);
            DELETE_OBJECT(m_submeshes[i].indexBuffer);
        }
    }

    void BasicMesh::AddSubmesh(MeshDescription* description) {
        m_submeshes.push_back({
            description->BuildVertexBuffer(),
            description->BuildIndexBuffer()
        });
    }

    StaticMesh* BasicMesh::BuildStaticMesh() const {
        StaticMesh* mesh = ClassType<StaticMesh>::CreateObject(ObjectArgument::Dummy());
        mesh->m_submeshes = m_submeshes;
        mesh->m_materials.resize(m_submeshes.size());

        return mesh;
    }

    //GENERATE_INSTANTIATION(MeshElement)

    //MeshElement::MeshElement(const ObjectArgument& arguments) 
    //    : Super(arguments), m_vertexBuffer(nullptr), m_indexBuffer(nullptr) {
    //}

    //MeshElement::~MeshElement() {
    //    DELETE_OBJECT(m_vertexBuffer);
    //    DELETE_OBJECT(m_indexBuffer);
    //}

    //Int32 MeshElement::GetNumIndexies() const {
    //    return m_indexBuffer->GetNumElements();
    //}

    //IBufferResourceData* MeshElement::GetVertexBuffer() const {
    //    return m_vertexBuffer;
    //}

    //IBufferResourceData* MeshElement::GetIndexBuffer() const {
    //    return m_indexBuffer;
    //}

    //void MeshElement::Create(const BufferInfo& vertexInfo, const BufferInfo& indexInfo) {
    //    IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
    //    m_vertexBuffer = factory->CreateBuffer(BufferType::BT_VERTEX, vertexInfo.bufferSize, vertexInfo.typeSize, vertexInfo.data);
    //    m_indexBuffer = factory->CreateBuffer(BufferType::BT_INDEX, indexInfo.bufferSize, indexInfo.typeSize, indexInfo.data);
    //}

    //GENERATE_INSTANTIATION(Mesh)

    //Mesh::Mesh(const ObjectArgument& argument)
    //    : Super(argument) {

    //}

    //Mesh::~Mesh() {
    //    DELETE_ARRAY_OF_OBJECTS(m_submeshes);
    //}

    //void Mesh::AddMeshElement(MeshDescription* meshDesc) {
    //    MeshElement* element = ClassType<MeshElement>::CreateObject(ObjectArgument::Dummy());
    //    element->Create(meshDesc->GetMeshInfo(), meshDesc->GetFaceInfo());

    //    m_submeshes.push_back(element);
    //    m_materials.push_back(nullptr);
    //}

    //void Mesh::SetMaterial(Int32 slot, Material* material) {
    //    if (m_materials.size() < slot) {
    //        throw EngineException("[Mesh] Mesh::SetMaterial() failed (Out of range)");
    //    }
    //    m_materials[slot] = material;
    //}

    //MeshElement* Mesh::GetMeshElement(Int32 slot) const {
    //    if (m_submeshes.size() < slot) {
    //        throw EngineException("[Mesh] Mesh::GetMeshDescription() failed (Out of range)");
    //    }
    //    return m_submeshes[slot];
    //}

    //Material* Mesh::GetMaterial(Int32 slot) const {
    //    if (m_materials.size() < slot) {
    //        throw EngineException("[Mesh] Mesh::GetMaterial() failed (Out of range)");
    //    }
    //    return m_materials[slot];
    //}

    //Int32 Mesh::GetNumMeshElements() const {
    //    return static_cast<Int32>(m_submeshes.size());
    //}

    //Int32 Mesh::GetNumMaterials() const {
    //    return GetNumMeshElements();
    //}
}
