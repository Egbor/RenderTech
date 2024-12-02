#ifndef RENDERDEF_H
#define RENDERDEF_H

#include "Engine/EngineDef.h"
#include "Engine/Math/Matrix.h"

namespace Engine {
    enum class RenderOutput {
        RO_TARGET0  = 0,
        RO_TARGET1  = 1,
        RO_TARGET2  = 2,
        RO_TARGET3  = 3,
        RO_TARGET4  = 4,
        RO_TARGET5  = 5,
        RO_TARGET6  = 6,
        RO_TARGET7  = 7,
        RO_DEPTH    = 8
    };

    enum class RenderBuffer {
        RB_VERTEX = 0,
        RB_INDEX = 1,
    };

    enum class RenderDynamicBuffer {
        RDB_SLOT_0  = 0,
        RDB_SLOT_1  = 1,
        RDB_SLOT_2  = 2,
        RDB_SLOT_3  = 3,
        RDB_SLOT_4  = 4,
        RDB_SLOT_5  = 5,
        RDB_SLOT_6  = 6,
        RDB_SLOT_7  = 7,
        RDB_SLOT_8  = 8,
        RDB_SLOT_9  = 9,
        RDB_SLOT_10 = 10,
        RDB_SLOT_11 = 11,
        RDB_SLOT_12 = 12,
        RDB_SLOT_13 = 13,
        RDB_SLOT_14 = 14
    };

    enum class RenderStage {
        RS_VERTEX = 0,
        RS_PIXEL = 1,
        RS_NUM,
    };

    enum class StateType {
        ST_BLEND            = 0,
        ST_SAMPLER          = 1,
        ST_DEPTH_STENCIL    = 2,
        ST_RASTERIZER       = 3
    };

    enum class SamplerFilter {
        SF_POINT        = 0,
        SF_LINEAR       = 1,
        SF_ANISOTROPIC  = 2
    };

    enum class SamplerAddress {
        SA_WRAP         = 0,
        SA_MIRROR       = 1,
        SA_CLAMP        = 2,
        SA_BORDER       = 3,
        SA_MIRROR_ONCE  = 4
    };

    struct Viewport {
    public:
        Viewport() = default;

        Viewport(const Viewport&) = default;
        Viewport& operator=(const Viewport&) = default;

        Viewport(Viewport&&) = default;
        Viewport& operator=(Viewport&&) = default;

        Viewport(Int32 width, Int32 height) noexcept 
            : resolution(static_cast<Float>(width), static_cast<Float>(height)) {

        }

        Int32 GetWidth() const {
            return static_cast<Int32>(resolution.x);
        }

        Int32 GetHeight() const {
            return static_cast<Int32>(resolution.y);
        }

        constexpr bool operator==(const Viewport& other) noexcept {
            return (static_cast<Int32>(resolution.x) == static_cast<Int32>(other.resolution.x)) 
                && (static_cast<Int32>(resolution.y) == static_cast<Int32>(other.resolution.y));
        }

        constexpr bool operator!=(const Viewport& other) noexcept {
            return !(this->operator==(other));
        }

    private:
        Vector2 resolution;
    };

    enum class TextureSlot {
        TS_ALBEDO       = 0,
        TS_NORMAL       = 1,
        TS_METALLIC     = 2,
        TS_ROUGHNESS    = 3,
        TS_AO           = 4,
        TS_DEPTH        = 2,
        TS_ORM          = 3
    };

    enum class TextureType {
        TT_DEFAULT      = 0,
        TT_CUBE         = 1,
        TT_DEPTH        = 3,
        TT_DEPTH_CUBE   = 4
    };

    enum class BufferType {
        BT_VERTEX   = 0,
        BT_INDEX    = 1,
        BT_UNIFORM  = 2
    };

    enum class ShaderType {
        ST_VERTEX   = 0,
        ST_PIXEL    = 1
    };

    enum class LightType {
        LT_POINT = 0,
        LT_SPOT,
        LT_DIRECTIONAL,
        LT_NUM,
        LT_UNDEFINED
    };

    enum class ResourceIdentifier {
        RI_TEXTURE = 0,
        RI_TARGET = 1,
        RI_BUFFER = 2,
        RI_SHADER = 3,
        RI_STATE = 4,
    };

    enum class TextureFormat {
        TF_R8_BMP,
        TF_R8G8_BMP,
        TF_R8G8B8A8_BMP,
        TF_B8G8R8A8_BMP,
        TF_R8G8B8A8_BMP_sRGB,
        TF_B8G8R8A8_BMP_sRGB,
        TF_R16_INT,
        TF_R32_INT,
        TF_R16_UINT,
        TF_R32_UINT,
        TF_R32_FLOAT,
        TF_R32G32B32A32_FLOAT,
        TF_R24G8_BMP,
        TF_R24_BMP_G8_UINT,
        TF_UNKNOWN
    };

    enum class TextureFace {
        TF_POSITIVE_X = 0,
        TF_NEGATIVE_X = 1,
        TF_POSITIVE_Y = 2,
        TF_NEGATIVE_Y = 3,
        TF_POSITIVE_Z = 4,
        TF_NEGATIVE_Z = 5,
        TF_DEFAULT = 0
    };

    constexpr static TextureFormat AdjustTargetFormatForTexture(TextureFormat format) {
        switch (format) {
        case TextureFormat::TF_R24_BMP_G8_UINT:
            return TextureFormat::TF_R24G8_BMP;
        default:
            break;
        }
        return format;
    }

    constexpr static TextureFormat AdjustTextureFormatForTarget(TextureFormat format) {
        switch (format) {
        case TextureFormat::TF_R24G8_BMP:
            return TextureFormat::TF_R24_BMP_G8_UINT;
        default:
            break;
        }
        return format;
    }

    constexpr static Int32 GetBytesPrePixel(TextureFormat format) {
        switch (format) {
        case Engine::TextureFormat::TF_R8_BMP:
            return 1;
        case Engine::TextureFormat::TF_R8G8_BMP:
        case Engine::TextureFormat::TF_R16_UINT:
        case Engine::TextureFormat::TF_R16_INT:
            return 2;
        case Engine::TextureFormat::TF_R8G8B8A8_BMP_sRGB:
        case Engine::TextureFormat::TF_B8G8R8A8_BMP_sRGB:
        case Engine::TextureFormat::TF_R8G8B8A8_BMP:
        case Engine::TextureFormat::TF_B8G8R8A8_BMP:
        case Engine::TextureFormat::TF_R24G8_BMP:
        case Engine::TextureFormat::TF_R24_BMP_G8_UINT:
        case Engine::TextureFormat::TF_R32_INT:
        case Engine::TextureFormat::TF_R32_UINT:
        case Engine::TextureFormat::TF_R32_FLOAT:
            return 4;
        case Engine::TextureFormat::TF_R32G32B32A32_FLOAT:
            return 128;
        default:
            break;
        }
        return 0;
    }

    template<class TResourceClass>
    struct ResourceIdentifierOf {
        static const ResourceIdentifier value;
    };

    union ClearArg {
        Float rgba[4];
        struct {
            Float depth;
            Int32 stencil;
            bool clearDepth;
            bool clearStencil;
        } ds;
    };

    struct TextureInfo {
        UInt32 width;
        UInt32 height;
        TextureFormat format;
        Array<Int8*> data;
    };

    struct BufferInfo {
        const void* data;
        UInt32 bufferSize;
        UInt32 typeSize;
    };

    struct ShaderInfo {
        const void* data;
        Size dataSize;
    };

#define DYNAMIC_BUFFER __declspec(align(16))

    struct DYNAMIC_BUFFER UB_Object {
        Matrix4x4 ViewProjection;
        Matrix4x4 World;
    };

    struct DYNAMIC_BUFFER UB_ObjectHelper {
        Matrix4x4 invWorld;
        Matrix4x4 invView;
        Matrix4x4 invProjection;
    };

    struct DYNAMIC_BUFFER UB_Camera {
        Vector4 EyePosition;
        Vector2 Resolution;
    };

    struct DYNAMIC_BUFFER UB_Light {
        Vector4 Color;
        Vector4 Value;
        Float Brightness;
    };
}

#endif // RENDERDEF_H
