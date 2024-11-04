#ifndef MESH_H
#define MESH_H

#include "Engine/Object/Class/Material.h"
#include "Engine/Rendering/MeshDescription.h"
#include "Engine/Core/Render/Base/Resource/BufferResource.h"

namespace Engine {
    struct MeshUnit {
        BufferResource* vertexBuffer;
        BufferResource* indexBuffer;
    };

    CLASSTYPE(StaticMesh)
    class StaticMesh : public Object {
        GENERATE_BODY(StaticMesh, Object)

        friend class BasicMesh;

    private:
        Array<MeshUnit> m_submeshes;
        Array<Material*> m_materials;

    public:
        StaticMesh(const ObjectArgument& argument);
        virtual ~StaticMesh() = default;

        void SetMaterial(Material* material, Int32 index);

        const MeshUnit& GetSubmesh(Int32 index) const;
        const Material* GetMaterial(Int32 index) const;

        Int32 GetNumberOfElements() const;
    };

    CLASSTYPE(BasicMesh)
    class BasicMesh : public Object {
        GENERATE_BODY(BasicMesh, Object)

    private:
        Array<MeshUnit> m_submeshes;

    public:
        BasicMesh(const ObjectArgument& argument);
        virtual ~BasicMesh();

        void AddSubmesh(MeshDescription* description);
        StaticMesh* BuildStaticMesh() const;
    };



    //CLASSTYPE(MeshElement)
    //class MeshElement : public Object {
    //    GENERATE_BODY(MeshElement, Object)

    //    friend class Mesh;

    //private:
    //    IBufferResourceData* m_vertexBuffer;
    //    IBufferResourceData* m_indexBuffer;

    //public:
    //    MeshElement(const ObjectArgument& arguments);
    //    virtual ~MeshElement();

    //    Int32 GetNumIndexies() const;

    //    IBufferResourceData* GetVertexBuffer() const;
    //    IBufferResourceData* GetIndexBuffer() const;

    //protected:
    //    void Create(const BufferInfo& vertexInfo, const BufferInfo& indexInfo);
    //};

    //CLASSTYPE(Mesh)
    //class Mesh : public Object {
    //    GENERATE_BODY(Mesh, Object)

    //private:
    //    Array<MeshElement*> m_submeshes;
    //    Array<Material*> m_materials;

    //public:
    //    Mesh(const ObjectArgument& argument);
    //    virtual ~Mesh();

    //    void AddMeshElement(MeshDescription* meshDesc);
    //    void SetMaterial(Int32 slot, Material* material);

    //    MeshElement* GetMeshElement(Int32 slot) const;
    //    Material* GetMaterial(Int32 slot) const;

    //    Int32 GetNumMeshElements() const;
    //    Int32 GetNumMaterials() const;
    //};
}

#endif // MESH_H
