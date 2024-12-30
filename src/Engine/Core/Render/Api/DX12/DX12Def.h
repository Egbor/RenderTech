#ifndef DX11DEF_H
#define DX11DEF_H

#include "directx/d3dx12.h"
#include "Engine/Core/Render/Api/DXCommon/DXCDef.h"

namespace Engine {
    class DX12Context;

	constexpr D3D12_INPUT_ELEMENT_DESC D3D12InputDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };
}

#endif // !DX11DEF_H
