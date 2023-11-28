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

    DX11Texture2DDescription::DX11Texture2DDescription(Int32 width, Int32 height, TextureFormat format) {
        m_d3dTexture2DDesc.Width = static_cast<UINT>(width);
        m_d3dTexture2DDesc.Height = static_cast<UINT>(height);
        m_d3dTexture2DDesc.Format = GetD3D11Format(format);
        m_d3dTexture2DDesc.MipLevels = 1;
        m_d3dTexture2DDesc.SampleDesc.Count = 1;
        m_d3dTexture2DDesc.SampleDesc.Quality = 0;
        m_d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
        m_d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        m_d3dTexture2DDesc.CPUAccessFlags = 0;
        m_d3dTexture2DDesc.ArraySize = 1;
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
        m_d3dTexture2DDesc.ArraySize = 1;
        m_d3dTexture2DDesc.MiscFlags = 1;
    }

    const D3D11_TEXTURE2D_DESC* DX11Texture2DDescription::GetD3D11Texture2DDesc() const {
        return &m_d3dTexture2DDesc;
    }

    DX11Texture2DSubresourceData::DX11Texture2DSubresourceData(Int32 stride, TextureFormat format, Int8* data) 
        : m_d3dSubresourceData(1) {
        m_d3dSubresourceData[0].pSysMem = data;
        m_d3dSubresourceData[0].SysMemPitch = static_cast<UINT>(stride) * GetD3D11FormatSizeOf(GetD3D11Format(format));
        m_d3dSubresourceData[0].SysMemSlicePitch = 0;
    }

    DX11Texture2DSubresourceData::DX11Texture2DSubresourceData(Int32 stride, TextureFormat format, Array<Int8*> data) 
        : m_d3dSubresourceData(data.size()) {
        for (Size i = 0; i < m_d3dSubresourceData.size(); i++) {
            m_d3dSubresourceData[i].pSysMem = data[i];
            m_d3dSubresourceData[i].SysMemPitch = static_cast<UINT>(stride) * GetD3D11FormatSizeOf(GetD3D11Format(format));
            m_d3dSubresourceData[i].SysMemSlicePitch = 0;
        }
    }

    const D3D11_SUBRESOURCE_DATA* DX11Texture2DSubresourceData::GetD3D11SubresourceData() const {
        return m_d3dSubresourceData.data();
    }

    DX11Texture2D::DX11Texture2D(ComPtr<ID3D11Texture2D> d3dTexture) 
        : m_d3dTexture2D(d3dTexture) {

    }

    DX11Texture2D::DX11Texture2D(ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height, bool isCubemap) {
        DX11Texture2DDescription textureDesc(width, height, format);

        textureDesc.AddD3D11BindFlags(D3D11_BIND_RENDER_TARGET);
        if (isCubemap) {
            textureDesc.AddD3D11CubemapPreset();
        }

        CreateD3D11Texture2D(d3dDevice, &textureDesc);
    }

    DX11Texture2D::DX11Texture2D(ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height, Int8* data) {
        DX11Texture2DDescription textureDesc(width, height, format);
        DX11Texture2DSubresourceData textureSubresData(width, format, data);
        CreateD3D11Texture2D(d3dDevice, &textureDesc, &textureSubresData);
    }

    DX11Texture2D::DX11Texture2D(ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) {
        DX11Texture2DDescription textureDesc(width, height, format);
        DX11Texture2DSubresourceData textureSubresData(width, format, data);

        if (data.size() >= 6) {
            textureDesc.AddD3D11CubemapPreset();
        }

        CreateD3D11Texture2D(d3dDevice, &textureDesc, &textureSubresData);
    }

    TextureFormat DX11Texture2D::GetFormat() const {
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
        m_d3dTexture2D->GetDesc(&d3dTexture2DDesc);
        return ToTextureFormat(d3dTexture2DDesc.Format);
    }

    Int32 DX11Texture2D::GetWidth() const {
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
        m_d3dTexture2D->GetDesc(&d3dTexture2DDesc);
        return static_cast<Int32>(d3dTexture2DDesc.Width);
    }

    Int32 DX11Texture2D::GetHeight() const {
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
        m_d3dTexture2D->GetDesc(&d3dTexture2DDesc);
        return static_cast<Int32>(d3dTexture2DDesc.Height);
    }

    bool DX11Texture2D::IsCubemap() const {
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
        m_d3dTexture2D->GetDesc(&d3dTexture2DDesc);
        return d3dTexture2DDesc.MiscFlags == D3D11_RESOURCE_MISC_TEXTURECUBE;
    }

    ComPtr<ID3D11Texture2D> DX11Texture2D::GetD3D11Texture2D() const {
        return m_d3dTexture2D;
    }

    ComPtr<ID3D11ShaderResourceView> DX11Texture2D::GetD3D11ShaderResourceView(ComPtr<ID3D11Device> d3dDevice) {
        D3D11_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc;
        ZeroMemory(&d3dShaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

        TextureFormat format = GetFormat();
        if (format == TextureFormat::TF_R24G8_BMP || format == TextureFormat::TF_R24_BMP_G8_UINT) {
            d3dShaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        } else {
            d3dShaderResourceViewDesc.Format = GetD3D11Format(format);
        }

        if (IsCubemap()) {
            d3dShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
            d3dShaderResourceViewDesc.TextureCube.MipLevels = 1;
        } else {
            d3dShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            d3dShaderResourceViewDesc.Texture2D.MipLevels = 1;
        }

        ComPtr<ID3D11ShaderResourceView> d3dShaderResourceView;

        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateShaderResourceView(m_d3dTexture2D.Get(), &d3dShaderResourceViewDesc, &d3dShaderResourceView))) {
            throw EngineException("[DX11Texture2D] ID3D11Device::CreateShaderResourceView() failed");
        }
        return d3dShaderResourceView;
    }

    void DX11Texture2D::CreateD3D11Texture2D(ComPtr<ID3D11Device> d3dDevice, const DX11Texture2DDescription* texture2DDesc) {
        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateTexture2D(texture2DDesc->GetD3D11Texture2DDesc(), nullptr, &m_d3dTexture2D))) {
            throw EngineException("[DX11Texture2D] ID3D11Device::CreateTexture2D() failed");
        }
    }

    void DX11Texture2D::CreateD3D11Texture2D(ComPtr<ID3D11Device> d3dDevice, const DX11Texture2DDescription* texture2DDesc, const DX11Texture2DSubresourceData* data) {
        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateTexture2D(texture2DDesc->GetD3D11Texture2DDesc(), data->GetD3D11SubresourceData(), &m_d3dTexture2D))) {
            throw EngineException("[DX11Texture2D] ID3D11Device::CreateTexture2D() failed");
        }
    }
}
