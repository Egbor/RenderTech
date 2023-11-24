#include "Engine/Core/System/Import/MeshImport.h"
#include "Engine/Rendering/MeshAttributes.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

namespace Engine {
    void ImportAssimpVector3DData(VertexAttributeRef<Vector3>& ref, VertexID id, const aiVector3D& vector) {
        ref[id].x = vector.x;
        ref[id].y = vector.y;
        ref[id].z = vector.z;
    }

    void ImportAssimpVector2DData(VertexAttributeRef<Vector2>& ref, VertexID id, const aiVector3D& vector) {
        ref[id].x = vector.x;
        ref[id].y = vector.y;
    }

#define TryImportVector3DData(ref, refId, ptrVector, ptrVectorId) if (ptrVector != nullptr) { ImportAssimpVector3DData(ref, refId, ptrVector[ptrVectorId]); }
#define TryImportVector2DData(ref, refId, ptrVector, ptrVectorId) if (ptrVector != nullptr) { ImportAssimpVector2DData(ref, refId, ptrVector[ptrVectorId]); }

    void StartSubmeshImportingProcess(aiMesh* mesh, const aiScene* scene, Mesh* outMesh) {
        MeshDescription desc;
        MeshAttributes meshAttributes(desc);

        outMesh->AddMeshElement(&desc);
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            VertexID v = desc.CreateVertex();
            TryImportVector3DData(meshAttributes.GetVertexPositions(), v, mesh->mVertices, i);
            TryImportVector3DData(meshAttributes.GetVertexTangents(), v, mesh->mTangents, i);
            TryImportVector3DData(meshAttributes.GetVertexBitangents(), v, mesh->mBitangents, i);
            TryImportVector3DData(meshAttributes.GetVertexNormals(), v, mesh->mNormals, i);
            TryImportVector2DData(meshAttributes.GetVertexUVs(), v, mesh->mTextureCoords[0], i);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            desc.CreateFace({ (int)face.mIndices[0], (int)face.mIndices[1], (int)face.mIndices[2] });
        }
    }

    void StartMeshImportingProcess(aiNode* node, const aiScene* scene, Mesh* outMesh) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            StartSubmeshImportingProcess(scene->mMeshes[node->mMeshes[i]], scene, outMesh);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            StartMeshImportingProcess(node->mChildren[i], scene, outMesh);
        }
    }

    void MeshImporter::ImportTo(const String& filename, Object* object) {
        if (!object->Is(Mesh::TypeIdClass())) {
            throw EngineException("[MeshImporter] Object::Is() failed. Object is not a mesh object");
        }

        Assimp::Importer importer;

        importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f);
        importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

        const aiScene* scene = importer.ReadFile(
            filename,
            aiProcessPreset_TargetRealtime_Quality |
            aiProcess_ConvertToLeftHanded |
            //aiProcess_RemoveComponent |
            //aiProcess_OptimizeGraph |
            aiProcess_OptimizeMeshes |
            0
        );

        if (scene == nullptr) {
            throw new EngineException("[MeshImport] Failed to load model: " + filename);
        }

        StartMeshImportingProcess(scene->mRootNode, scene, object->As<Mesh>());
    }
}
