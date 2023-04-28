#include "MeshImport.h"
#include "Engine/Rendering/MeshAttributes.h"

namespace Engine {
    MeshImport::MeshImport(const String& filename)
        : Import(filename) {
    }

    Mesh* MeshImport::LoadResource() {
        Assimp::Importer importer;

        importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f);
        importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

        const aiScene* scene = importer.ReadFile(
            GetImportingFilename(),
            aiProcessPreset_TargetRealtime_Quality |
            aiProcess_ConvertToLeftHanded |
            //aiProcess_RemoveComponent |
            aiProcess_OptimizeGraph |
            aiProcess_OptimizeMeshes |
            0
        );

        if (scene == nullptr) {
            throw new EngineException("[MeshImport] Failed to load model: " + GetImportingFilename());
        }

        Mesh* mesh = Mesh::TypeMetaClass()->CreateDefaultObject<Mesh>(ObjectArgument::Dummy());
        ProcessNode(scene->mRootNode, scene, mesh);
        return mesh;
    }

    void MeshImport::ProcessNode(aiNode* node, const aiScene* scene, Mesh* outMesh) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            MeshDescription desc;
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ProcessMesh(mesh, scene, &desc);
            outMesh->AddSubmesh(&desc);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene, outMesh);
        }
    }

    void MeshImport::ProcessMesh(aiMesh* mesh, const aiScene* scene, MeshDescription* outMeshDesc) {
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            VertexID v = outMeshDesc->CreateVertex();
            MeshAttributes meshAttributes(*outMeshDesc);

            VertexAttributeRef<Vector3> position = meshAttributes.GetVertexPositions();
            VertexAttributeRef<Vector3> tangent = meshAttributes.GetVertexTangents();
            VertexAttributeRef<Vector3> binormal = meshAttributes.GetVertexBitangents();
            VertexAttributeRef<Vector3> normal = meshAttributes.GetVertexNormals();
            VertexAttributeRef<Vector2> texcoord = meshAttributes.GetVertexUVs();

            position[v].x = mesh->mVertices[i].x;
            position[v].y = mesh->mVertices[i].y;
            position[v].z = mesh->mVertices[i].z;

            if (mesh->mTangents) {
                tangent[v].x = mesh->mTangents[i].x;
                tangent[v].y = mesh->mTangents[i].y;
                tangent[v].z = mesh->mTangents[i].z;
            }

            if (mesh->mBitangents) {
                binormal[v].x = mesh->mBitangents[i].x;
                binormal[v].y = mesh->mBitangents[i].y;
                binormal[v].z = mesh->mBitangents[i].z;
            }

            if (mesh->mNormals) {
                normal[v].x = mesh->mNormals[i].x;
                normal[v].y = mesh->mNormals[i].y;
                normal[v].z = mesh->mNormals[i].z;
            }

            if (mesh->mTextureCoords[0]) {
                texcoord[v].x = mesh->mTextureCoords[0][i].x;
                texcoord[v].y = mesh->mTextureCoords[0][i].y;
            }
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            outMeshDesc->CreateFace({ (int)face.mIndices[0], (int)face.mIndices[1], (int)face.mIndices[2] });
        }
    }
}
