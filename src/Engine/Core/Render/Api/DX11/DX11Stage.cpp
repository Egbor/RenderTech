#define NOMINMAX

#include "Engine/Core/Render/Api/DX11/DX11Stage.h"
#include "Engine/Core/Render/Api/DX11/DX11Buffer.h"
#include "Engine/Core/Render/Api/DX11/DX11Texture.h"
#include "Engine/Core/Render/Api/DX11/DX11Shader.h"
#include "Engine/Core/Render/Api/DX11/DX11SamplerState.h"

#include "Engine/Core/Render/Api/DX11/DX11Context.h"

#include <algorithm>

namespace Engine {
	Int32 BindTexturesWithCallback(const Array<ITextureResourceData*>& resources, Int32 countOfUnbindedResources, ComPtr<ID3D11Device> d3dDevice, std::function<void(const Array<ID3D11ShaderResourceView*>&)> bindCallback) {
		Int32 size = resources.size() + std::max(0, countOfUnbindedResources - static_cast<Int32>(resources.size()));
		
		Array<ComPtr<ID3D11ShaderResourceView>> temp(size);
		for (Size i = 0; i < temp.size(); i++) {
			if (i < resources.size()) {
				temp[i] = dynamic_cast<DX11Texture2D*>(resources[i])->GetD3D11ShaderResourceView(d3dDevice);
			} else {
				temp[i] = nullptr;
			}
		}
		
		Array<ID3D11ShaderResourceView*> textures(size);
		for (Size i = 0; i < textures.size(); i++) {
			textures[i] = temp[i].Get();
		}

		bindCallback(textures);	
		return size;
	}

	void BindSamplersWithCallback(const Array<IStateResourceData*>& resources, std::function<void(Array<ID3D11SamplerState*>&)> bindCallback) {
		Array<ComPtr<ID3D11SamplerState>> temp(resources.size());
		for (Size i = 0; i < temp.size(); i++) {
			temp[i] = dynamic_cast<DX11SamplerState*>(resources[i])->GetD3D11SamplerState();
		}

		Array<ID3D11SamplerState*> samplers(resources.size());
		for (Size i = 0; i < samplers.size(); i++) {
			samplers[i] = temp[i].Get();
		}
		
		bindCallback(samplers);
	}

	void BindBuffersWithCallback(const Array<IBufferResourceData*>& resources, std::function<void(const Array<ID3D11Buffer*>&)> bindCallback) {
		Array<ID3D11Buffer*> buffers(resources.size());
		for (Size i = 0; i < buffers.size(); i++) {
			buffers[i] = dynamic_cast<DX11Buffer*>(resources[i])->GetD3D11Buffer().Get();
		}
		bindCallback(buffers);
	}

	DX11StageVS::DX11StageVS(DX11Context* dxContext)
		: m_dxContext(dxContext), m_countOfTakenTexturesSlots(0), m_lastBindedShader(nullptr) {

	}

	void DX11StageVS::BindTextures(const Array<ITextureResourceData*>& resources) {
		m_countOfTakenTexturesSlots = BindTexturesWithCallback(resources, m_countOfTakenTexturesSlots, m_dxContext->GetD3D11Device(), [&](const Array<ID3D11ShaderResourceView*>& resources) {
			ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();
			d3dContext->VSSetShaderResources(0, static_cast<UINT>(resources.size()), resources.data());
		});
	}

	void DX11StageVS::BindBuffers(const Array<IBufferResourceData*>& resources) {
		ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();

		BindBuffersWithCallback(resources, [&](const Array<ID3D11Buffer*>& buffers) {
			d3dContext->VSSetConstantBuffers(0, static_cast<UINT>(buffers.size()), buffers.data()); 
		});
	}

	void DX11StageVS::BindSamplers(const Array<IStateResourceData*>& resources) {
		ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();

		BindSamplersWithCallback(resources, [&](const Array<ID3D11SamplerState*>& resources) {
			d3dContext->VSSetSamplers(0, static_cast<UINT>(resources.size()), resources.data());
		});
	}

	void DX11StageVS::BindShader(IShaderResourceData* resource) {
		if (m_lastBindedShader == resource) {
			return;
		}
		m_lastBindedShader = resource;

		ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();
		if (resource != nullptr) {
			DX11VertexShader* shader = dynamic_cast<DX11VertexShader*>(resource);
			d3dContext->IASetInputLayout(shader->GetD3D11Layout().Get());
			d3dContext->VSSetShader(shader->GetD3D11Shader().Get(), nullptr, 0);
		} else {
			d3dContext->VSSetShader(nullptr, nullptr, 0);
		}
	}

	DX11StagePS::DX11StagePS(DX11Context* dxContext)
		: m_dxContext(dxContext), m_countOfTakenTexturesSlots(0), m_lastBindedShader(nullptr) {

	}

	void DX11StagePS::BindTextures(const Array<ITextureResourceData*>& resources) {
		m_countOfTakenTexturesSlots = BindTexturesWithCallback(resources, m_countOfTakenTexturesSlots, m_dxContext->GetD3D11Device(), [&](const Array<ID3D11ShaderResourceView*>& resources) {
			ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();
			d3dContext->PSSetShaderResources(0, static_cast<UINT>(resources.size()), resources.data());
		});
	}

	void DX11StagePS::BindBuffers(const Array<IBufferResourceData*>& resources) {
		BindBuffersWithCallback(resources, [&](const Array<ID3D11Buffer*>& buffers) {
			ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();
			d3dContext->PSSetConstantBuffers(0, static_cast<UINT>(buffers.size()), buffers.data());
		});
	}

	void DX11StagePS::BindSamplers(const Array<IStateResourceData*>& resources) {
		ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();

		BindSamplersWithCallback(resources, [&](const Array<ID3D11SamplerState*>& resources) {
			d3dContext->PSSetSamplers(0, static_cast<UINT>(resources.size()), resources.data());
		});
	}

	void DX11StagePS::BindShader(IShaderResourceData* resource) {
		if (m_lastBindedShader == resource) {
			return;
		}
		m_lastBindedShader = resource;

		ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();
		if (resource != nullptr) {
			DX11PixelShader* shader = dynamic_cast<DX11PixelShader*>(resource);
			d3dContext->PSSetShader(shader->GetD3D11Shader().Get(), nullptr, 0);
		} else {
			d3dContext->PSSetShader(nullptr, nullptr, 0);
		}
	}
}