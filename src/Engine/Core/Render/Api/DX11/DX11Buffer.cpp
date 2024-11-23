#include "Engine/Core/Render/Api/DX11/DX11Buffer.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    DX11Buffer::DX11Buffer(const String& name, IContext* context, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlags, 
        UINT cpuAccessFlags, UINT size, UINT strides, const void* data)
        : BufferResource(name, context), m_strides(strides), m_offset(0)
        , m_data(RawData::nulldata), m_isMutable(usage == D3D11_USAGE_DYNAMIC) {

        D3D11_BUFFER_DESC d3dBufferDesc;
        ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
        d3dBufferDesc.ByteWidth = size * strides;
        d3dBufferDesc.Usage = usage;
        d3dBufferDesc.BindFlags = bindFlags;
        d3dBufferDesc.CPUAccessFlags = cpuAccessFlags;

        D3D11_SUBRESOURCE_DATA d3dSubresourceData;
        ZeroMemory(&d3dSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
        d3dSubresourceData.pSysMem = data;

        HRESULT hr = 0;
        ComPtr<ID3D11Device> d3dDevice = dynamic_cast<DX11Context*>(context)->GetD3D11Device();
        if (FAILED(hr = d3dDevice->CreateBuffer(&d3dBufferDesc, &d3dSubresourceData, &m_d3dBuffer))) {
            throw EngineException("[DX11Buffer] ID3DDevice::CreateBuffer() failed");
        }

        if (m_isMutable) {
            Size dataSize = size * strides;

            m_data = RawData(dataSize);
            memcpy_s(m_data.As<char>(), dataSize, data, dataSize);
        }
    }

    Int32 DX11Buffer::GetNumberOfBytes() const {
        D3D11_BUFFER_DESC d3dBufferDesc;
        m_d3dBuffer->GetDesc(&d3dBufferDesc);
        return static_cast<Int32>(d3dBufferDesc.ByteWidth);
    }

    Int32 DX11Buffer::GetNumberOfElements() const {
        return static_cast<Int32>(GetNumberOfBytes() / m_strides);
    }

    RawData& DX11Buffer::GetBufferData() {
        assert(m_isMutable);
        return m_data;
    }

    void DX11Buffer::Update() {
        assert(m_isMutable);

        ComPtr<ID3D11DeviceContext> d3dDevice = dynamic_cast<DX11Context*>(GetContext())->GetD3D11Context();
        Int32 size = GetNumberOfBytes();

        void* data = Lock(d3dDevice);
        memcpy_s(data, size, m_data.As<char>(), size);
        Unlock(d3dDevice);
    }

    ComPtr<ID3D11Buffer> DX11Buffer::GetD3D11Buffer() const {
        return m_d3dBuffer;
    }

    const UINT* DX11Buffer::GetStrides() const {
        return &m_strides;
    }

    const UINT* DX11Buffer::GetOffset() const {
        return &m_offset;
    }

    void* DX11Buffer::Lock(ComPtr<ID3D11DeviceContext> d3dContext) {
        D3D11_MAPPED_SUBRESOURCE d3dMappedSubresources;
        ZeroMemory(&d3dMappedSubresources, sizeof(d3dMappedSubresources));
        d3dContext->Map(m_d3dBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedSubresources);
        return d3dMappedSubresources.pData;
    }

    void DX11Buffer::Unlock(ComPtr<ID3D11DeviceContext> d3dContext) {
        d3dContext->Unmap(m_d3dBuffer.Get(), 0);
    }
}
