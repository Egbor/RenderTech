#ifndef RENDERDEF_H
#define RENDERDEF_H

#include "Engine/EngineDef.h"
#include "Engine/Math/Matrix.h"

namespace Engine {
#define RENDER_OUTPUT_COUNT 9
#define RENDER_BUFFER_COUNT 2
#define RENDER_STAGE_COUNT 2
#define RENDER_VIEWPORT_COUNT 2
#define RENDER_TEXTURE_COUNT 8

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
        Float width;
        Float height;
    };

#define tagREALTIME_BUFFER_BINDER           "BufferBinder"
#define tagREALTIME_SHADER_BINDER           "ShaderBinder"
#define tagREALTIME_TARGET_BINDER           "TargetBinder"
#define tagREALTIME_TEXTURE_BINDER          "TextureBinder"
#define tagREALTIME_VIEWPORT_BINDER         "ViewportBinder"
#define tagREALTIME_DYNAMIC_BUFFER_BINDER   "DynamicBufferBinder"

#define tagREALTIME_RASTERIZER_STATE    "RasterizerState"
#define tagREALTIME_SAMPLER_STATE       "SamplerState"
#define tagREALTIME_DEPTHSTENCIL_STATE  "DepthStencilState"
#define tagREALTIME_BLEND_STATE         "BlendState"

    static constexpr const char* tagTexture = "Texture";
    static constexpr const char* tagCubeTexture = "CubeTexture";
    static constexpr const char* tagTargetTexture = "TargetTexture";
    static constexpr const char* tagCubeMapTargetTexture = "CubeMapTargetTexture";
    static constexpr const char* tagDepthStencilTargetTexture = "DepthStencilTargetTexture";

#define tagREALTIME_BUFFER_VERTEX           "VertexBuffer"
#define tagREALTIME_BUFFER_INDEX            "IndexBuffer"
#define tagREALTIME_BUFFER_VS_CONST_BUFFER  "VsConstBuffer"
#define tagREALTIME_BUFFER_PS_CONST_BUFFER  "PsConstBuffer"

#define tagREALTIME_SHADER_VERTEX       "VertexShader"
#define tagREALTIME_SHADER_FRAGMENT      "FragmentShader"

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
        LT_POINT        = 0,
        LT_SPOT         = 1,
        LT_DIRECTIONAL  = 2
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

    struct DYNAMIC_BUFFER UB_System {
        Vector2 Resolution;
    };

    struct DYNAMIC_BUFFER UB_Light {
        Vector4 Color;
        Vector4 Value;
        Vector4 Brightness;
    };
}

#endif // RENDERDEF_H
