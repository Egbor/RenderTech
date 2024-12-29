#ifndef DX12CONTEXT_H
#define DX12CONTEXT_H

#include "Engine/Core/Render/Api/DX12/DX12Def.h"
#include "Engine/Core/Render/Api/DX12/DX12Factory.h"
#include "Engine/Core/Render/Api/DXCommon/DXCSwapChain.h"
#include "Engine/Core/Render/Base/Interface/IContext.h"

namespace Engine {
	class DX12Context : public IContext {
	public:
		DX12Context();
		~DX12Context() override;

		IRenderResourceFactory* QueryResourceFactory() override;
		IRenderPipeline* QueryPipeline() override;
		ISwapChain* QuerySwapChain() override;

		void Init(const _Core* core) override;

	private:
		ComPtr<ID3D12Device> m_d3dDevice;
		ComPtr<ID3D12CommandQueue> m_d3dQueue;

		DXCSwapChain* m_dxSwapChain;
		DX12Factory* m_dxFactory;
	};
}

#endif // !DX12CONTEXT_H
