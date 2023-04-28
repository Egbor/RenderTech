#ifndef DX11TEXTURE_H
#define DX11TEXTURE_H

#include "Engine/Object/Class/Texture.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11Texture2DData {
    private:
        ComPtr<ID3D11Texture2D> m_d3dTexture2D;

    public:
        DX11Texture2DData() = default;
        virtual ~DX11Texture2DData() = default;

        DXGI_FORMAT GetD3D11Format() const;
        UINT GetWidth() const;
        UINT GetHeight() const;

        void ReadBits(ComPtr<ID3D11Device> d3dDevice,
                      ComPtr<ID3D11DeviceContext> d3dContext,
                      UInt32 resourceId, Int8* bits, Size size) const;

        void Initialize(ComPtr<ID3D11Texture2D> d3dTexture);
        void Initialize(ComPtr<ID3D11Device> d3dDevice,
                        D3D11_TEXTURE2D_DESC* d3dTextureDesc,
                        D3D11_SUBRESOURCE_DATA* d3dSubresourceData);

        ComPtr<ID3D11Texture2D> GetD3D11Texture2D() const;

    private:
        static ComPtr<ID3D11Texture2D> CreateD3D11Texture2D(ComPtr<ID3D11Device> d3dDevice,
                                                            D3D11_TEXTURE2D_DESC* d3dTexture2DDesc,
                                                            D3D11_SUBRESOURCE_DATA* d3dSubresourceData);
    };

    class IDX11Texture2D {
    public:
        virtual DX11Texture2DData& Data() = 0;
        virtual void GetD3D11ResourceDesc(D3D11_SHADER_RESOURCE_VIEW_DESC& desc) = 0;
    };

    class DX11Texture2D : public IDX11Texture2D, public Texture2D {
        GENERATE_BODY(DX11Texture2D, Texture2D)

    private:
        DX11Texture2DData m_dxTexture2DWrapper;

    public:
        DX11Texture2D(const ObjectArgument& argument);
        DX11Texture2D(const DX11Texture2D&) = default;
        virtual ~DX11Texture2D() = default;

        TextureFormat GetFormat() const override;
        Int32 GetWidth() const override;
        Int32 GetHeight() const override;

        DX11Texture2DData& Data() override;
        void GetD3D11ResourceDesc(D3D11_SHADER_RESOURCE_VIEW_DESC& desc) override;

        virtual void Create(DX11Context* context, TextureInfo info);
    };

    class DX11CubeTexture2D : public IDX11Texture2D, public CubeTexture2D {
        GENERATE_BODY(DX11CubeTexture2D, CubeTexture2D)

    private:
        DX11Texture2DData m_dxTexture2DWrapper;

    public:
        DX11CubeTexture2D(const ObjectArgument& argument);
        DX11CubeTexture2D(const DX11CubeTexture2D&) = default;
        virtual ~DX11CubeTexture2D() = default;

        TextureFormat GetFormat() const override;
        Int32 GetWidth() const override;
        Int32 GetHeight() const override;

        DX11Texture2DData& Data() override;
        void GetD3D11ResourceDesc(D3D11_SHADER_RESOURCE_VIEW_DESC& desc) override;

        virtual void Create(DX11Context* context, TextureInfo info);
    };

    class DX11ResourceTexture2D : public DX11Texture2D {
        GENERATE_BODY(DX11ResourceTexture2D, DX11Texture2D)

    public:
        DX11ResourceTexture2D(const ObjectArgument& argument);
        DX11ResourceTexture2D(const DX11ResourceTexture2D&) = default;
        virtual ~DX11ResourceTexture2D() = default;

        String GetTag() const override;
        void Create(DX11Context* context, TextureInfo info) override;
    };

    class DX11ResourceCubeTexture2D : public DX11CubeTexture2D {
        GENERATE_BODY(DX11ResourceTexture2D, DX11CubeTexture2D)

    public:
        DX11ResourceCubeTexture2D(const ObjectArgument& argument);
        DX11ResourceCubeTexture2D(const DX11ResourceCubeTexture2D&) = default;
        virtual ~DX11ResourceCubeTexture2D() = default;

        String GetTag() const override;
        void Create(DX11Context* context, TextureInfo info) override;
    };

    class DX11OutputTexture2D : public DX11Texture2D {
        GENERATE_BODY(DX11OutputTexture2D, DX11Texture2D)

    public:
        DX11OutputTexture2D(const ObjectArgument& argument);
        DX11OutputTexture2D(const DX11OutputTexture2D&) = default;
        virtual ~DX11OutputTexture2D() = default;

        String GetTag() const override;
        void Create(DX11Context* context, TextureInfo info) override;
    };

    class DX11OutputDepthStencilTexture2D : public DX11Texture2D {
        GENERATE_BODY(DX11OutputDepthStencilTexture2D, DX11Texture2D)

    public:
        DX11OutputDepthStencilTexture2D(const ObjectArgument& argument);
        DX11OutputDepthStencilTexture2D(const DX11OutputDepthStencilTexture2D&) = default;
        virtual ~DX11OutputDepthStencilTexture2D() = default;

        String GetTag() const override;
        void Create(DX11Context* context, TextureInfo info) override;
        void GetD3D11ResourceDesc(D3D11_SHADER_RESOURCE_VIEW_DESC& desc) override;
    };

    class DX11OutputCubeTexture2D : public DX11CubeTexture2D {
        GENERATE_BODY(DX11OutputCubeTexture2D, DX11CubeTexture2D)

    public:
        DX11OutputCubeTexture2D(const ObjectArgument& argument);
        DX11OutputCubeTexture2D(const DX11OutputCubeTexture2D&) = default;
        virtual ~DX11OutputCubeTexture2D() = default;

        void ReadBits(DX11Context* context, Int8* bits, Size size);

        String GetTag() const override;
        void Create(DX11Context* context, TextureInfo info) override;
    };
}

#endif // DX11TEXTURE_H
