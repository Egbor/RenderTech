#include "Engine/Core/Core.h"
#include "Engine/Core/Render/Api/DX12/DX12Context.h"
#include "Engine/Core/Render/Api/DX12/DX12Pipeline.h"
#include "Engine/Core/Render/Api/DXCommon/DXCShader.h"

namespace Engine {
	DX12PipelineFactory::DX12PipelineFactory(const DX12Context* dxContext) 
		: m_dxContext(dxContext) {
		Reset();
	}

	void DX12PipelineFactory::InitD3D12Signature(ComPtr<ID3D12Device> d3dDevice, const IID& riid, void** ppvRootSignature) const {
		CD3DX12_ROOT_SIGNATURE_DESC  d3dxRootSignatureDesc;
		d3dxRootSignatureDesc.Init(m_d3dRootParams.size(), m_d3dRootParams.data(), 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> d3dSignature;
		ComPtr<ID3DBlob> d3dError;
		D3D12SerializeRootSignature(&d3dxRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &d3dSignature, &d3dError);
		d3dDevice->CreateRootSignature(0, d3dSignature->GetBufferPointer(), d3dSignature->GetBufferSize(), riid, ppvRootSignature);
	}

	void DX12PipelineFactory::InitD3D12Pipeline(ComPtr<ID3D12Device> d3dDevice, ComPtr<ID3D12RootSignature> d3dSignature, const IID& riid, void** ppvPipelineState) const {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dCopyOfPipelineDesc = m_d3dPipelineDesc;
		d3dCopyOfPipelineDesc.pRootSignature = d3dSignature.Get();

		if (FAILED(d3dDevice->CreateGraphicsPipelineState(&d3dCopyOfPipelineDesc, riid, ppvPipelineState))) {
			_Core::InitiateFatalError(ENGINE_DEBUG_MESSAGE("ID3D12Device::CreateGraphicsPipelineState() failed"));
		}
	}

	void DX12PipelineFactory::Reset() {
		ZeroMemory(&m_d3dPipelineDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		m_d3dPipelineDesc.InputLayout.pInputElementDescs = D3D12InputDesc;
		m_d3dPipelineDesc.InputLayout.NumElements = _countof(D3D12InputDesc);
		m_d3dPipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		m_d3dPipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		m_d3dPipelineDesc.DepthStencilState.DepthEnable = FALSE;
		m_d3dPipelineDesc.DepthStencilState.StencilEnable = FALSE;
		m_d3dPipelineDesc.SampleMask = UINT_MAX;
		m_d3dPipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		m_d3dPipelineDesc.NumRenderTargets = 1;
		m_d3dPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_d3dPipelineDesc.SampleDesc.Count = 1;
	}

	IRenderPipelineFactory* DX12PipelineFactory::SetVSShader(const ShaderResource* shader) {
		assert(shader->Is(RenderStage::RS_VERTEX));

		const DXCShader* dxShader = dynamic_cast<const DXCShader*>(shader);
		ComPtr<ID3DBlob> d3dShader = dxShader->GetD3DInterface();

		m_d3dPipelineDesc.VS.BytecodeLength = d3dShader->GetBufferSize();
		m_d3dPipelineDesc.VS.pShaderBytecode = d3dShader->GetBufferPointer();

		return this;
	}

	IRenderPipelineFactory* DX12PipelineFactory::SetPSShader(const ShaderResource* shader) {
		assert(shader->Is(RenderStage::RS_PIXEL));

		const DXCShader* dxShader = dynamic_cast<const DXCShader*>(shader);
		ComPtr<ID3DBlob> d3dShader = dxShader->GetD3DInterface();

		m_d3dPipelineDesc.PS.BytecodeLength = d3dShader->GetBufferSize();
		m_d3dPipelineDesc.PS.pShaderBytecode = d3dShader->GetBufferPointer();

		return this;
	}

	IRenderPipeline* DX12PipelineFactory::Build() const {
		return new DX12Pipeline(this, m_dxContext);
	}

	DX12Pipeline::DX12Pipeline(const DX12PipelineFactory* dxFactory, const DX12Context* dxContext) {
		ComPtr<ID3D12Device> d3dDevice = dxContext->GetD3D12Device();
		ComPtr<ID3D12CommandAllocator> d3dAllocator = dxContext->GetD3D12CommandAllocator();

		dxFactory->InitD3D12Signature(d3dDevice, IID_PPV_ARGS(&m_d3dSignature));
		dxFactory->InitD3D12Pipeline(d3dDevice, m_d3dSignature, IID_PPV_ARGS(&m_d3dPipeline));

		if (FAILED(d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, d3dAllocator.Get(), m_d3dPipeline.Get(), IID_PPV_ARGS(&m_d3dCommandList)))) {
			_Core::InitiateFatalError(ENGINE_DEBUG_MESSAGE("ID3D12Device::CreateCommandList() failed"));
		}
		m_d3dCommandList->Close();
	}
}