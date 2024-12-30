#include "Engine/Core/Core.h"
#include "Engine/Core/Render/Api/Common.h"
#include "Engine/Core/Render/Api/DX12/DX12Context.h"

namespace Engine {
	DX12Context::DX12Context() 
		: m_dxResourceFactory(nullptr), m_dxPipelineFactory(nullptr)
		, m_dxSwapChain(nullptr) {
#if defined(DEBUG) || defined(_DEBUG)
		ComPtr<ID3D12Debug> d3dDebug;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&d3dDebug)))) {
			d3dDebug->EnableDebugLayer();
		}
#endif
	}

	DX12Context::~DX12Context() {
		DELETE_OBJECT(m_dxSwapChain);
		DELETE_OBJECT(m_dxPipelineFactory);
		DELETE_OBJECT(m_dxResourceFactory);
	}

	IRenderResourceFactory* DX12Context::QueryResourceFactory() {
		return m_dxResourceFactory;
	}

	IRenderPipelineFactory* DX12Context::QueryPipelineFactory() {
		return m_dxPipelineFactory;
	}

	IRenderPipeline* DX12Context::QueryPipeline() {
		return nullptr;
	}

	ISwapChain* DX12Context::QuerySwapChain() {
		return m_dxSwapChain;
	}

	void DX12Context::Init(const _Core* core) {
		ComPtr<IDXGIFactory4> dxgiFactory;
		ComPtr<IDXGIAdapter1> dxgiAdapter;

		if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)))) {
			_Core::InitiateFatalError(ENGINE_DEBUG_MESSAGE("CraeteDXGIFactory1() Failed"));
		}

		for (UINT i = 0; DXGI_ERROR_NOT_FOUND == dxgiFactory->EnumAdapters1(i, &dxgiAdapter); ++i) {
			DXGI_ADAPTER_DESC dxgiAdapterDesc;
			dxgiAdapter->GetDesc(&dxgiAdapterDesc);
			if ((VENDOR_ID_NVIDIA == dxgiAdapterDesc.VendorId) || (VENDOR_ID_AMD == dxgiAdapterDesc.VendorId)) {
				break;
			}
		}

		if (FAILED(D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_d3dDevice)))) {
			_Core::InitiateFatalError(ENGINE_DEBUG_MESSAGE("D3D12CreateDevice Failed"));
		}

		D3D12_COMMAND_QUEUE_DESC d3dQueueDesc;
		ZeroMemory(&d3dQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));
		d3dQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		d3dQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		if (FAILED(m_d3dDevice->CreateCommandQueue(&d3dQueueDesc, IID_PPV_ARGS(&m_d3dQueue)))) {
			_Core::InitiateFatalError(ENGINE_DEBUG_MESSAGE("ID3D12Device::CreateCommandQueue() failed"));
		}

		if (FAILED(m_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_d3dAllocator)))) {
			_Core::InitiateFatalError(ENGINE_DEBUG_MESSAGE("ID3D12Device::CreateCommandAllocator() failed"));
		}

		m_dxResourceFactory = new DX12ResourceFactory(this);
		m_dxPipelineFactory = new DX12PipelineFactory(this);
		m_dxSwapChain = new DXCSwapChain(core, dxgiFactory, m_d3dQueue);
	}

	ComPtr<ID3D12Device> DX12Context::GetD3D12Device() const {
		return m_d3dDevice;
	}

	ComPtr<ID3D12CommandAllocator> DX12Context::GetD3D12CommandAllocator() const {
		return m_d3dAllocator;
	}
}