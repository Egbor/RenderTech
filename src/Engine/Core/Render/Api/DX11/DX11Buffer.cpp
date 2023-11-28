#include "Engine/Core/Render/Api/DX11/DX11Buffer.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Exception/EngineException.h"

namespace Engine {
    DX11Buffer::DX11Buffer(ComPtr<ID3D11Device> d3dDevice, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlags, UINT cpuAccessFlags, UINT size, UINT strides, const void* data)
        : m_strides(strides), m_offset(0) {
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
        if (FAILED(hr = d3dDevice->CreateBuffer(&d3dBufferDesc, &d3dSubresourceData, &m_d3dBuffer))) {
            throw EngineException("[DX11Buffer] ID3DDevice::CreateBuffer() failed");
        }
    }

    Int32 DX11Buffer::GetNumBytes() const {
        D3D11_BUFFER_DESC d3dBufferDesc;
        m_d3dBuffer->GetDesc(&d3dBufferDesc);
        return static_cast<Int32>(d3dBufferDesc.ByteWidth);
    }

    Int32 DX11Buffer::GetNumElements() const {
        return static_cast<Int32>(GetNumBytes() / m_strides);
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

    DX11VertexBuffer::DX11VertexBuffer(ComPtr<ID3D11Device> d3dDevice, UINT size, UINT strides, const void* data) 
        : DX11Buffer(d3dDevice, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, size, strides, data) {

    }

    DX11IndexBuffer::DX11IndexBuffer(ComPtr<ID3D11Device> d3dDevice, UINT size, UINT strides, const void* data) 
        : DX11Buffer(d3dDevice, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, size, strides, data) {

    }

    DX11ConstantBuffer::DX11ConstantBuffer(ComPtr<ID3D11Device> d3dDevice, UINT size, UINT strides, const void* data) 
        : DX11Buffer(d3dDevice, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, size, strides, data)
        , m_data(size * strides) {
        memcpy_s(&m_data[0], m_data.size(), data, size * strides);
    }

    RawData DX11ConstantBuffer::GetBufferData() {
        return RawData(&m_data[0]);
    }

    void DX11ConstantBuffer::Update(IContext* context) {
        DX11Context* dxContext = dynamic_cast<DX11Context*>(context);

        void* data = Lock(dxContext->GetD3D11Context());
        memcpy_s(data, GetNumBytes(), &m_data[0], m_data.size());
        Unlock(dxContext->GetD3D11Context());
    }

    void* DX11ConstantBuffer::Lock(ComPtr<ID3D11DeviceContext> d3dContext) {
        D3D11_MAPPED_SUBRESOURCE d3dMappedSubresources;
        ZeroMemory(&d3dMappedSubresources, sizeof(d3dMappedSubresources));
        d3dContext->Map(m_d3dBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedSubresources);
        return d3dMappedSubresources.pData;
    }

    void DX11ConstantBuffer::Unlock(ComPtr<ID3D11DeviceContext> d3dContext) {
        d3dContext->Unmap(m_d3dBuffer.Get(), 0);
    }
}
