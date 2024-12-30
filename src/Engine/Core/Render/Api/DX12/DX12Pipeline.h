#ifndef DX12PIPELINE_H
#define DX12PIPELINE_H

#include "Engine/Core/Render/Api/DX12/DX12Def.h"
#include "Engine/Core/Render/Base/Interface/IRenderPipeline.h"
#include "Engine/Core/Render/Base/Resource/ShaderResource.h"

namespace Engine {
	class DX12PipelineFactory : public IRenderPipelineFactory {
	public:
		DX12PipelineFactory(const DX12Context* dxContext);

		void InitD3D12Signature(ComPtr<ID3D12Device> d3dDevice, const IID& riid, void** ppvRootSignature) const;
		void InitD3D12Pipeline(ComPtr<ID3D12Device> d3dDevice, ComPtr<ID3D12RootSignature> d3dSignature, const IID& riid, void** ppvPipelineState) const;

		IRenderPipelineFactory* SetVSShader(const ShaderResource* shader) override;
		IRenderPipelineFactory* SetPSShader(const ShaderResource* shader) override;

		IRenderPipeline* Build() const override;

	private:
		void Reset();

		D3D12_GRAPHICS_PIPELINE_STATE_DESC m_d3dPipelineDesc;
		Array<CD3DX12_ROOT_PARAMETER> m_d3dRootParams;

		const DX12Context* m_dxContext;
	};

	class DX12Pipeline : public IRenderPipeline {
	public:
		DX12Pipeline(const DX12PipelineFactory* dxInitializer, const DX12Context* dxContext);

	private:
		ComPtr<ID3D12PipelineState> m_d3dPipeline;
		ComPtr<ID3D12RootSignature> m_d3dSignature;
		ComPtr<ID3D12GraphicsCommandList> m_d3dCommandList;

	private:
		void Draw(BufferResource* vertex, BufferResource* index) override {};
		void DrawWaveframe(BufferResource* vertexBuffer, BufferResource* indexBuffer) override {};

		void SetViewport(Int32 width, Int32 height) override {};
		void SetTargets(const Array<TargetResource*>& targets) override {};
		void SetStates(const Array<StateResource*>& states) override {};

		void WipeTargets() override {};

		void GetViewport(Viewport& viewport) override {};
		IRenderStage* GetStage(RenderStage stage) override { return nullptr; };
	};
}

#endif // !DX12PIPELINE_H
