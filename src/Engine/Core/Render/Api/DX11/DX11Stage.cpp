#include "Engine/Core/Render/Api/DX11/DX11Stage.h"
#include "Engine/Core/Render/Api/DX11/DX11Buffer.h"
#include "Engine/Core/Render/Api/DX11/DX11Texture.h"
#include "Engine/Core/Render/Api/DX11/DX11Shader.h"

namespace Engine {
	void BindTexturesWithCallback(const Array<ITextureResourceData*>& resources, ComPtr<ID3D11Device> d3dDevice, std::function<void(const Array<ID3D11ShaderResourceView*>&)> bindCallback) {
		Array<ComPtr<ID3D11ShaderResourceView>> temp(resources.size());
		for (Size i = 0; i < temp.size(); i++) {
			temp[i] = dynamic_cast<DX11Texture2D*>(resources[i])->GetD3D11ShaderResourceView(d3dDevice);
		}
		
		Array<ID3D11ShaderResourceView*> textures(resources.size());
		for (Size i = 0; i < textures.size(); i++) {
			textures[i] = temp[i].Get();
		}
		bindCallback(textures);
	}

	void BindBuffersWithCallback(const Array<IBufferResourceData*>& resources, std::function<void(const Array<ID3D11Buffer*>&)> bindCallback) {
		Array<ID3D11Buffer*> buffers(resources.size());
		for (Size i = 0; i < buffers.size(); i++) {
			buffers[i] = dynamic_cast<DX11Buffer*>(resources[i])->GetD3D11Buffer().Get();
		}
		bindCallback(buffers);
	}

	DX11StageVS::DX11StageVS(ComPtr<ID3D11Device> d3dDevice, ComPtr<ID3D11DeviceContext> d3dContext)
		: m_d3dDevice(d3dDevice), m_d3dContext(d3dContext) {

	}

	void DX11StageVS::BindTextures(const Array<ITextureResourceData*>& resources) {
		BindTexturesWithCallback(resources, m_d3dDevice, [&](const Array<ID3D11ShaderResourceView*>& resources) {
			m_d3dContext->VSSetShaderResources(0, static_cast<UINT>(resources.size()), resources.data());
		});
	}

	void DX11StageVS::BindBuffers(const Array<IBufferResourceData*>& resources) {
		BindBuffersWithCallback(resources, [&](const Array<ID3D11Buffer*>& buffers) {
			m_d3dContext->VSSetConstantBuffers(0, static_cast<UINT>(buffers.size()), buffers.data()); 
		});
	}

	void DX11StageVS::BindShader(IShaderResourceData* resource) {
		DX11VertexShader* shader = dynamic_cast<DX11VertexShader*>(resource);

		m_d3dContext->IASetInputLayout(shader->GetD3D11Layout().Get());
		m_d3dContext->VSSetShader(shader->GetD3D11Shader().Get(), nullptr, 0);
				
	}

	DX11StagePS::DX11StagePS(ComPtr<ID3D11Device> d3dDevice, ComPtr<ID3D11DeviceContext> d3dContext)
		: m_d3dDevice(d3dDevice), m_d3dContext(d3dContext) {

	}

	void DX11StagePS::BindTextures(const Array<ITextureResourceData*>& resources) {
		BindTexturesWithCallback(resources, m_d3dDevice, [&](const Array<ID3D11ShaderResourceView*>& resources) {
			m_d3dContext->PSSetShaderResources(0, static_cast<UINT>(resources.size()), resources.data());
		});
	}

	void DX11StagePS::BindBuffers(const Array<IBufferResourceData*>& resources) {
		BindBuffersWithCallback(resources, [&](const Array<ID3D11Buffer*>& buffers) {
			m_d3dContext->PSSetConstantBuffers(0, static_cast<UINT>(buffers.size()), buffers.data());
		});
	}

	void DX11StagePS::BindShader(IShaderResourceData* resource) {
		DX11PixelShader* shader = dynamic_cast<DX11PixelShader*>(resource);

		m_d3dContext->PSSetShader(shader->GetD3D11Shader().Get(), nullptr, 0);
	}
}