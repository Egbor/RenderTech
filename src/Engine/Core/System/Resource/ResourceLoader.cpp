#include "Engine/Core/System/Resource/ResourceLoader.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include "Engine/Object/Class/Material.h"
#include "Engine/Object/Class/Texture.h"
#include "Engine/Object/Class/Shader.h"
#include "Engine/Object/Class/Mesh.h"

#include "Engine/Rendering/MeshAttributes.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <rapidxml-1.13/rapidxml.hpp>
#include <rapidxml-1.13/rapidxml_utils.hpp>
#include <FreeImage.h>

#include <filesystem>
#include <sstream>

namespace Engine {


    // Section of texture resource loader


    TextureFormat ToFormat(FIBITMAP* dib) {
        switch (FreeImage_GetBPP(dib)) {
        case 8: {
            switch (FreeImage_GetImageType(dib)) {
            case FIT_BITMAP: return TextureFormat::TF_R8_BMP;//DXGI_FORMAT_R8_UNORM;
            default: throw EngineException("Unknown 8 bit texture format");
            }
        }
        case 16: {
            switch (FreeImage_GetImageType(dib)) {
            case FIT_BITMAP: return TextureFormat::TF_R8G8_BMP;//DXGI_FORMAT_R8G8_UNORM;
            case FIT_UINT16: return TextureFormat::TF_R16_UINT;//DXGI_FORMAT_R16_UINT;
            case FIT_INT16: return TextureFormat::TF_R16_INT;//DXGI_FORMAT_R16_SINT;
            default: throw EngineException("Unknown 16 bit texture format");
            }
        }
        case 32: {
            switch (FreeImage_GetImageType(dib)) {
            case FIT_BITMAP:
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_BGR
                return TextureFormat::TF_B8G8R8A8_BMP;//DXGI_FORMAT_B8G8R8A8_UNORM;
#else
                return TextureFormat::TF_R8G8B8A8_BMP;//DXGI_FORMAT_R8G8B8A8_UNORM;
#endif
            case FIT_UINT32: return TextureFormat::TF_R32_UINT;//DXGI_FORMAT_R32_UINT;
            case FIT_INT32: return TextureFormat::TF_R32_INT;//DXGI_FORMAT_R32_SINT;
            case FIT_FLOAT: return TextureFormat::TF_R32_FLOAT;//DXGI_FORMAT_R32_FLOAT;
            default: throw EngineException("Unknown 32 bit texture format");
            }
        }
        case 128:
            switch (FreeImage_GetImageType(dib)) {
            case FIT_RGBAF: return TextureFormat::TF_R32G32B32A32_FLOAT;
            default: throw EngineException("Unknown 128 bit texture format");
            }

        default:
            throw EngineException("Unknown texture format");
        }
    }

    FREE_IMAGE_TYPE ToFreeImageFormat(TextureFormat format) {
        switch (format) {
        case TextureFormat::TF_R8G8B8A8_BMP:
        case TextureFormat::TF_B8G8R8A8_BMP:
            return FIT_BITMAP;
        case TextureFormat::TF_R32G32B32A32_FLOAT: return FIT_RGBAF;
        case TextureFormat::TF_R16_INT: return FIT_INT16;
        case TextureFormat::TF_R16_UINT: return FIT_UINT16;
        case TextureFormat::TF_R32_INT: return FIT_INT32;
        case TextureFormat::TF_R32_UINT: return FIT_UINT32;
        case TextureFormat::TF_R32_FLOAT: return FIT_FLOAT;
        default:
            throw EngineException("Unknown texture format");
        }
        return FIT_UNKNOWN;
    }

    FREE_IMAGE_FORMAT GetFreeImageFormat(FIBITMAP* dib) {
        if (FreeImage_GetBPP(dib) > 32) {
            return FIF_EXR;
        } else {
            return FIF_PNG;
        }
    }

    Size GetFreeImageByteWidth(FIBITMAP* dib) {
        UInt32 bytes = FreeImage_GetBPP(dib) / 8;
        Int32 width = FreeImage_GetWidth(dib);
        Int32 height = FreeImage_GetHeight(dib);
        return bytes * width * height;
    }

    FIBITMAP* LoadFreeImageResource(const String& filename) {
        FREE_IMAGE_FORMAT fiFormat = FreeImage_GetFileType(filename.c_str());

        if (fiFormat == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(fiFormat)) {
            throw EngineException("Unknown file format: " + filename);
        }

        FIBITMAP* dib = FreeImage_Load(fiFormat, filename.c_str());

        if (dib == nullptr || FreeImage_HasPixels(dib) == FALSE) {
            throw EngineException("Failed to load texture: " + filename);
        }

        unsigned int strides = FreeImage_GetBPP(dib);
        if (!IS_POWER_OF_TWO(strides)) {
            FIBITMAP* dibExd = nullptr;

            if (strides < 32) {
                dibExd = FreeImage_ConvertTo32Bits(dib);
            } else {
                dibExd = FreeImage_ConvertToRGBAF(dib);
            }

            FreeImage_Unload(dib);
            dib = dibExd;
        }
        return dib;
    }

    void UnloadFreeImageResources(const Array<FIBITMAP*>& dibs) {
        for (FIBITMAP* dib : dibs) {
            FreeImage_Unload(dib);
        }
    }

    template<>
    void ResourceLoader::Load(ResourceNode* root, const String& path, Texture2D* outResource) {
        Int32 dataSize = 0;

        Array<String> paths;

        if (outResource->IsCubemap()) {
            dataSize = NUMBER_OF_FACES_FOR_CUBEMAP_TEXTURE;
            paths.resize(dataSize);

            // Setup texture factory for a cubemap
        } else {
            dataSize = NUMBER_OF_FACES_FOR_SINGLE_TEXTURE;
            paths.resize(dataSize);

            // Setup texture factory for a regular texture
        }

        Array<Int8*> data(dataSize);
        Array<FIBITMAP*> dibs(dataSize);

        for (Int32 i = 0; i < dataSize; i++) {
            dibs[i] = LoadFreeImageResource(paths[i]);
            data[i] = reinterpret_cast<Int8*>(FreeImage_GetBits(dibs[i]));
        }

        UnloadFreeImageResources(dibs);
        //FIBITMAP* dib = LoadFreeImageResource(root->Filename(path));

        //Array<Int8*> data;
        //Int32 width = static_cast<Int32>(FreeImage_GetWidth(dib));
        //Int32 height = static_cast<Int32>(FreeImage_GetHeight(dib));
        //data.push_back(reinterpret_cast<Int8*>(FreeImage_GetBits(dib)));

        ////Texture2D* texture = ClassType<Texture2D>::CreateObject(ObjectArgument::Dummy());
        //outResource->Create(width, height, ToFormat(dib), data);

        //FreeImage_Unload(dib);

        root->Set(path, outResource);
    }

    template<>
    TextureCube* LoadResourceFromFile(const String& filename) {
        std::filesystem::path filepath(filename);

        Array<FIBITMAP*> dibs;
        Array<Int8*> data;

        for (Int32 i = 0; i < 6; i++) {
            std::filesystem::path tempFilepath(filepath);
            tempFilepath.replace_filename(filepath.stem().string() + std::to_string(i + 1) + filepath.extension().string());

            dibs.push_back(LoadFreeImageResource(tempFilepath.string()));
            data.push_back(reinterpret_cast<Int8*>(FreeImage_GetBits(dibs[dibs.size() - 1])));
        }

        Int32 width = static_cast<Int32>(FreeImage_GetWidth(dibs[0]));
        Int32 height = static_cast<Int32>(FreeImage_GetHeight(dibs[0]));

        TextureCube* texture = ClassType<TextureCube>::CreateObject(ObjectArgument::Dummy());
        texture->Create(width, height, ToFormat(dibs[0]), data);

        UnloadFreeImageResources(dibs);

        return texture;
    }


    // Section of shader resource loader


    template<>
    void ResourceLoader::Load(ResourceNode* root, const String& path, Shader* outResource) {
        std::filesystem::path filepath(root->Filename(path));
        std::ifstream in(filepath, std::ios::binary);

        if (!in.is_open()) {
            throw EngineException("[Resource] Binary file can not be opened");
        }

        in.seekg(0, std::ios::end);

        Array<Int8> code(in.tellg());

        in.seekg(0, std::ios::beg);
        in.read(reinterpret_cast<char*>(code.data()), code.size());

        outResource->Build(code);
        root->Set(path, outResource);
    }


    // Section of material resource loader


#define RTMTL_TAG_TYPE(rtmtlAttributes) rtmtlAttributes.at("type")
#define RTMTL_TAG_PATH(rtmtlAttributes) rtmtlAttributes.at("path")

#define RTMTL_SHADER_TYPE(rtmtlAttributes) rtmltShaderTypes.at(RTMTL_TAG_TYPE(rtmtlAttributes))
#define RTMTL_CALL_LOADER(tag, attributes, root, material) rtmltLoaders.at(tag)(attributes, root, material) 

    void LoadTextureFromMaterial(const Map<String, String>& attributes, ResourceNode* root, Material* out);
    void LoadShaderFromMaterial(const Map<String, String>& attributes, ResourceNode* root, Material* out);


    static const Map<String, RenderStage> rtmltShaderTypes = {
        { "vertex", RenderStage::RS_VERTEX },
        { "pixel", RenderStage::RS_PIXEL },
    };

    static const Map<String, std::function<void(const Map<String, String>&, ResourceNode*, Material*)>> rtmltLoaders {
        { "texture2d", LoadTextureFromMaterial },
        { "shader", LoadShaderFromMaterial },
    };

    void LoadTextureFromMaterial(const Map<String, String>& attributes, ResourceNode* root, Material* out) {
        Texture2D* texture = dynamic_cast<Texture2D*>(root->Get(RTMTL_TAG_PATH(attributes)));
        if (texture == nullptr) {
            texture = ClassType<Texture2D>::CreateObject(ObjectArgument::Dummy());

            ResourceLoader::Load<Texture2D>(root, RTMTL_TAG_PATH(attributes), texture);
        }
        out->AddTexture(texture);
    }

    void LoadShaderFromMaterial(const Map<String, String>& attributes, ResourceNode* root, Material* out) {
        Shader* shader = dynamic_cast<Shader*>(root->Get(RTMTL_TAG_PATH(attributes)));
        if (shader == nullptr) {
            shader = ClassType<Shader>::CreateObject(ObjectArgument::Dummy());
            shader->Init(RTMTL_SHADER_TYPE(attributes));
            
            ResourceLoader::Load<Shader>(root, RTMTL_TAG_PATH(attributes), shader);
        }
        out->AddShader(shader);
    }

	template<>
	void ResourceLoader::Load(ResourceNode* root, const String& path, Material* outResource) {
		rapidxml::file<>* xmlFile = new rapidxml::file<>(root->Filename(path).c_str());
		rapidxml::xml_document<>* xmlDocument = new rapidxml::xml_document<>();

		xmlDocument->parse<0>(xmlFile->data());

		rapidxml::xml_node<>* xmlRoot = xmlDocument->first_node();
		if (std::strcmp(boost::algorithm::to_lower_copy(xmlRoot->name()), "material") != 0) {
			throw EngineException("[Resource] The XML file does not meet material conditions");
		}

		for (rapidxml::xml_node<>* node = xmlRoot->first_node(); node != nullptr; node = node->next_sibling()) {
			Map<String, String> xmlAtributes;
			for (rapidxml::xml_attribute<>* attr = node->first_attribute(); attr != nullptr; attr = attr->next_attribute()) {
				xmlAtributes[boost::algorithm::to_lower_copy(attr->name())] = boost::algorithm::to_lower_copy(attr->value());
			}
            RTMTL_CALL_LOADER(boost::algorithm::to_lower_copy(node->name()), xmlAtributes, root, outResource);
		}

		delete xmlDocument;
		delete xmlFile;

        root->Set(path, outResource);
	}


    // Section of mesh resource loader


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

    void StartSubmeshImportingProcess(aiMesh* mesh, const aiScene* scene, BasicMesh* outMesh) {
        MeshDescription desc;
        MeshAttributes meshAttributes(desc);

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

        outMesh->AddSubmesh(&desc);
    }

    void StartMeshImportingProcess(aiNode* node, const aiScene* scene, BasicMesh* outMesh) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            StartSubmeshImportingProcess(scene->mMeshes[node->mMeshes[i]], scene, outMesh);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            StartMeshImportingProcess(node->mChildren[i], scene, outMesh);
        }
    }

    template<>
    void ResourceLoader::Load(ResourceNode* root, const String& path, BasicMesh* outResource) {
        Assimp::Importer importer;

        importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f);
        importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

        const aiScene* scene = importer.ReadFile(
            root->Filename(path),
            aiProcessPreset_TargetRealtime_Quality |
            aiProcess_ConvertToLeftHanded |
            //aiProcess_RemoveComponent |
            //aiProcess_OptimizeGraph |
            aiProcess_OptimizeMeshes |
            0
        );

        if (scene == nullptr) {
            throw EngineException("[Resource] Failed to load model");
        }
        StartMeshImportingProcess(scene->mRootNode, scene, outResource);

        root->Set(path, outResource);
    }
}