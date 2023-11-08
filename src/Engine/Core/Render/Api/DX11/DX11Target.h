#ifndef DX11TARGET_H
#define DX11TARGET_H

#include "Engine/Core/Render/Api/Interface/ITargetResource.h"
#include "Engine/Core/Render/Api/DX11/DX11Texture.h"

namespace Engine {
    class DX11RenderTarget : public ITargetResourceData {
    public:
        DX11RenderTarget(ComPtr<ID3D11Device> d3dDevice, DX11Texture2D* texture, Float color[4]);
        virtual ~DX11RenderTarget();

        bool IsDepth() const override;
        void Clear(IContext* context) override;

        ITextureResourceData* GetTextureResource() const override;

        ComPtr<ID3D11RenderTargetView> GetD3D11RenderTargetView() const;

    private:
        Array<ComPtr<ID3D11RenderTargetView>> m_d3dViews;
        DX11Texture2D* m_data;

        Float m_clearColor[4];
        UInt32 m_viewId;
    };

    class DX11DepthStencil : public ITargetResourceData {
    public:
        DX11DepthStencil(ComPtr<ID3D11Device> d3dDevice, DX11Texture2D* texture, Float depth, UInt32 stencil);
        virtual ~DX11DepthStencil();

        bool IsDepth() const override;
        void Clear(IContext* context) override;

        void EnableDepthClear(bool enable);
        void EnableStencilClear(bool enable);

        ITextureResourceData* GetTextureResource() const override;

        ComPtr<ID3D11DepthStencilView> GetD3D11DepthStencilView() const;

    private:
        Array<ComPtr<ID3D11DepthStencilView>> m_d3dViews;
        DX11Texture2D* m_data;

        Float m_clearDepth;
        UInt32 m_clearStencil;
        UInt32 m_clearFlags;
        UInt32 m_viewId;
    };
}

#endif // DX11TARGET_H
