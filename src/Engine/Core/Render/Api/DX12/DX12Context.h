#ifndef DX12CONTEXT_H
#define DX12CONTEXT_H

#include "Engine/Core/Render/Api/DX12/DX12Factory.h"
#include "Engine/Core/Render/Api/DX12/DX12Pipeline.h"
#include "Engine/Core/Render/Api/DXCommon/DXCSwapChain.h"
#include "Engine/Core/Render/Base/Interface/IContext.h"

namespace Engine {
	class DX12Context : public IContext {
	public:
		DX12Context();
		~DX12Context() override;

		IRenderResourceFactory* QueryResourceFactory() override;
		IRenderPipelineFactory* QueryPipelineFactory();
		ISwapChain* QuerySwapChain() override;

		void Init(const _Core* core) override;

		ComPtr<ID3D12Device> GetD3D12Device() const;
		ComPtr<ID3D12CommandAllocator> GetD3D12CommandAllocator() const;

	private:
		ComPtr<ID3D12Device> m_d3dDevice;
		ComPtr<ID3D12CommandQueue> m_d3dQueue;
		ComPtr<ID3D12CommandAllocator> m_d3dAllocator;

		DXCSwapChain* m_dxSwapChain;
		DX12ResourceFactory* m_dxResourceFactory;
		DX12PipelineFactory* m_dxPipelineFactory;

	private:
		IRenderPipeline* QueryPipeline() override;
	};
}

#endif // !DX12CONTEXT_H
