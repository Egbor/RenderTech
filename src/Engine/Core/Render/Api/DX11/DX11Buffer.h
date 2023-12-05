#ifndef DX11BUFFER_H
#define DX11BUFFER_H

#include "Engine/Core/Render/Api/DX11/DX11Def.h"
#include "Engine/Core/Render/Api/Interface/IBufferResource.h"

namespace Engine {
    class DX11Buffer : public IBufferResourceData {
    public:
        DX11Buffer(ComPtr<ID3D11Device> d3dDevice, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlags, UINT cpuAccessFlags, UINT size, UINT strides, const void* data);
        virtual ~DX11Buffer() = default;

        Int32 GetNumBytes() const override;
        Int32 GetNumElements() const override;

        ComPtr<ID3D11Buffer> GetD3D11Buffer() const;
        const UINT* GetStrides() const;
        const UINT* GetOffset() const;

    private:
        UINT m_strides;
        UINT m_offset;

    protected:
        ComPtr<ID3D11Buffer> m_d3dBuffer;
    };

    class DX11VertexBuffer : public DX11Buffer {
    public:
        DX11VertexBuffer(ComPtr<ID3D11Device> d3dDevice, UINT size, UINT strides, const void* data);
        virtual ~DX11VertexBuffer() = default;
    };
    
    class DX11IndexBuffer : public DX11Buffer {
    public:
        DX11IndexBuffer(ComPtr<ID3D11Device> d3dDevice, UINT size, UINT strides, const void* data);
        virtual ~DX11IndexBuffer() = default;
    };

    class DX11ConstantBuffer : public DX11Buffer, public IDynamicResourceData {
    public:
        DX11ConstantBuffer(ComPtr<ID3D11Device> d3dDevice, UINT size, UINT strides, const void* data);
        virtual ~DX11ConstantBuffer() = default;

        RawData GetBufferData() override;
        void Update(IContext* context) override;

    private:
        void* Lock(ComPtr<ID3D11DeviceContext> d3dContext);
        void Unlock(ComPtr<ID3D11DeviceContext> d3dContext);

        Array<Int8> m_data;
    };
}

#endif // DX11BUFFER_H
