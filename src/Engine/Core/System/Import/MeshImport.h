#ifndef MESHIMPORT_H
#define MESHIMPORT_H

#include "Engine/Core/System/Import/Import.h"
#include "Engine/Object/Class/Mesh.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

namespace Engine {
    class MeshImport : public Import<Mesh> {
    public:
        MeshImport(const String& filename);
        MeshImport(const MeshImport&) = default;
        virtual ~MeshImport() = default;

        Mesh* LoadResource() override;

    private:
        void ProcessNode(aiNode* node, const aiScene* scene, Mesh* outMesh);
        void ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshDescription* outMeshDesc);
    };
}

#endif // MESHIMPORT_H
