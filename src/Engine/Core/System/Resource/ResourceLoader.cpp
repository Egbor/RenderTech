#include "Engine/Core/System/Resource/ResourceLoader.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include "Engine/Object/Class/Material.h"
#include "Engine/Object/Class/Texture.h"
#include "Engine/Object/Class/Shader.h"
#include "Engine/Object/Class/Mesh.h"

#include "Engine/Rendering/MeshAttributes.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <rapidxml-1.13/rapidxml.hpp>
#include <rapidxml-1.13/rapidxml_utils.hpp>
#include <FreeImage.h>

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

    void StartSubmeshImportingProcess(aiMesh* mesh, const aiScene* scene, StaticMesh::Metadata* metadata) {
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

        metadata->AddSubmesh(&desc);
    }

    void StartMeshImportingProcess(aiNode* node, const aiScene* scene, StaticMesh::Metadata* metadata) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            StartSubmeshImportingProcess(scene->mMeshes[node->mMeshes[i]], scene, metadata);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            StartMeshImportingProcess(node->mChildren[i], scene, metadata);
        }
    }

    Object* LoadAssetStaticMesh(rapidxml::xml_node<>* xmlRoot);
    Object* LoadAssetTexture2D(rapidxml::xml_node<>* xmlRoot);
    Object* LoadAssetMaterial(rapidxml::xml_node<>* xmlRoot);
    Object* LoadAssetCubemap(rapidxml::xml_node<>* xmlRoot);
    Object* LoadAssetShader(rapidxml::xml_node<>* xmlRoot);

    void SaveAssetTexture2D(Object* object);

    void LoadAssetMetadataLeftHandedOptimization(IResourceMetadata* metadata, const Map<String, String>& attributes);
    void LoadAssetMetadataMeshOptimization(IResourceMetadata* metadata, const Map<String, String>& attributes);
    void LoadAssetMetadataResolution(IResourceMetadata* metadata, const Map<String, String>& attributes);
    void LoadAssetMetadataSizeX(IResourceMetadata* metadata, const Map<String, String>& attributes);
    void LoadAssetMetadataType(IResourceMetadata* metadata, const Map<String, String>& attributes);

    static const Map<String, std::function<Object*(rapidxml::xml_node<>*)>> rtAssetLoaders {
        { "staticmesh", LoadAssetStaticMesh },
        { "texture2d", LoadAssetTexture2D },
        { "material", LoadAssetMaterial },
        { "cubemap", LoadAssetCubemap },
        { "shader", LoadAssetShader },
    };

    static const Map<UInt64, std::function<void(Object*)>> rtAssetSavers {
        { Texture2D::TypeIdClass(), SaveAssetTexture2D },
    };

    static const Map<String, std::function<void(IResourceMetadata*, const Map<String, String>&)>> rtAssetMetadataLoaders {
        { "lefthandedoptimization", LoadAssetMetadataLeftHandedOptimization },
        { "meshoptimization", LoadAssetMetadataMeshOptimization },
        { "resolution", LoadAssetMetadataResolution },
        { "sizex", LoadAssetMetadataSizeX },
        { "type", LoadAssetMetadataType },
    };

    static const Map<String, TextureFace> rtFaces {
        { "+x", TextureFace::TF_POSITIVE_X },
        { "-x", TextureFace::TF_NEGATIVE_X },
        { "+y", TextureFace::TF_POSITIVE_Y },
        { "-y", TextureFace::TF_NEGATIVE_Y },
        { "+z", TextureFace::TF_POSITIVE_Z },
        { "-z", TextureFace::TF_NEGATIVE_Z },
    };

    static const Map<String, RenderStage> rtShaderTypes {
        { "vertex", RenderStage::RS_VERTEX },
        { "pixel", RenderStage::RS_PIXEL },
    };

#define RTASSET_TAG_METADATA "metadata"
#define RTASSET_TAG_FACES "faces"
#define RTASSET_TAG_DATA "data"

#define RTASSET_ATTRIBUTE_HEIGHT "height"
#define RTASSET_ATTRIBUTE_WIDTH "width"
#define RTASSET_ATTRIBUTE_VALUE "value"
#define RTASSET_ATTRIBUTE_PATH "path"
#define RTASSET_ATTRIBUTE_TYPE "type"
#define RTASSET_ATTRIBUTE_ID "id"

    //void MaybeInitAssetSavers() {
    //    if (rtAssetSavers.empty()) {
    //        rtAssetSavers[Texture2D::TypeIdClass()] = SaveAssetTexture2D;
    //    }
    //}

    void ParseMetadataSection(IResourceMetadata* metadata, rapidxml::xml_node<>* xmlRoot) {
        Map<String, String> attributes;

        for (rapidxml::xml_node<>* node = xmlRoot->first_node(); node != nullptr; node = node->next_sibling()) {
            for (rapidxml::xml_attribute<>* attr = node->first_attribute(); attr != nullptr; attr = attr->next_attribute()) {
                attributes[boost::algorithm::to_lower_copy(attr->name())] = attr->value();
            }

            auto function = rtAssetMetadataLoaders.at(boost::algorithm::to_lower_copy(node->name()));
            function(metadata, attributes);
        }
    }

    void ParseTextureDataSection(IResourceMetadata* metadata, rapidxml::xml_node<>* xmlRoot) {
        TextureFace face = TextureFace::TF_DEFAULT;
        String path = "";

        for (rapidxml::xml_attribute<>* attr = xmlRoot->first_attribute(); attr != nullptr; attr = attr->next_attribute()) {
            if (strcmp(boost::algorithm::to_lower_copy(attr->name()), RTASSET_ATTRIBUTE_ID) == 0) {
                face = rtFaces.at(boost::algorithm::to_lower_copy(attr->value()));
            }
            if (strcmp(boost::algorithm::to_lower_copy(attr->name()), RTASSET_ATTRIBUTE_PATH) == 0) {
                path = attr->value();
            }
        }

        assert(!path.empty());

        FIBITMAP* dib = LoadFreeImageResource(path);
        Texture2D::Metadata* textureMetadata = dynamic_cast<Texture2D::Metadata*>(metadata);

        assert(FreeImage_GetWidth(dib) == textureMetadata->GetWidth());
        assert(FreeImage_GetHeight(dib) == textureMetadata->GetHeight());

        textureMetadata->SetFormat(ToFormat(dib));
        textureMetadata->SetData(reinterpret_cast<Int8*>(FreeImage_GetBits(dib)), face);

        FreeImage_Unload(dib);
    }

    void ParseShaderDataSection(IResourceMetadata* metadata, rapidxml::xml_node<>* xmlRoot) {
        String path = "";

        for (rapidxml::xml_attribute<>* attr = xmlRoot->first_attribute(); attr != nullptr; attr = attr->next_attribute()) {
            if (strcmp(boost::algorithm::to_lower_copy(attr->name()), RTASSET_ATTRIBUTE_PATH) == 0) {
                path = attr->value();
            }
        }

        assert(!path.empty());

        std::ifstream in(path, std::ios::binary);
        Shader::Metadata* shaderMetadata = dynamic_cast<Shader::Metadata*>(metadata);

        if (!in.is_open()) {
            throw EngineException("[Resource] Shader .CSO file can not be opened");
        }

        in.seekg(0, std::ios::end);

        Int32 dataLength = in.tellg();
        Int8* data = shaderMetadata->SetDataLength(dataLength)->GetData();

        in.seekg(0, std::ios::beg);
        in.read(reinterpret_cast<char*>(data), dataLength);
    }

    void ParseStaticMeshDataSection(IResourceMetadata* metadata, rapidxml::xml_node<>* xmlRoot) {
        String path = "";

        for (rapidxml::xml_attribute<>* attr = xmlRoot->first_attribute(); attr != nullptr; attr = attr->next_attribute()) {
            if (strcmp(boost::algorithm::to_lower_copy(attr->name()), RTASSET_ATTRIBUTE_PATH) == 0) {
                path = attr->value();
            }
        }

        assert(!path.empty());

        Assimp::Importer importer;
        StaticMesh::Metadata* meshMetadata = dynamic_cast<StaticMesh::Metadata*>(metadata);

        UInt32 flags = aiProcessPreset_TargetRealtime_Quality;
        flags |= (meshMetadata->HasLeftHandedOptimization()) ? aiProcess_ConvertToLeftHanded : 0;
        flags |= (meshMetadata->HasMeshOptimization()) ? aiProcess_OptimizeMeshes : 0;

        importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f);
        importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

        const aiScene* scene = importer.ReadFile(path, flags);
        if (scene == nullptr) {
            throw EngineException("[Resource] Failed to load model");
        }

        StartMeshImportingProcess(scene->mRootNode, scene, meshMetadata);
    }

    void LoadAssetMetadataLeftHandedOptimization(IResourceMetadata* metadata, const Map<String, String>& attributes) {
        StaticMesh::Metadata* meshMetadata = dynamic_cast<StaticMesh::Metadata*>(metadata);
        meshMetadata->SetLeftHandedOptimization(true);
    }

    void LoadAssetMetadataMeshOptimization(IResourceMetadata* metadata, const Map<String, String>& attributes) {
        StaticMesh::Metadata* meshMetadata = dynamic_cast<StaticMesh::Metadata*>(metadata);
        meshMetadata->SetMeshOptimization(true);
    }

    void LoadAssetMetadataResolution(IResourceMetadata* metadata, const Map<String, String>& attributes) {
        Texture2D::Metadata* textureMetadata = dynamic_cast<Texture2D::Metadata*>(metadata);

        Int32 width = std::stoi(attributes.at(RTASSET_ATTRIBUTE_WIDTH));
        Int32 height = std::stoi(attributes.at(RTASSET_ATTRIBUTE_HEIGHT));

        assert(!textureMetadata->IsCubemap() || width == height);

        textureMetadata->SetWidth(width)->SetHeight(height);
    }

    void LoadAssetMetadataSizeX(IResourceMetadata* metadata, const Map<String, String>& attributes) {
        Texture2D::Metadata* textureMetadata = dynamic_cast<Texture2D::Metadata*>(metadata);
        Int32 size = std::stoi(attributes.at(RTASSET_ATTRIBUTE_VALUE));
        textureMetadata->SetWidth(size)->SetHeight(size);
    }

    void LoadAssetMetadataType(IResourceMetadata* metadata, const Map<String, String>& attributes) {
        Shader::Metadata* shaderMetadata = dynamic_cast<Shader::Metadata*>(metadata);
        RenderStage stage = rtShaderTypes.at(boost::algorithm::to_lower_copy(attributes.at(RTASSET_ATTRIBUTE_VALUE)));
        shaderMetadata->SetType(stage);
    }


    // Load assets section


    Object* LoadAssetTexture2D(rapidxml::xml_node<>* xmlRoot) {
        Texture2D::Metadata metadata(TextureType::TT_DEFAULT);
        for (rapidxml::xml_node<>* node = xmlRoot->first_node(); node != nullptr; node = node->next_sibling()) {
            if (strcmp(boost::algorithm::to_lower_copy(xmlRoot->name()), RTASSET_TAG_METADATA) == 0) {
                ParseMetadataSection(&metadata, node);
            }

            if (strcmp(boost::algorithm::to_lower_copy(xmlRoot->name()), RTASSET_TAG_DATA) == 0) {
                ParseTextureDataSection(&metadata, node);
            }
        }
        return metadata.Build();
    }

    Object* LoadAssetMaterial(rapidxml::xml_node<>* xmlRoot) {
        Material::Metadata metadata;
        for (rapidxml::xml_node<>* node = xmlRoot->first_node(); node != nullptr; node = node->next_sibling()) {
            if (strcmp(boost::algorithm::to_lower_copy(xmlRoot->name()), RTASSET_TAG_METADATA) == 0) {
                ParseMetadataSection(&metadata, node);
            }

            if (strcmp(boost::algorithm::to_lower_copy(xmlRoot->name()), RTASSET_TAG_DATA) == 0) {
                for (rapidxml::xml_attribute<>* attr = xmlRoot->first_attribute(); attr != nullptr; attr = attr->next_attribute()) {
                    String type = "";
                    String path = "";

                    if (strcmp(boost::algorithm::to_lower_copy(attr->name()), RTASSET_ATTRIBUTE_TYPE) == 0) {
                        type = boost::algorithm::to_lower_copy(attr->value());
                    }

                    if (strcmp(boost::algorithm::to_lower_copy(attr->name()), RTASSET_ATTRIBUTE_PATH) == 0) {
                        path = attr->value();
                    }

                    assert(!(type.empty() || path.empty()));

                    if (strcmp("texture", type.c_str()) == 0) {
                        metadata.AddTexturePath(path);
                    } else if (strcmp("shader", type.c_str()) == 0) {
                        metadata.AddShaderPath(path);
                    }
                }
            }
        }
        return metadata.Build();
    }

    Object* LoadAssetCubemap( rapidxml::xml_node<>* xmlRoot) {
        Texture2D::Metadata metadata(TextureType::TT_CUBE);
        for (rapidxml::xml_node<>* node = xmlRoot->first_node(); node != nullptr; node = node->next_sibling()) {
            if (strcmp(boost::algorithm::to_lower_copy(node->name()), RTASSET_TAG_METADATA) == 0) {
                ParseMetadataSection(&metadata, node);
            }

            if (strcmp(boost::algorithm::to_lower_copy(node->name()), RTASSET_TAG_FACES) == 0) {
                for (rapidxml::xml_node<>* dataNode = node->first_node(); dataNode != nullptr; dataNode = dataNode->next_sibling()) {
                    if (strcmp(boost::algorithm::to_lower_copy(node->name()), RTASSET_TAG_DATA) == 0) {
                        ParseTextureDataSection(&metadata, dataNode);
                    }
                }
            }
        }
        return metadata.Build();
    }

    Object* LoadAssetShader(rapidxml::xml_node<>* xmlRoot) {
        Shader::Metadata metadata;
        for (rapidxml::xml_node<>* node = xmlRoot->first_node(); node != nullptr; node = node->next_sibling()) {
            if (strcmp(boost::algorithm::to_lower_copy(node->name()), RTASSET_TAG_METADATA) == 0) {
                ParseMetadataSection(&metadata, node);
            }

            if (strcmp(boost::algorithm::to_lower_copy(node->name()), RTASSET_TAG_DATA) == 0) {
                ParseShaderDataSection(&metadata, node);
            }
        }
        return metadata.Build();
    }

    Object* LoadAssetStaticMesh(rapidxml::xml_node<>* xmlRoot) {
        StaticMesh::Metadata metadata;
        for (rapidxml::xml_node<>* node = xmlRoot->first_node(); node != nullptr; node = node->next_sibling()) {
            if (strcmp(boost::algorithm::to_lower_copy(node->name()), RTASSET_TAG_METADATA) == 0) {
                ParseMetadataSection(&metadata, node);
            }

            if (strcmp(boost::algorithm::to_lower_copy(node->name()), RTASSET_TAG_DATA) == 0) {
                ParseShaderDataSection(&metadata, node);
            }
        }
        return metadata.Build();
    }

    
    // Save assets section


    void SaveAssetTexture2D(Object* object) {
        const TextureResource* texture = object->As<Texture2D>()->GetNativeResource();
        const Int32 facesNum = texture->IsCubemap() ? NUMBER_OF_FACES_FOR_CUBEMAP_TEXTURE : NUMBER_OF_FACES_FOR_SINGLE_TEXTURE;

        Int32 width = texture->GetWidth();
        Int32 height = texture->GetHeight();

        FREE_IMAGE_TYPE type = ToFreeImageFormat(texture->GetFormat());
        FREE_IMAGE_FORMAT format;

        String path = "bin\\textures\\" + object->GetName();
        String extend;

        if (type == FIT_RGBAF) {
            format = FIF_EXR;
            extend = ".exr";
        } else {
            format = FIF_PNG;
            extend = ".png";
        }

        FIBITMAP* dib = FreeImage_AllocateT(type, width, height);
        for (Int32 face = 0; face < facesNum; face++) {
            Int8* bits = reinterpret_cast<Int8*>(FreeImage_GetBits(dib));
            Int32 size = width * height * FreeImage_GetBPP(dib) / 8;

            texture->ReadByCPUAccess(bits, face, size);
            FreeImage_Save(format, dib, (path + std::to_string(face) + extend).c_str());
        }
        FreeImage_Unload(dib);
    }


    Object* ResourceLoader::Load(const String& path) {        
        rapidxml::file<>* xmlFile = new rapidxml::file<>(path.c_str());
        rapidxml::xml_document<>* xmlDocument = new rapidxml::xml_document<>();

        xmlDocument->parse<0>(xmlFile->data());

        rapidxml::xml_node<>* xmlRoot = xmlDocument->first_node();

        auto function = rtAssetLoaders.at(xmlRoot->name());  
        Object* resource = function(xmlRoot);

        delete xmlDocument;
        delete xmlFile;

        return resource;
    }

    void ResourceLoader::Save(const String& path, Object* object) {
        auto function = rtAssetSavers.at(object->TypeIdClass());
        function(object);
    }
}