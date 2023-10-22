#include "Engine/Core/Render/Api/DX11/DX11Stage.h"
#include "Engine/Core/Render/Api/DX11/DX11Buffer.h"
#include "Engine/Core/Render/Api/DX11/DX11Texture.h"

namespace Engine {
	DX11AbstractStage::DX11AbstractStage(DX11Context* context)
		: m_context(context) {

	}

	void DX11AbstractStage::BindTextures(const Array<ITextureResourceData*>& resources, std::function<void(ComPtr<ID3D11DeviceContext>, const Array<ID3D11ShaderResourceView*>&)> bindCallback) {
		Array<ID3D11ShaderResourceView*> textures(resources.size());
		for (Size i = 0; i < textures.size(); i++) {
			textures[i] = dynamic_cast<DX11TextureCommonResourceData*>(resources[i])->GetD3D11ShaderResourceView(m_context->GetD3D11Device());
		}
		bindCallback(m_context->GetD3D11Context(), textures);
	}

	void DX11AbstractStage::BindBuffers(const Array<IBufferResourceData*>& resources, std::function<void(ComPtr<ID3D11DeviceContext>, const Array<ID3D11Buffer*>&)> bindCallback) {
		Array<ID3D11Buffer*> buffers(resources.size());
		for (Size i = 0; i < buffers.size(); i++) {
			buffers[i] = dynamic_cast<DX11Buffer*>(resources[i])->GetD3D11Buffer();
		}
		bindCallback(m_context->GetD3D11Context(), buffers);
	}

	DX11StageVS::DX11StageVS(DX11Context* context)
		: DX11AbstractStage(context) {

	}

	void DX11StageVS::BindTextures(const Array<ITextureResourceData*>& resources) {
		BindTextures(resources, [&](ComPtr<ID3D11DeviceContext> d3dContext, const Array<ID3D11ShaderResourceView*>& resources) {
			d3dContext->VSSetShaderResources(0, static_cast<UINT>(resources.size()), resources.data());
		});
	}

	void DX11StageVS::BindBuffers(const Array<IBufferResourceData*>& resources) {
		BindBuffers(resources, [&](ComPtr<ID3D11DeviceContext> d3dContext, const Array<ID3D11Buffer*>& buffers) {
			d3dContext->VSSetConstantBuffers(0, static_cast<UINT>(buffers.size()), buffers.data()); 
		});
	}

	DX11StagePS::DX11StagePS(DX11Context* context) 
		: DX11AbstractStage(context) {

	}

	void DX11StagePS::BindTextures(const Array<ITextureResourceData*>& resources) {
		BindTextures(resources, [&](ComPtr<ID3D11DeviceContext> d3dContext, const Array<ID3D11ShaderResourceView*>& resources) {
			d3dContext->PSSetShaderResources(0, static_cast<UINT>(resources.size()), resources.data());
		});
	}

	void DX11StagePS::BindBuffers(const Array<IBufferResourceData*>& resources) {
		BindBuffers(resources, [&](ComPtr<ID3D11DeviceContext> d3dContext, const Array<ID3D11Buffer*>& buffers) {
			d3dContext->PSSetConstantBuffers(0, static_cast<UINT>(buffers.size()), buffers.data());
		});
	}
}