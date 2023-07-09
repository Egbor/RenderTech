#ifndef MESH_H
#define MESH_H

#include "Engine/Object/Object.h"
#include "Engine/Object/Class/Buffer.h"
#include "Engine/Object/Class/Material.h"
#include "Engine/Rendering/MeshDescription.h"

namespace Engine {
    constexpr static const char* argVertexBufferInfo = "vertexBufferInfo";
    constexpr static const char* argIndexBufferInfo = "indexBufferInfo";

    CLASSTYPE(Submesh)
    class Submesh : public Object {
        GENERATE_BODY(Submesh, Object)

    private:
        Buffer* m_vertexBuffer;
        Buffer* m_indexBuffer;
        Material* m_material;

    public:
        Submesh(const ObjectArgument& argument);
        Submesh(const Submesh&) = default;
        virtual ~Submesh();

        UInt32 GetNumIndexies() const;

        Buffer* GetVertexBuffer() const;
        Buffer* GetIndexBuffer() const;
        Material* GetMaterial() const;

        void SetMaterial(Material* material);
    };

    CLASSTYPE(Mesh)
    class Mesh : public Object {
        GENERATE_BODY(Mesh, Object)

    private:
        Array<Submesh*> m_submeshes;

    public:
        Mesh(const ObjectArgument& argument);
        Mesh(const Mesh&) = default;
        virtual ~Mesh();

        void AddSubmesh(MeshDescription* meshDesc);
        void SetMaterial(Int32 slot, Material* material);

        Submesh* GetSubmesh(Int32 slot) const;
        Material* GetMaterial(Int32 slot) const;

        Int32 GetNumSubmeshes() const;
        Int32 GetNumMaterials() const;
    };
}

#endif // MESH_H
