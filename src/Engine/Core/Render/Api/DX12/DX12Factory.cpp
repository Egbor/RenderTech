#include "Engine/Core/Core.h"
#include "Engine/Core/Render/Api/DX12/DX12Factory.h"
#include "Engine/Core/Render/Api/DX12/DX12Context.h"

#define MAX_NUMBER_RTV_DESCRIPTORS 16

namespace Engine {
	DX12ResourceFactory::DX12ResourceFactory(const DX12Context* dxContext) {
		ComPtr<ID3D12Device> d3dDevice = dxContext->GetD3D12Device();

		D3D12_DESCRIPTOR_HEAP_DESC d3dRTVHeapDesc;
		ZeroMemory(&d3dRTVHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
		d3dRTVHeapDesc.NumDescriptors = MAX_NUMBER_RTV_DESCRIPTORS;
		d3dRTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		d3dRTVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		if (FAILED(d3dDevice->CreateDescriptorHeap(&d3dRTVHeapDesc, IID_PPV_ARGS(&m_d3dRTVDescriptionHeap)))) {
			_Core::InitiateFatalError(ENGINE_DEBUG_MESSAGE("ID3D12Device::CreateDescriptionHeap() for RTV failed"));
		}
	}

	StateResource* DX12ResourceFactory::CreateState(StateType type, const String& name, StateData data) {
		return nullptr;
	}

	ShaderResource* DX12ResourceFactory::CreateShader(RenderStage stage, const String& name, Size codeLength, const void* code) {
		return nullptr;
	}

	BufferResource* DX12ResourceFactory::CreateBuffer(BufferType type, const String& name, Int32 size, Int32 strides, const void* data) {
		return nullptr;
	}

	TargetResource* DX12ResourceFactory::CreateTarget(TextureType type, const String& name, TextureFormat format, Int32 width, Int32 height) {
		return nullptr;
	}

	TextureResource* DX12ResourceFactory::CreateTexture(TextureType type, const String& name, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) {
		return nullptr;
	}
}