#include "Engine/Core/Render/Api/DX11/Binder/DX11TextureBinder.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    DX11TextureBinder::DX11TextureBinder(DX11Context* context)
        : m_dxContext(context), m_dxTextures(RENDER_TEXTURE_COUNT) {

    }

    void DX11TextureBinder::Bind() {
        ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();
        ID3D11ShaderResourceView* d3dTextures[RENDER_TEXTURE_COUNT] = { nullptr };

        for (int i = 0; i < RENDER_TEXTURE_COUNT; i++) {
            d3dTextures[i] = m_dxTextures[i].Get();
        }

        d3dContext->PSSetShaderResources(0, RENDER_TEXTURE_COUNT, d3dTextures);
    }

    void DX11TextureBinder::UnBind() {
        ComPtr<ID3D11DeviceContext> d3dContext = m_dxContext->GetD3D11Context();
        ID3D11ShaderResourceView* d3dTextures[RENDER_TEXTURE_COUNT] = { nullptr };
        d3dContext->PSSetShaderResources(0, RENDER_TEXTURE_COUNT, d3dTextures);
    }

    void DX11TextureBinder::AttachRenderItem(TextureSlot id, Texture2D* item) {
        if (item == nullptr) {
            DetachRenderItem(id);
            return;
        }

        if (m_dxTextures[static_cast<Int32>(id)].Get() != nullptr) {
            DetachRenderItem(id);
        }

        HRESULT hr = 0;
        ComPtr<ID3D11Device> d3dDevice = m_dxContext->GetD3D11Device();
        IDX11Texture2D* dxTexture2D = dynamic_cast<IDX11Texture2D*>(item);
        ID3D11ShaderResourceView** d3dTexture = &m_dxTextures[static_cast<Int32>(id)];

        D3D11_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc;
        dxTexture2D->GetD3D11ResourceDesc(d3dShaderResourceViewDesc);

        if (FAILED(hr = d3dDevice->CreateShaderResourceView(dxTexture2D->Data().GetD3D11Texture2D().Get(),
                                                            &d3dShaderResourceViewDesc, d3dTexture))) {
            throw EngineException("[DX11TextureBinder] DX11TextureBinder::AttachRenderItem() failed");
        }
    }

    void DX11TextureBinder::DetachRenderItem(TextureSlot id) {
        m_dxTextures[static_cast<Int32>(id)] = nullptr;
    }
}
