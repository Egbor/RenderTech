#ifndef DX12PIPELINE_H
#define DX12PIPELINE_H

#include "Engine/Core/Render/Api/DX12/DX12Def.h"
#include "Engine/Core/Render/Base/Interface/IRenderPipeline.h"

namespace Engine {
	class DX12Pipeline {
	public:
		DX12Pipeline();

	private:
		ComPtr<ID3D12PipelineState> m_d3dPipeline;
		ComPtr<ID3D12GraphicsCommandList> m_d3dCommandList;
	};
}

#endif // !DX12PIPELINE_H
