#include "Engine/Core/Render/Api/DX11/DX11Texture.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    constexpr static TextureFormat ToTextureFormat(DXGI_FORMAT format) {
        switch (format) {
        case DXGI_FORMAT_R32G32B32A32_FLOAT:
            return TextureFormat::TF_R32G32B32A32_FLOAT;
        case DXGI_FORMAT_R32_SINT:
            return TextureFormat::TF_R32_INT;
        case DXGI_FORMAT_R32_UINT:
            return TextureFormat::TF_R32_UINT;
        case DXGI_FORMAT_R32_FLOAT:
            return TextureFormat::TF_R32_FLOAT;
        case DXGI_FORMAT_R24G8_TYPELESS:
            return TextureFormat::TF_R24G8_BMP;
        case DXGI_FORMAT_R8G8B8A8_UNORM:
            return TextureFormat::TF_R8G8B8A8_BMP;
        case DXGI_FORMAT_B8G8R8A8_UNORM:
            return TextureFormat::TF_B8G8R8A8_BMP;
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
            return TextureFormat::TF_R8G8B8A8_BMP_sRGB;
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
            return TextureFormat::TF_B8G8R8A8_BMP_sRGB;
        case DXGI_FORMAT_R16_SINT:
            return TextureFormat::TF_R16_INT;
        case DXGI_FORMAT_R16_UINT:
            return TextureFormat::TF_R16_UINT;
        case DXGI_FORMAT_R8G8_UNORM:
            return TextureFormat::TF_R8G8_BMP;
        case DXGI_FORMAT_R8_UNORM:
            return TextureFormat::TF_UNKNOWN;
        default:
            break;
        }
        return TextureFormat::TF_UNKNOWN;
    }

    void InitializeDimentionForD3D11RenderTarget2DDesc(D3D11_RTV_DIMENSION d3dDimension, UINT firstSlice, D3D11_RENDER_TARGET_VIEW_DESC* d3dOutRenderTargetViewDesc) {
        switch (d3dDimension) {
        case D3D11_RTV_DIMENSION_TEXTURE2D: {
            d3dOutRenderTargetViewDesc->Texture2D.MipSlice = 0;
            break;
        }
        case D3D11_RTV_DIMENSION_TEXTURE2DARRAY: {
            d3dOutRenderTargetViewDesc->Texture2DArray.MipSlice = 0;
            d3dOutRenderTargetViewDesc->Texture2DArray.ArraySize = 1;
            d3dOutRenderTargetViewDesc->Texture2DArray.FirstArraySlice = firstSlice;
            break;
        }
        }
    }

    void InitializeDimentionForD3D11DepthStencilDesc(D3D11_DSV_DIMENSION d3dDimension, UINT firstSlice, D3D11_DEPTH_STENCIL_VIEW_DESC* d3dOutDepthStencilViewDesc) {
        switch (d3dDimension) {
        case D3D11_DSV_DIMENSION_TEXTURE2D: {
            d3dOutDepthStencilViewDesc->Texture2D.MipSlice = 0;
            break;
        }
        case D3D11_DSV_DIMENSION_TEXTURE2DARRAY: {
            d3dOutDepthStencilViewDesc->Texture2DArray.MipSlice = 0;
            d3dOutDepthStencilViewDesc->Texture2DArray.ArraySize = 1;
            d3dOutDepthStencilViewDesc->Texture2DArray.FirstArraySlice = firstSlice;
            break;
        }
        }
    }

    Map<TextureFormat, DXGI_FORMAT> d3dFormatTable = {
        { TextureFormat::TF_R8_BMP, DXGI_FORMAT_R8_UNORM },
        { TextureFormat::TF_R8G8_BMP, DXGI_FORMAT_R8G8_UNORM },
        { TextureFormat::TF_R8G8B8A8_BMP, DXGI_FORMAT_R8G8B8A8_UNORM },
        { TextureFormat::TF_B8G8R8A8_BMP, DXGI_FORMAT_B8G8R8A8_UNORM },
        { TextureFormat::TF_R8G8B8A8_BMP_sRGB, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB },
        { TextureFormat::TF_B8G8R8A8_BMP_sRGB, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB },
        { TextureFormat::TF_R16_INT, DXGI_FORMAT_R16_SINT },
        { TextureFormat::TF_R32_INT, DXGI_FORMAT_R32_SINT },
        { TextureFormat::TF_R16_UINT, DXGI_FORMAT_R16_UINT },
        { TextureFormat::TF_R32_UINT, DXGI_FORMAT_R32_UINT },
        { TextureFormat::TF_R32_FLOAT, DXGI_FORMAT_R32_FLOAT },
        { TextureFormat::TF_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT },
        { TextureFormat::TF_R24G8_BMP, DXGI_FORMAT_R24G8_TYPELESS },
        { TextureFormat::TF_R24_BMP_G8_UINT, DXGI_FORMAT_D24_UNORM_S8_UINT }
    };

    DX11Texture2DDescription::DX11Texture2DDescription(Int32 width, Int32 height, TextureFormat format) {
        m_d3dTexture2DDesc.Width = static_cast<UINT>(width);
        m_d3dTexture2DDesc.Height = static_cast<UINT>(height);
        m_d3dTexture2DDesc.Format = d3dFormatTable[format];
        m_d3dTexture2DDesc.MipLevels = 1;
        m_d3dTexture2DDesc.SampleDesc.Count = 1;
        m_d3dTexture2DDesc.SampleDesc.Quality = 0;
        m_d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
        m_d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        m_d3dTexture2DDesc.CPUAccessFlags = 0;
        m_d3dTexture2DDesc.ArraySize = 0;
        m_d3dTexture2DDesc.MiscFlags = 1;
    }

    void DX11Texture2DDescription::AddD3D11BindFlags(D3D11_BIND_FLAG d3dBindFlags) {
        m_d3dTexture2DDesc.BindFlags |= d3dBindFlags;
    }

    void DX11Texture2DDescription::AddD3D11CubemapPreset() {
        m_d3dTexture2DDesc.ArraySize = 6;
        m_d3dTexture2DDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    }

    void DX11Texture2DDescription::RemoveD3D11BindFlags(D3D11_BIND_FLAG d3dBindFlags) {
        m_d3dTexture2DDesc.BindFlags &= ~d3dBindFlags;
    }
    
    void DX11Texture2DDescription::RemoveD3D11CubemapPreset() {
        m_d3dTexture2DDesc.ArraySize = 0;
        m_d3dTexture2DDesc.MiscFlags = 1;
    }

    const D3D11_TEXTURE2D_DESC* DX11Texture2DDescription::GetD3D11Texture2DDesc() const {
        return &m_d3dTexture2DDesc;
    }

    DX11Texture2DSubresourceData::DX11Texture2DSubresourceData(Int32 stride, TextureFormat format, Int8* data) 
        : m_d3dSubresourceData(1) {
        m_d3dSubresourceData[0].pSysMem = data;
        m_d3dSubresourceData[0].SysMemPitch = static_cast<UINT>(stride) * GetD3D11FormatSizeOf(d3dFormatTable[format]);
        m_d3dSubresourceData[0].SysMemSlicePitch = 0;
    }

    DX11Texture2DSubresourceData::DX11Texture2DSubresourceData(Int32 stride, TextureFormat format, Array<Int8*> data) 
        : m_d3dSubresourceData(data.size()) {
        for (Size i = 0; i < m_d3dSubresourceData.size(); i++) {
            m_d3dSubresourceData[i].pSysMem = data[i];
            m_d3dSubresourceData[i].SysMemPitch = static_cast<UINT>(stride) * GetD3D11FormatSizeOf(d3dFormatTable[format]);
            m_d3dSubresourceData[i].SysMemSlicePitch = 0;
        }
    }

    const D3D11_SUBRESOURCE_DATA* DX11Texture2DSubresourceData::GetD3D11SubresourceData() const {
        return m_d3dSubresourceData.data();
    }

    DX11TextureCommonResourceData::DX11TextureCommonResourceData() 
        : m_d3dTexture2D(nullptr) {

    }

    TextureFormat DX11TextureCommonResourceData::GetFormat() const {
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
        m_d3dTexture2D->GetDesc(&d3dTexture2DDesc);
        return ToTextureFormat(d3dTexture2DDesc.Format);
    }

    Int32 DX11TextureCommonResourceData::GetWidth() const {
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
        m_d3dTexture2D->GetDesc(&d3dTexture2DDesc);
        return static_cast<Int32>(d3dTexture2DDesc.Width);
    }

    Int32 DX11TextureCommonResourceData::GetHeight() const {
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
        m_d3dTexture2D->GetDesc(&d3dTexture2DDesc);
        return static_cast<Int32>(d3dTexture2DDesc.Height);
    }

    bool DX11TextureCommonResourceData::IsCubemap() const {
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
        m_d3dTexture2D->GetDesc(&d3dTexture2DDesc);
        return d3dTexture2DDesc.MiscFlags == D3D11_RESOURCE_MISC_TEXTURECUBE;
    }

    ComPtr<ID3D11Texture2D> DX11TextureCommonResourceData::GetD3D11Texture2D() const {
        return m_d3dTexture2D;
    }

    void DX11TextureCommonResourceData::CreateD3D11Texture2D(ComPtr<ID3D11Texture2D> d3dTexture) {
        m_d3dTexture2D = d3dTexture;
    }

    void DX11TextureCommonResourceData::CreateD3D11Texture2D(ComPtr<ID3D11Device> d3dDevice, const DX11Texture2DDescription* texture2DDesc) {
        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateTexture2D(texture2DDesc->GetD3D11Texture2DDesc(), nullptr, &m_d3dTexture2D))) {
            throw EngineException("[DX11TextureCommonResourceData] DX11Texture2DResourceData::DX11Texture2DResourceData() failed");
        }
    }

    void DX11TextureCommonResourceData::CreateD3D11Texture2D(ComPtr<ID3D11Device> d3dDevice, const DX11Texture2DDescription* texture2DDesc, const DX11Texture2DSubresourceData* data) {
        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateTexture2D(texture2DDesc->GetD3D11Texture2DDesc(), data->GetD3D11SubresourceData(), &m_d3dTexture2D))) {
            throw EngineException("[DX11TextureCommonResourceData] DX11Texture2DResourceData::DX11Texture2DResourceData() failed");
        }
    }

    DX11Texture2DResourceData::DX11Texture2DResourceData(ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height, Int8* data) {
        DX11Texture2DDescription textureDesc(width, height, format);
        DX11Texture2DSubresourceData textureSubresData(width, format, data);

        CreateD3D11Texture2D(d3dDevice, &textureDesc, &textureSubresData);
    }

    DX11Texture2DResourceData::DX11Texture2DResourceData(ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) {
        DX11Texture2DDescription textureDesc(width, height, format);
        DX11Texture2DSubresourceData textureSubresData(width, format, data);
        textureDesc.AddD3D11CubemapPreset();

        CreateD3D11Texture2D(d3dDevice, &textureDesc, &textureSubresData);
    }

    DX11TextureRenderResourceData::DX11TextureRenderResourceData(ComPtr<ID3D11Device> d3dDevice, ComPtr<ID3D11Texture2D> d3dTexture) {
        CreateD3D11Texture2D(d3dTexture);
        CreateD3D11RenderTargetViews(d3dDevice, GetFormat());
    }

    DX11TextureRenderResourceData::DX11TextureRenderResourceData(ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height, bool isCubemap) {
        DX11Texture2DDescription textureDesc(width, height, format);
        textureDesc.AddD3D11BindFlags(D3D11_BIND_RENDER_TARGET);
        if (isCubemap) {
            textureDesc.AddD3D11CubemapPreset();
        }

        CreateD3D11Texture2D(d3dDevice, &textureDesc);
        CreateD3D11RenderTargetViews(d3dDevice, format);
    }

    void DX11TextureRenderResourceData::CreateD3D11RenderTargetViews(ComPtr<ID3D11Device> d3dDevice, TextureFormat format) {
        D3D11_RENDER_TARGET_VIEW_DESC  d3dRenderTargetViewDesc;
        ZeroMemory(&d3dRenderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
        d3dRenderTargetViewDesc.Format = d3dFormatTable[format];

        if (IsCubemap()) {
            d3dRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
            m_d3dViews.resize(6);
        } else {
            d3dRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            m_d3dViews.resize(1);
        }

        for (Size i = 0; i < m_d3dViews.size(); i++) {
            InitializeDimentionForD3D11RenderTarget2DDesc(d3dRenderTargetViewDesc.ViewDimension, i, &d3dRenderTargetViewDesc);

            HRESULT hr = 0;
            if (FAILED(hr = d3dDevice->CreateRenderTargetView(GetD3D11Texture2D().Get(), &d3dRenderTargetViewDesc, &m_d3dViews[i]))) {
                throw EngineException("[DX11TextureRenderResourceData] ID3D11Device::CreateRenderTargetView() failed");
            }
        }
    }

    DX11DepthStencilResourceData::DX11DepthStencilResourceData(ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height, bool isCubemap) {
        DX11Texture2DDescription textureDesc(width, height, format);
        textureDesc.AddD3D11BindFlags(D3D11_BIND_DEPTH_STENCIL);
        if (isCubemap) {
            textureDesc.AddD3D11CubemapPreset();
        }

        CreateD3D11Texture2D(d3dDevice, &textureDesc);
        CreateD3D11DepthStencilViews(d3dDevice, format);
    }

    void DX11DepthStencilResourceData::CreateD3D11DepthStencilViews(ComPtr<ID3D11Device> d3dDevice, TextureFormat format) {
        D3D11_DEPTH_STENCIL_VIEW_DESC  d3dDepthStencilViewDesc;
        ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
        d3dDepthStencilViewDesc.Format = d3dFormatTable[format];

        if (IsCubemap()) {
            d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
            m_d3dViews.resize(6);
        } else {
            d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            m_d3dViews.resize(1);
        }

        for (Size i = 0; i < m_d3dViews.size(); i++) {
            InitializeDimentionForD3D11DepthStencilDesc(d3dDepthStencilViewDesc.ViewDimension, i, &d3dDepthStencilViewDesc);

            HRESULT hr = 0;
            if (FAILED(hr = d3dDevice->CreateDepthStencilView(GetD3D11Texture2D().Get(), &d3dDepthStencilViewDesc, &m_d3dViews[i]))) {
                throw EngineException("[DX11TextureRenderResourceData] ID3D11Device::CreateRenderTargetView() failed");
            }
        }
    }
}
