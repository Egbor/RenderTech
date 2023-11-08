#include "Engine/Core/Render/Api/DX11/DX11Stage.h"
#include "Engine/Core/Render/Api/DX11/DX11Buffer.h"
#include "Engine/Core/Render/Api/DX11/DX11Texture.h"
#include "Engine/Core/Render/Api/DX11/DX11Shader.h"

namespace Engine {
	DX11AbstractStage::DX11AbstractStage(DX11Context* context)
		: m_context(context) {

	}

	void DX11AbstractStage::BindTexturesWithCallback(const Array<ITextureResourceData*>& resources, std::function<void(ComPtr<ID3D11DeviceContext>, const Array<ID3D11ShaderResourceView*>&)> bindCallback) {
		Array<ComPtr<ID3D11ShaderResourceView>> temp(resources.size());
		for (Size i = 0; i < temp.size(); i++) {
			temp[i] = dynamic_cast<DX11Texture2D*>(resources[i])->GetD3D11ShaderResourceView(m_context->GetD3D11Device());
		}
		
		Array<ID3D11ShaderResourceView*> textures(resources.size());
		for (Size i = 0; i < textures.size(); i++) {
			textures[i] = temp[i].Get();
		}
		bindCallback(m_context->GetD3D11Context(), textures);
	}

	void DX11AbstractStage::BindBuffersWithCallback(const Array<IBufferResourceData*>& resources, std::function<void(ComPtr<ID3D11DeviceContext>, const Array<ID3D11Buffer*>&)> bindCallback) {
		Array<ID3D11Buffer*> buffers(resources.size());
		for (Size i = 0; i < buffers.size(); i++) {
			buffers[i] = dynamic_cast<DX11Buffer*>(resources[i])->GetD3D11Buffer().Get();
		}
		bindCallback(m_context->GetD3D11Context(), buffers);
	}

	DX11StageVS::DX11StageVS(DX11Context* context)
		: DX11AbstractStage(context) {

	}

	void DX11StageVS::BindTextures(const Array<ITextureResourceData*>& resources) {
		BindTexturesWithCallback(resources, [&](ComPtr<ID3D11DeviceContext> d3dContext, const Array<ID3D11ShaderResourceView*>& resources) {
			d3dContext->VSSetShaderResources(0, static_cast<UINT>(resources.size()), resources.data());
		});
	}

	void DX11StageVS::BindBuffers(const Array<IBufferResourceData*>& resources) {
		BindBuffersWithCallback(resources, [&](ComPtr<ID3D11DeviceContext> d3dContext, const Array<ID3D11Buffer*>& buffers) {
			d3dContext->VSSetConstantBuffers(0, static_cast<UINT>(buffers.size()), buffers.data()); 
		});
	}

	void DX11StageVS::BindShader(IShaderResourceData* resource) {
		ComPtr<ID3D11DeviceContext> d3dContext = m_context->GetD3D11Context();
		DX11VertexShader* shader = dynamic_cast<DX11VertexShader*>(resource);

		d3dContext->IASetInputLayout(shader->GetD3D11Layout().Get());
		d3dContext->VSSetShader(shader->GetD3D11Shader().Get(), nullptr, 0);
				
	}

	DX11StagePS::DX11StagePS(DX11Context* context) 
		: DX11AbstractStage(context) {

	}

	void DX11StagePS::BindTextures(const Array<ITextureResourceData*>& resources) {
		BindTexturesWithCallback(resources, [&](ComPtr<ID3D11DeviceContext> d3dContext, const Array<ID3D11ShaderResourceView*>& resources) {
			d3dContext->PSSetShaderResources(0, static_cast<UINT>(resources.size()), resources.data());
		});
	}

	void DX11StagePS::BindBuffers(const Array<IBufferResourceData*>& resources) {
		BindBuffersWithCallback(resources, [&](ComPtr<ID3D11DeviceContext> d3dContext, const Array<ID3D11Buffer*>& buffers) {
			d3dContext->PSSetConstantBuffers(0, static_cast<UINT>(buffers.size()), buffers.data());
		});
	}

	void DX11StagePS::BindShader(IShaderResourceData* resource) {
		ComPtr<ID3D11DeviceContext> d3dContext = m_context->GetD3D11Context();
		DX11PixelShader* shader = dynamic_cast<DX11PixelShader*>(resource);

		d3dContext->PSSetShader(shader->GetD3D11Shader().Get(), nullptr, 0);
	}
}