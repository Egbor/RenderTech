#include "Engine/Core/Render/Api/DX11/DX11Texture.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    DXGI_FORMAT gFormatTable[] = {
        DXGI_FORMAT_R8_UNORM,
        DXGI_FORMAT_R8G8_UNORM,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT_B8G8R8A8_UNORM,
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
        DXGI_FORMAT_R16_SINT,
        DXGI_FORMAT_R32_SINT,
        DXGI_FORMAT_R16_UINT,
        DXGI_FORMAT_R32_UINT,
        DXGI_FORMAT_R32_FLOAT,
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        DXGI_FORMAT_R24G8_TYPELESS
    };

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

    constexpr static Int32 ToByteWidth(DXGI_FORMAT format) {
        switch (format) {
        case DXGI_FORMAT_R32G32B32A32_FLOAT:
            return 16;
        case DXGI_FORMAT_R32_SINT:
        case DXGI_FORMAT_R32_UINT:
        case DXGI_FORMAT_R32_FLOAT:
        case DXGI_FORMAT_R24G8_TYPELESS:
        case DXGI_FORMAT_R8G8B8A8_UNORM:
        case DXGI_FORMAT_B8G8R8A8_UNORM:
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
            return 4;
        case DXGI_FORMAT_R16_SINT:
        case DXGI_FORMAT_R16_UINT:
        case DXGI_FORMAT_R8G8_UNORM:
            return 2;
        case DXGI_FORMAT_R8_UNORM:
            return 1;
        default:
            break;
        }
        return 0;
    }

    constexpr static void InitializeD3D11Texture2DDescByDefault(D3D11_TEXTURE2D_DESC& desc) {
        desc.Width = 0;
        desc.Height = 0;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = 0;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
    }

    DXGI_FORMAT DX11Texture2DData::GetD3D11Format() const {
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
        m_d3dTexture2D->GetDesc(&d3dTexture2DDesc);
        return d3dTexture2DDesc.Format;
    }

    UINT DX11Texture2DData::GetWidth() const {
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
        m_d3dTexture2D->GetDesc(&d3dTexture2DDesc);
        return d3dTexture2DDesc.Width;
    }

    UINT DX11Texture2DData::GetHeight() const {
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
        m_d3dTexture2D->GetDesc(&d3dTexture2DDesc);
        return d3dTexture2DDesc.Height;
    }

    void DX11Texture2DData::ReadBits(ComPtr<ID3D11Device> d3dDevice,
                                     ComPtr<ID3D11DeviceContext> d3dContext,
                                     UInt32 resourceId, Int8* bits, Size size) const {
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
        InitializeD3D11Texture2DDescByDefault(d3dTexture2DDesc);
        d3dTexture2DDesc.Width = GetWidth();
        d3dTexture2DDesc.Height = GetHeight();
        d3dTexture2DDesc.Format = GetD3D11Format();
        d3dTexture2DDesc.Usage = D3D11_USAGE_STAGING;
        d3dTexture2DDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

        ComPtr<ID3D11Texture2D> temp = CreateD3D11Texture2D(d3dDevice, &d3dTexture2DDesc, nullptr);
        d3dContext->CopySubresourceRegion(temp.Get(), 0, 0, 0, 0, m_d3dTexture2D.Get(), resourceId, nullptr);

        D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
        d3dContext->Map(temp.Get(), 0, D3D11_MAP_READ, 0, &d3dMappedResource);
        memcpy(bits, d3dMappedResource.pData, size);
    }

    ComPtr<ID3D11Texture2D> DX11Texture2DData::GetD3D11Texture2D() const {
        return m_d3dTexture2D;
    }

    void DX11Texture2DData::Initialize(ComPtr<ID3D11Texture2D> d3dTexture) {
        m_d3dTexture2D = d3dTexture;
    }

    void DX11Texture2DData::Initialize(ComPtr<ID3D11Device> d3dDevice,
                                       D3D11_TEXTURE2D_DESC* d3dTexture2DDesc,
                                       D3D11_SUBRESOURCE_DATA* d3dSubresourceData) {
        Initialize(CreateD3D11Texture2D(d3dDevice, d3dTexture2DDesc, d3dSubresourceData));
    }

    ComPtr<ID3D11Texture2D> DX11Texture2DData::CreateD3D11Texture2D(ComPtr<ID3D11Device> d3dDevice,
                                                                    D3D11_TEXTURE2D_DESC* d3dTexture2DDesc,
                                                                    D3D11_SUBRESOURCE_DATA* d3dSubresourceData) {
        ComPtr<ID3D11Texture2D> d3dTexture2D;

        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateTexture2D(d3dTexture2DDesc, d3dSubresourceData, &d3dTexture2D))) {
            throw EngineException("[DX11Texture2DData] ID3DDevice::CreateTexture2D() failed");
        }
        return d3dTexture2D;
    }

    GENERATE_RTTI_DEFINITIONS(DX11Texture2D)

    DX11Texture2D::DX11Texture2D(const ObjectArgument& argument)
        : Super(argument), m_dxTexture2DWrapper() {

    }

    Int32 DX11Texture2D::GetWidth() const {
        return static_cast<Int32>(m_dxTexture2DWrapper.GetWidth());
    }

    Int32 DX11Texture2D::GetHeight() const {
        return static_cast<Int32>(m_dxTexture2DWrapper.GetHeight());
    }

    DX11Texture2DData& DX11Texture2D::Data() {
        return m_dxTexture2DWrapper;
    }

    TextureFormat DX11Texture2D::GetFormat() const {
        return ToTextureFormat(m_dxTexture2DWrapper.GetD3D11Format());
    }

    void DX11Texture2D::GetD3D11ResourceDesc(D3D11_SHADER_RESOURCE_VIEW_DESC& desc) {
        ZeroMemory(&desc, sizeof(desc));
        desc.Format = Data().GetD3D11Format();
        desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipLevels = 1;
    }

    void DX11Texture2D::Create(DX11Context* context, TextureInfo info) {
        throw EngineException("[DX11Texture2D] DX11Texture2D::Create() is abstaract method without any features");
    }

    GENERATE_RTTI_DEFINITIONS(DX11CubeTexture2D)

    DX11CubeTexture2D::DX11CubeTexture2D(const ObjectArgument& argument)
        : Super(argument), m_dxTexture2DWrapper() {

    }

    TextureFormat DX11CubeTexture2D::GetFormat() const {
        return ToTextureFormat(m_dxTexture2DWrapper.GetD3D11Format());
    }

    Int32 DX11CubeTexture2D::GetWidth() const {
        return static_cast<Int32>(m_dxTexture2DWrapper.GetWidth());
    }

    Int32 DX11CubeTexture2D::GetHeight() const {
        return static_cast<Int32>(m_dxTexture2DWrapper.GetHeight());
    }

    DX11Texture2DData& DX11CubeTexture2D::Data() {
        return m_dxTexture2DWrapper;
    }

    void DX11CubeTexture2D::GetD3D11ResourceDesc(D3D11_SHADER_RESOURCE_VIEW_DESC& desc) {
        ZeroMemory(&desc, sizeof(desc));
        desc.Format = Data().GetD3D11Format();
        desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
        desc.TextureCube.MipLevels = 1;
    }

    void DX11CubeTexture2D::Create(DX11Context* context, TextureInfo info) {
        throw EngineException("[DX11Texture2D] DX11Texture2D::Create() is abstaract method without any features");
    }

    GENERATE_RTTI_DEFINITIONS(DX11ResourceTexture2D)

    DX11ResourceTexture2D::DX11ResourceTexture2D(const ObjectArgument& argument)
        : Super(argument) {

    }

    String DX11ResourceTexture2D::GetTag() const {
        return tagTexture;
    }

    void DX11ResourceTexture2D::Create(DX11Context* context, TextureInfo desc) {
        ComPtr<ID3D11Device> d3dDevice = context->GetD3D11Device();
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
        D3D11_SUBRESOURCE_DATA d3dSubresourceData;

        InitializeD3D11Texture2DDescByDefault(d3dTexture2DDesc);
        d3dTexture2DDesc.Width = static_cast<UINT>(desc.width);
        d3dTexture2DDesc.Height = static_cast<UINT>(desc.height);
        d3dTexture2DDesc.Format = gFormatTable[static_cast<Int32>(desc.format)];
        d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        d3dSubresourceData.pSysMem = desc.data[0];
        d3dSubresourceData.SysMemPitch = desc.width * ToByteWidth(d3dTexture2DDesc.Format);
        d3dSubresourceData.SysMemSlicePitch = 0;

        Data().Initialize(d3dDevice, &d3dTexture2DDesc, &d3dSubresourceData);
    }

    GENERATE_RTTI_DEFINITIONS(DX11ResourceCubeTexture2D)

    DX11ResourceCubeTexture2D::DX11ResourceCubeTexture2D(const ObjectArgument& argument)
        : Super(argument) {

    }

    String DX11ResourceCubeTexture2D::GetTag() const {
        return tagCubeTexture;
    }

    void DX11ResourceCubeTexture2D::Create(DX11Context* context, TextureInfo desc) {
        ComPtr<ID3D11Device> d3dDevice = context->GetD3D11Device();
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
        D3D11_SUBRESOURCE_DATA d3dSubresourceData[6];

        InitializeD3D11Texture2DDescByDefault(d3dTexture2DDesc);
        d3dTexture2DDesc.Width = static_cast<UINT>(desc.width);
        d3dTexture2DDesc.Height = static_cast<UINT>(desc.height);
        d3dTexture2DDesc.ArraySize = 6;
        d3dTexture2DDesc.Format = gFormatTable[static_cast<Int32>(desc.format)];
        d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        d3dTexture2DDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

        for (Int32 i = 0; i < 6; i++) {
            d3dSubresourceData[i].pSysMem = desc.data[i];
            d3dSubresourceData[i].SysMemPitch = desc.width * ToByteWidth(d3dTexture2DDesc.Format);
            d3dSubresourceData[i].SysMemSlicePitch = 0;
        }

        Data().Initialize(d3dDevice, &d3dTexture2DDesc, d3dSubresourceData);
    }

    GENERATE_RTTI_DEFINITIONS(DX11OutputTexture2D)

    DX11OutputTexture2D::DX11OutputTexture2D(const ObjectArgument& argument)
        : Super(argument) {

    }

    String DX11OutputTexture2D::GetTag() const {
        return tagTargetTexture;
    }

    void DX11OutputTexture2D::Create(DX11Context* context, TextureInfo desc) {
        ComPtr<ID3D11Device> d3dDevice = context->GetD3D11Device();
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;

        InitializeD3D11Texture2DDescByDefault(d3dTexture2DDesc);
        d3dTexture2DDesc.Width = static_cast<UINT>(desc.width);
        d3dTexture2DDesc.Height = static_cast<UINT>(desc.height);
        d3dTexture2DDesc.Format = gFormatTable[static_cast<Int32>(desc.format)];
        d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

        Data().Initialize(d3dDevice, &d3dTexture2DDesc, NULL);
    }

    GENERATE_RTTI_DEFINITIONS(DX11OutputDepthStencilTexture2D)

    DX11OutputDepthStencilTexture2D::DX11OutputDepthStencilTexture2D(const ObjectArgument& argument)
        : Super(argument) {

    }

    String DX11OutputDepthStencilTexture2D::GetTag() const {
        return tagDepthStencilTargetTexture;
    }

    void DX11OutputDepthStencilTexture2D::Create(DX11Context* context, TextureInfo desc) {
        ComPtr<ID3D11Device> d3dDevice = context->GetD3D11Device();
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;

        InitializeD3D11Texture2DDescByDefault(d3dTexture2DDesc);
        d3dTexture2DDesc.Width = static_cast<UINT>(desc.width);
        d3dTexture2DDesc.Height = static_cast<UINT>(desc.height);
        d3dTexture2DDesc.Format = gFormatTable[static_cast<Int32>(desc.format)];
        d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;

        Data().Initialize(d3dDevice, &d3dTexture2DDesc, NULL);
    }

    void DX11OutputDepthStencilTexture2D::GetD3D11ResourceDesc(D3D11_SHADER_RESOURCE_VIEW_DESC& desc) {
        Super::GetD3D11ResourceDesc(desc);
        desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    }

    GENERATE_RTTI_DEFINITIONS(DX11OutputCubeTexture2D)

    DX11OutputCubeTexture2D::DX11OutputCubeTexture2D(const ObjectArgument& argument)
        : Super(argument) {

    }

    void DX11OutputCubeTexture2D::ReadBits(DX11Context* context, Int8* bits, Size size) {
        UInt32 resourceId = D3D11CalcSubresource(0, GetFaceId(), 1);
        Data().ReadBits(context->GetD3D11Device(), context->GetD3D11Context(), resourceId, bits, size);
    }

    String DX11OutputCubeTexture2D::GetTag() const {
        return tagCubeMapTargetTexture;
    }

    void DX11OutputCubeTexture2D::Create(DX11Context* context, TextureInfo desc) {
        ComPtr<ID3D11Device> d3dDevice = context->GetD3D11Device();
        D3D11_TEXTURE2D_DESC d3dTexture2DDesc;

        InitializeD3D11Texture2DDescByDefault(d3dTexture2DDesc);
        d3dTexture2DDesc.Width = static_cast<UINT>(desc.width);
        d3dTexture2DDesc.Height = static_cast<UINT>(desc.height);
        d3dTexture2DDesc.ArraySize = 6;
        d3dTexture2DDesc.Format = gFormatTable[static_cast<Int32>(desc.format)];
        d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        d3dTexture2DDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

        Data().Initialize(d3dDevice, &d3dTexture2DDesc, NULL);
    }
}
