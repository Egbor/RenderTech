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

    private:
        Array<MeshUnit> m_submeshes;
        Array<Material*> m_materials;

    public:
        class Metadata : public IResourceMetadata {
        public:
            Metadata();
            ~Metadata() = default;

            Metadata* AddSubmesh(MeshDescription* description);
            Metadata* SetName(const String& value);
            Metadata* SetMeshOptimization(bool value);
            Metadata* SetLeftHandedOptimization(bool value);

            bool HasMeshOptimization() const;
            bool HasLeftHandedOptimization() const;

            Object* Build() override;

        private:
            IResourceMetadata* _SetName(const String& value) override;

            String m_name;
            Array<MeshUnit> m_submeshes;
            bool m_hasMeshOptimization;
            bool m_hasLeftHandedOptiization;
        };

        StaticMesh(const ObjectArgument& argument);
        virtual ~StaticMesh();

        void SetMaterial(Material* material, Int32 index);

        const MeshUnit& GetSubmesh(Int32 index) const;
        const Material* GetMaterial(Int32 index) const;

        Int32 GetNumberOfElements() const;
    };
}

#endif // MESH_H
