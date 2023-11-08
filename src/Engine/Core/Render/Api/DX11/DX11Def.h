#ifndef DX11DEF_H
#define DX11DEF_H

#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_3.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>
#include <wrl\client.h>

#include "Engine/EngineDef.h"
#include "Engine/Core/Render/Api/RenderDef.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"

using namespace Microsoft::WRL;

constexpr INT D3D11InputStride = 56;
constexpr D3D11_INPUT_ELEMENT_DESC D3D11InputDesc[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0                           , D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

UINT GetD3D11FormatSizeOf(DXGI_FORMAT format) {
    switch (format) {
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
        return 16;
    case DXGI_FORMAT_R32_SINT:
    case DXGI_FORMAT_R32_UINT:
    case DXGI_FORMAT_R32_FLOAT:
    case DXGI_FORMAT_R24G8_TYPELESS:
    case DXGI_FORMAT_R8G8B8A8_UNORM:
    case DXGI_FORMAT_B8G8R8A8_UNORM:
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        return 4;
    case DXGI_FORMAT_R16_SINT:
    case DXGI_FORMAT_R16_UINT:
    case DXGI_FORMAT_R8G8_UNORM:
        return 2;
    case DXGI_FORMAT_R8_UNORM:
        return 1;
    default:
        break;
    }
    return 0;
}

DXGI_FORMAT GetD3D11Format(Engine::TextureFormat format) {
    switch (format) {
    case Engine::TextureFormat::TF_R8_BMP:
        return DXGI_FORMAT_R8_UNORM;
    case Engine::TextureFormat::TF_R8G8_BMP:
        return DXGI_FORMAT_R8G8_UNORM;
    case Engine::TextureFormat::TF_R8G8B8A8_BMP:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
    case Engine::TextureFormat::TF_B8G8R8A8_BMP:
        return DXGI_FORMAT_B8G8R8A8_UNORM;
    case Engine::TextureFormat::TF_R8G8B8A8_BMP_sRGB:
        return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    case Engine::TextureFormat::TF_B8G8R8A8_BMP_sRGB:
        return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    case Engine::TextureFormat::TF_R16_INT:
        return DXGI_FORMAT_R16_SINT;
    case Engine::TextureFormat::TF_R32_INT:
        return DXGI_FORMAT_R32_SINT;
    case Engine::TextureFormat::TF_R16_UINT:
        return DXGI_FORMAT_R16_UINT;
    case Engine::TextureFormat::TF_R32_UINT:
        return DXGI_FORMAT_R32_UINT;
    case Engine::TextureFormat::TF_R32_FLOAT:
        return DXGI_FORMAT_R32_FLOAT;
    case Engine::TextureFormat::TF_R32G32B32A32_FLOAT:
        return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case Engine::TextureFormat::TF_R24G8_BMP:
        return DXGI_FORMAT_R24G8_TYPELESS;
    case Engine::TextureFormat::TF_R24_BMP_G8_UINT:
        return DXGI_FORMAT_D24_UNORM_S8_UINT;
    default:
        break;
    }
    return DXGI_FORMAT_UNKNOWN;
}

#endif // DX11DEF_H
