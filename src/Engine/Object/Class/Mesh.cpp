#include "Engine/Object/Class/Mesh.h"
#include "Engine/Object/Global/EngineConfig.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    GENERATE_INSTANTIATION(MeshElement)

    MeshElement::MeshElement(const ObjectArgument& arguments) 
        : Super(arguments), m_vertexBuffer(nullptr), m_indexBuffer(nullptr) {
        //BufferInfo vertexInfo;
        //BufferInfo indexInfo;

        //argument.Pull(argVertexBufferInfo, vertexInfo);
        //argument.Pull(argIndexBufferInfo, indexInfo);

         //context->CreateBuffer(tagREALTIME_BUFFER_VERTEX, vertexInfo);
         //context->CreateBuffer(tagREALTIME_BUFFER_INDEX, indexInfo);
    }

    MeshElement::~MeshElement() {
        DELETE_OBJECT(m_vertexBuffer);
        DELETE_OBJECT(m_indexBuffer);
    }

    Int32 MeshElement::GetNumIndexies() const {
        return m_indexBuffer->GetNumElements();
    }

    IBufferResourceData* MeshElement::GetVertexBuffer() const {
        return m_vertexBuffer;
    }

    IBufferResourceData* MeshElement::GetIndexBuffer() const {
        return m_indexBuffer;
    }

    void MeshElement::Create(const BufferInfo& vertexInfo, const BufferInfo& indexInfo) {
        IContext* context = EngineConfig::GetInstance().GetContext();
        IRenderResourceFactory* factory = context->QueryResourceFactory();

        m_vertexBuffer = factory->CreateBuffer(BufferType::BT_VERTEX, vertexInfo.bufferSize, vertexInfo.typeSize, vertexInfo.data);
        m_indexBuffer = factory->CreateBuffer(BufferType::BT_INDEX, indexInfo.bufferSize, indexInfo.typeSize, indexInfo.data);
    }

    //Material* Submesh::GetMaterial() const {
    //    return m_material;
    //}

    //void Submesh::SetMaterial(Material* material) {
    //    m_material = material;
    //}

    GENERATE_INSTANTIATION(Mesh)

    Mesh::Mesh(const ObjectArgument& argument)
        : Super(argument) {

    }

    Mesh::~Mesh() {
        DELETE_ARRAY_OF_OBJECTS(m_submeshes);
    }

    void Mesh::AddMeshElement(MeshDescription* meshDesc) {
        MeshElement* element = ClassType<MeshElement>::CreateObject(ObjectArgument::Dummy());
        element->Create(meshDesc->GetMeshInfo(), meshDesc->GetFaceInfo());
        m_submeshes.push_back(element);
        m_materials.push_back(nullptr);
    }

    void Mesh::SetMaterial(Int32 slot, Material* material) {
        if (m_materials.size() < slot) {
            throw EngineException("[Mesh] Mesh::SetMaterial() failed (Out of range)");
        }
        m_materials[slot] = material;
    }

    MeshElement* Mesh::GetMeshElement(Int32 slot) const {
        if (m_submeshes.size() < slot) {
            throw EngineException("[Mesh] Mesh::GetMeshDescription() failed (Out of range)");
        }
        return m_submeshes[slot];
    }

    Material* Mesh::GetMaterial(Int32 slot) const {
        if (m_materials.size() < slot) {
            throw EngineException("[Mesh] Mesh::GetMaterial() failed (Out of range)");
        }
        return m_materials[slot];
    }

    Int32 Mesh::GetNumMeshElements() const {
        return static_cast<Int32>(m_submeshes.size());
    }

    Int32 Mesh::GetNumMaterials() const {
        return GetNumMeshElements();
    }
}
