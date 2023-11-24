#ifndef MESH_H
#define MESH_H

#include "Engine/Object/Object.h"
#include "Engine/Object/Class/Material.h"
#include "Engine/Rendering/MeshDescription.h"

#include "Engine/Core/Render/Api/Interface/IBufferResource.h"

namespace Engine {
    CLASSTYPE(MeshElement)
    class MeshElement : public Object {
        GENERATE_BODY(MeshElement, Object)

    private:
        IBufferResourceData* m_vertexBuffer;
        IBufferResourceData* m_indexBuffer;

    public:
        MeshElement(const ObjectArgument& arguments);
        virtual ~MeshElement();

        Int32 GetNumIndexies() const;

        IBufferResourceData* GetVertexBuffer() const;
        IBufferResourceData* GetIndexBuffer() const;

        void Create(const BufferInfo& vertexInfo, const BufferInfo& indexInfo);
    };

    CLASSTYPE(Mesh)
    class Mesh : public Object {
        GENERATE_BODY(Mesh, Object)

    private:
        Array<MeshElement*> m_submeshes;
        Array<Material*> m_materials;

    public:
        Mesh(const ObjectArgument& argument);
        virtual ~Mesh();

        void AddMeshElement(MeshDescription* meshDesc);
        void SetMaterial(Int32 slot, Material* material);

        MeshElement* GetMeshElement(Int32 slot) const;
        Material* GetMaterial(Int32 slot) const;

        Int32 GetNumMeshElements() const;
        Int32 GetNumMaterials() const;
    };
}

#endif // MESH_H
