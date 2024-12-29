#include "Engine/Core/Render/Api/DX12/DX12Pipeline.h"

namespace Engine {
	DX12Pipeline::DX12Pipeline() {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
		ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		d3dPipelineStateDesc.InputLayout.pInputElementDescs = D3D12InputDesc;
		d3dPipelineStateDesc.InputLayout.NumElements = _countof(D3D12InputDesc);
	}
}