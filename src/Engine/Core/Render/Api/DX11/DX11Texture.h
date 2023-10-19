#ifndef DX11TEXTURE_H
#define DX11TEXTURE_H

#include "Engine/Core/Render/Api/DX11/DX11Def.h"

#include "Engine/Core/Render/Api/Interface/ITextureResource.h"
#include "Engine/Core/Render/Api/Interface/ITargetResource.h"

namespace Engine {
    struct DX11Texture2DDescription {
    public:
        DX11Texture2DDescription(Int32 width, Int32 height, TextureFormat format);

        void AddD3D11BindFlags(D3D11_BIND_FLAG d3dBindFlags);
        void AddD3D11CubemapPreset();

        void RemoveD3D11BindFlags(D3D11_BIND_FLAG d3dBindFlags);
        void RemoveD3D11CubemapPreset();

        const D3D11_TEXTURE2D_DESC* GetD3D11Texture2DDesc() const;

    private:
        D3D11_TEXTURE2D_DESC m_d3dTexture2DDesc;
    };

    struct DX11Texture2DSubresourceData {
    public:
        DX11Texture2DSubresourceData(Int32 stride, TextureFormat format, Int8* data);
        DX11Texture2DSubresourceData(Int32 stride, TextureFormat format, Array<Int8*> data);

        const D3D11_SUBRESOURCE_DATA* GetD3D11SubresourceData() const;

    private:
        Array<D3D11_SUBRESOURCE_DATA> m_d3dSubresourceData;
    };

    class DX11TextureCommonResourceData : public ITextureResourceData {
    public:
        DX11TextureCommonResourceData();
        virtual ~DX11TextureCommonResourceData() = default;

        TextureFormat GetFormat() const override;
        Int32 GetWidth() const override;
        Int32 GetHeight() const override;

        bool IsCubemap() const override;

        ComPtr<ID3D11Texture2D> GetD3D11Texture2D() const;

    protected:
        void CreateD3D11Texture2D(ComPtr<ID3D11Texture2D> d3dTexture);
        void CreateD3D11Texture2D(ComPtr<ID3D11Device> d3dDevice, const DX11Texture2DDescription* texture2DDesc);
        void CreateD3D11Texture2D(ComPtr<ID3D11Device> d3dDevice, const DX11Texture2DDescription* texture2DDesc, const DX11Texture2DSubresourceData* data);

    private:
        ComPtr<ID3D11Texture2D> m_d3dTexture2D;
    };

    class DX11Texture2DResourceData : public DX11TextureCommonResourceData {
    public:
        DX11Texture2DResourceData(ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height, Int8* data);
        DX11Texture2DResourceData(ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data);
        virtual ~DX11Texture2DResourceData() = default;
    };

    class DX11TextureRenderResourceData : public DX11TextureCommonResourceData {
    public:
        DX11TextureRenderResourceData(ComPtr<ID3D11Device> d3dDevice, ComPtr<ID3D11Texture2D> d3dTexture);
        DX11TextureRenderResourceData(ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height, bool isCubemap);

    private:
        void CreateD3D11RenderTargetViews(ComPtr<ID3D11Device> d3dDevice, TextureFormat format);

        Array<ComPtr<ID3D11RenderTargetView>> m_d3dViews;
    };

    class DX11DepthStencilResourceData : public DX11TextureCommonResourceData {
    public:
        DX11DepthStencilResourceData(ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height, bool isCubemap);
        virtual ~DX11DepthStencilResourceData() = default;

        void SetDepthClearingEnable(bool enable);
        void SetStencilClearingEnable(bool enable);

    private:
        void CreateD3D11DepthStencilViews(ComPtr<ID3D11Device> d3dDevice, TextureFormat format);

        UINT clearFlag;
        Array<ComPtr<ID3D11DepthStencilView>> m_d3dViews;
    };
}

#endif // DX11TEXTURE_H
