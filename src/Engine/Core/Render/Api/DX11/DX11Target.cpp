#include "Engine/Core/Render/Api/DX11/DX11Target.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    DX11RenderTarget::DX11RenderTarget(ComPtr<ID3D11Device> d3dDevice, DX11Texture2D* texture, Float color[4])
        : m_data(texture), m_viewId(0) {
        memcpy_s(m_clearColor, 4, color, 4);

        D3D11_RENDER_TARGET_VIEW_DESC  d3dRenderTargetViewDesc;
        ZeroMemory(&d3dRenderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
        d3dRenderTargetViewDesc.Format = GetD3D11Format(AdjustTextureFormatForTarget(texture->GetFormat()));

        if (texture->IsCubemap()) {
            m_d3dViews.resize(6);

            d3dRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
            d3dRenderTargetViewDesc.Texture2DArray.MipSlice = 0;
            d3dRenderTargetViewDesc.Texture2DArray.ArraySize = 1;
            d3dRenderTargetViewDesc.Texture2DArray.FirstArraySlice = 0;
        } else {
            m_d3dViews.resize(1);

            d3dRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            d3dRenderTargetViewDesc.Texture2D.MipSlice = 0;
        }

        for (Size i = 0; i < m_d3dViews.size(); i++) {
            if (m_d3dViews.size() > 1) {
                d3dRenderTargetViewDesc.Texture2DArray.FirstArraySlice = i;
            }

            HRESULT hr = 0;
            if (FAILED(hr = d3dDevice->CreateRenderTargetView(texture->GetD3D11Texture2D().Get(), &d3dRenderTargetViewDesc, &m_d3dViews[i]))) {
                throw EngineException("[DX11RenderTarget] ID3D11Device::CreateRenderTargetView() failed");
            }
        }
    }

    DX11RenderTarget::~DX11RenderTarget() {
        DELETE_OBJECT(m_data);
    }

    bool DX11RenderTarget::IsDepth() const {
        return false;
    }

    void DX11RenderTarget::Clear(IContext* context) {
        ComPtr<ID3D11DeviceContext> d3dContext = dynamic_cast<DX11Context*>(context)->GetD3D11Context();
        d3dContext->ClearRenderTargetView(m_d3dViews[m_viewId].Get(), m_clearColor);
    }

    ITextureResourceData* DX11RenderTarget::GetTextureResource() const {
        return m_data;
    }

    ComPtr<ID3D11RenderTargetView> DX11RenderTarget::GetD3D11RenderTargetView() const {
        return m_d3dViews[0];
    }

    DX11DepthStencil::DX11DepthStencil(ComPtr<ID3D11Device> d3dDevice, DX11Texture2D* texture, Float depth, UInt32 stencil) 
        : m_data(texture), m_clearDepth(depth), m_clearStencil(stencil), m_clearFlags(D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL ), m_viewId(0) {
        D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
        ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
        d3dDepthStencilViewDesc.Format = GetD3D11Format(AdjustTextureFormatForTarget(texture->GetFormat()));

        if (texture->IsCubemap()) {
            m_d3dViews.resize(6);

            d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
            d3dDepthStencilViewDesc.Texture2DArray.MipSlice = 0;
            d3dDepthStencilViewDesc.Texture2DArray.ArraySize = 1;
            d3dDepthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;
        } else {
            m_d3dViews.resize(1);

            d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;
        }

        for (Size i = 0; i < m_d3dViews.size(); i++) {
            if (m_d3dViews.size() > 1) {
                d3dDepthStencilViewDesc.Texture2DArray.FirstArraySlice = i;
            }

            HRESULT hr = 0;
            if (FAILED(hr = d3dDevice->CreateDepthStencilView(texture->GetD3D11Texture2D().Get(), &d3dDepthStencilViewDesc, &m_d3dViews[i]))) {
                throw EngineException("[DX11DepthStencil] ID3D11Device::CreateDepthStencilView() failed");
            }
        }
    }

    DX11DepthStencil::~DX11DepthStencil() {
        DELETE_OBJECT(m_data);
    }

    bool DX11DepthStencil::IsDepth() const {
        return true;
    }

    void DX11DepthStencil::Clear(IContext* context) {
        ComPtr<ID3D11DeviceContext> d3dContext = dynamic_cast<DX11Context*>(context)->GetD3D11Context();
        d3dContext->ClearDepthStencilView(m_d3dViews[m_viewId].Get(), m_clearFlags, m_clearDepth, m_clearStencil);
    }

    ITextureResourceData* DX11DepthStencil::GetTextureResource() const {
        return m_data;
    }

    ComPtr<ID3D11DepthStencilView> DX11DepthStencil::GetD3D11DepthStencilView() const {
        return m_d3dViews[0];
    }

    void DX11DepthStencil::EnableDepthClear(bool enable) {
        m_clearFlags = enable ? m_clearFlags | D3D11_CLEAR_DEPTH : m_clearFlags & ~D3D11_CLEAR_DEPTH;
    }

    void DX11DepthStencil::EnableStencilClear(bool enable) {
        m_clearFlags = enable ? m_clearFlags | D3D11_CLEAR_STENCIL : m_clearFlags & ~D3D11_CLEAR_STENCIL;
    }
}
