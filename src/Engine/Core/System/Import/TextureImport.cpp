#include "Engine/Core/System/Import/TextureImport.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include "Engine/Object/Global/EngineConfig.h"

#include <FreeImage.h>
#include <sstream>

namespace Engine {
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
        switch(format) {
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

//    String GetImageExtention(FREE_IMAGE_FORMAT format) {
//        switch(format) {
//        case FIF_EXR: return ".exr";
//        case FIF_PNG: return ".png";
//        default:
//            break;
//        }
//        return "";
//    }

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

    void UnloadFreeImageResources(const Array<FIBITMAP*> dibs) {
        for (FIBITMAP* dib : dibs) {
            FreeImage_Unload(dib);
        }
    }

    TextureImport::TextureImport(const String& filename)
        : Import(filename) {

    }

    Texture2D* TextureImport::LoadResource() {
        FIBITMAP* dib = LoadFreeImageResource(GetImportingFilename());

        TextureInfo info;
        info.width = FreeImage_GetWidth(dib);
        info.height = FreeImage_GetHeight(dib);
        info.format = ToFormat(dib);
        info.data.push_back((Int8*)FreeImage_GetBits(dib));

        Texture2D* texture = EngineConfig::GetInstance().GetContext()->CreateTexture2D(tagTexture, info);

        FreeImage_Unload(dib);
        return texture;
    }

    CubeTextureImport::CubeTextureImport(const String& filename)
        : Import(filename) {

    }

    CubeTexture2D* CubeTextureImport::LoadResource() {
        TextureInfo info;
        CubeTexture2D* texture;

        Array<FIBITMAP*> dibs(6);
        for (Int32 face = 0; face < dibs.size(); face++) {
            std::stringstream ss;
            ss << GetImportingFilename() << face;

            dibs[face] = LoadFreeImageResource(ss.str());

            info.width = FreeImage_GetWidth(dibs[face]);
            info.height = FreeImage_GetHeight(dibs[face]);
            info.format = ToFormat(dibs[face]);
            info.data.push_back((Int8*)FreeImage_GetBits(dibs[face]));
        }

        texture = EngineConfig::GetInstance().GetContext()->CreateCubeTexture2D(tagCubeTexture, info);
        UnloadFreeImageResources(dibs);

        return texture;
    }

    void CubeTextureImport::SaveResource(CubeTexture2D* texture) {
        for (Int32 face = 0; face < 6; face++) {
            FIBITMAP* dib = FreeImage_AllocateT(ToFreeImageFormat(texture->GetFormat()),
                                                texture->GetWidth(), texture->GetHeight());
            Int8* bits = (Int8*)FreeImage_GetBits(dib);
            Size bitsSize = GetFreeImageByteWidth(dib);

            EngineConfig::GetInstance().GetContext()->ReadCubeTexture2D(texture, face, bits, bitsSize);

            std::stringstream ss;
            ss << GetImportingFilename() << face;// << GetImageExtention(GetFreeImageFormat(dib));

            FreeImage_Save(GetFreeImageFormat(dib), dib, ss.str().c_str());
            FreeImage_Unload(dib);
        }
    }
}
