#include "Engine/Core/System/Import/MeshImport.h"
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

        Mesh* mesh = ClassType<Mesh>::CreateObject(ObjectArgument::Dummy());
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

        outMesh->AddSubmesh(&desc);
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

    MeshImporter::MeshImporter(const String& filename)
        : Importer(filename) {

    }

    void MeshImporter::ImportTo(Object* object) {
        Importer::ImportTo(object);

        if (!object->Is(Mesh::TypeIdClass())) {
            throw EngineException("[MeshImporter] Object::Is() failed. Object is not a mesh object");
        }

        Assimp::Importer importer;

        importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f);
        importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

        const aiScene* scene = importer.ReadFile(
            GetFilename(),
            aiProcessPreset_TargetRealtime_Quality |
            aiProcess_ConvertToLeftHanded |
            //aiProcess_RemoveComponent |
            //aiProcess_OptimizeGraph |
            aiProcess_OptimizeMeshes |
            0
        );

        if (scene == nullptr) {
            throw new EngineException("[MeshImport] Failed to load model: " + GetFilename());
        }

        StartMeshImportingProcess(scene->mRootNode, scene, object->As<Mesh>());
    }
}
