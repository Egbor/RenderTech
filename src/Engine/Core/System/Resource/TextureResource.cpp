#include "Engine/Core/System/Resource/Resource.h"
#include "Engine/Core/System/Exception/EngineException.h"
#include "Engine/Core/Render/Api/RenderDef.h"

#include "Engine/Object/Class/Texture.h"

#include <FreeImage.h>

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
            }
            else {
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

    template<>
    Texture2D* Resource::Load(const String& filename) {
        FIBITMAP* dib = LoadFreeImageResource(filename);

        Array<Int8*> data;
        Int32 width = static_cast<Int32>(FreeImage_GetWidth(dib));
        Int32 height = static_cast<Int32>(FreeImage_GetHeight(dib));
        data.push_back(reinterpret_cast<Int8*>(FreeImage_GetBits(dib)));

        Texture2D* texture = ClassType<Texture2D>::CreateObject(ObjectArgument::Dummy());
        texture->Create(width, height, ToFormat(dib), data);

        FreeImage_Unload(dib);

        return texture;
    }
}