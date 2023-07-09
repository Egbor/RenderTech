#include "Engine/Core/Render/Api/DX11/DX11Buffer.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include "Engine/Object/Global/EngineConfig.h"

namespace Engine {
    //GENERATE_RTTI_DEFINITIONS(DX11Buffer)
    GENERATE_INSTANTIATION(DX11Buffer)

    DX11Buffer::DX11Buffer(const ObjectArgument& argument)
        : Super(argument) {

    }

    ComPtr<ID3D11Buffer> DX11Buffer::GetD3D11Buffer() const {
        throw new EngineException("[DX11Buffer] DX11Buffer::GetD3D11Buffer() is abstract method without any features");
    }

    void DX11Buffer::Bind(DX11Context* dxContext) {
        throw new EngineException("[DX11Buffer] DX11Buffer::Bind() is abstract method without any features");
    }

    void DX11Buffer::UnBind(DX11Context* dxContext) {
        throw new EngineException("[DX11Buffer] DX11Buffer::UnBind() is abstract method without any features");
    }

    void DX11Buffer::Create(const DX11Context* context, const void* buffer, UInt32 bufferSize, UInt32 typeSize) {
        throw new EngineException("[DX11Buffer] DX11Buffer::Create() is abstract method without any features");
    }

    //GENERATE_RTTI_DEFINITIONS(DX11DynamicBuffer)
    GENERATE_INSTANTIATION(DX11DynamicBuffer)

    DX11DynamicBuffer::DX11DynamicBuffer(const ObjectArgument& argument)
        : Super(argument) {

    }

    ComPtr<ID3D11Buffer> DX11DynamicBuffer::GetD3D11Buffer() const {
        throw new EngineException("[DX11DynamicBuffer] DX11DynamicBuffer::GetD3D11Buffer() is abstract method without any features");
    }

    void DX11DynamicBuffer::Bind(DX11Context* dxContext, UInt32 slot) {
        throw new EngineException("[DX11DynamicBuffer] DX11DynamicBuffer::Bind() is abstract method without any features");
    }

    void DX11DynamicBuffer::UnBind(DX11Context* dxContext, UInt32 slot) {
        throw new EngineException("[DX11SynamicBuffer] DX11SynamicBuffer::UnBind() is abstract method without any features");
    }

    void DX11DynamicBuffer::Create(const DX11Context* context, const void* buffer, UInt32 bufferSize, UInt32 typeSize) {
        throw new EngineException("[DX11DynamicBuffer] DX11DynamicBuffer::Create() is abstract method without any features");
    }

    //GENERATE_RTTI_DEFINITIONS(DX11VertexBuffer)
    GENERATE_INSTANTIATION(DX11VertexBuffer)

    DX11VertexBuffer::DX11VertexBuffer(const ObjectArgument& argument)
        : Super(argument) {
    }

    ComPtr<ID3D11Buffer> DX11VertexBuffer::GetD3D11Buffer() const {
        return m_d3dBuffer.Get();
    }

    UInt32 DX11VertexBuffer::GetBufferNumBytes() const {
        D3D11_BUFFER_DESC d3dBufferDesc;
        m_d3dBuffer->GetDesc(&d3dBufferDesc);
        return d3dBufferDesc.ByteWidth;
    }

    UInt32 DX11VertexBuffer::GetBufferNumElements() const {
        return GetBufferNumBytes() / D3D11InputStride;
    }

    void DX11VertexBuffer::Create(const DX11Context* context, const void* buffer, UInt32 bufferSize, UInt32 typeSize) {
        HRESULT hr = 0;
        ComPtr<ID3D11Device> d3dDevice = context->GetD3D11Device();

        D3D11_BUFFER_DESC d3dBufferDesc;
        ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
        d3dBufferDesc.ByteWidth = bufferSize * typeSize;
        d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA d3dSubresourceData;
        ZeroMemory(&d3dSubresourceData, sizeof(d3dSubresourceData));
        d3dSubresourceData.pSysMem = buffer;

        m_strides = typeSize;
        m_offsets = 0;

        if (FAILED(hr = d3dDevice->CreateBuffer(&d3dBufferDesc, &d3dSubresourceData, &m_d3dBuffer))) {
            throw new EngineException("[DX11VertexBuffer] ID3DDevice::CreateBuffer() failed");
        }
    }

    void DX11VertexBuffer::Bind(DX11Context* context) {
        ComPtr<ID3D11DeviceContext> d3dContext = context->GetD3D11Context();
        d3dContext->IASetVertexBuffers(0, 1, m_d3dBuffer.GetAddressOf(), &m_strides, &m_offsets);
    }

    void DX11VertexBuffer::UnBind(DX11Context* context) {
        ComPtr<ID3D11DeviceContext> d3dContext = context->GetD3D11Context();
        d3dContext->IASetVertexBuffers(0, 0, NULL, &m_strides, &m_offsets);
    }

    //GENERATE_RTTI_DEFINITIONS(DX11IndexBuffer)
    GENERATE_INSTANTIATION(DX11IndexBuffer)

    DX11IndexBuffer::DX11IndexBuffer(const ObjectArgument& argument)
        : Super(argument) {

    }

    ComPtr<ID3D11Buffer> DX11IndexBuffer::GetD3D11Buffer() const {
        return m_d3dBuffer;
    }

    UInt32 DX11IndexBuffer::GetBufferNumBytes() const {
        D3D11_BUFFER_DESC d3dBufferDesc;
        m_d3dBuffer->GetDesc(&d3dBufferDesc);
        return d3dBufferDesc.ByteWidth;
    }

    UInt32 DX11IndexBuffer::GetBufferNumElements() const {
        return GetBufferNumBytes() / sizeof(UINT);
    }

    void DX11IndexBuffer::Create(const DX11Context* context, const void* buffer, UInt32 bufferSize, UInt32 typeSize) {
        HRESULT hr = 0;
        ComPtr<ID3D11Device> d3dDevice = context->GetD3D11Device();

        D3D11_BUFFER_DESC d3dBufferDesc;
        ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
        d3dBufferDesc.ByteWidth = bufferSize * typeSize;
        d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA d3dSubresourceData;
        ZeroMemory(&d3dSubresourceData, sizeof(d3dSubresourceData));
        d3dSubresourceData.pSysMem = buffer;

        m_countOfIndexes = d3dBufferDesc.ByteWidth / sizeof(UINT);

        if (FAILED(hr = d3dDevice->CreateBuffer(&d3dBufferDesc, &d3dSubresourceData, &m_d3dBuffer))) {
            throw new EngineException("[DX11IndexBuffer] ID3DDevice::CreateBuffer() failed");
        }
    }

    void DX11IndexBuffer::Bind(DX11Context* context) {
        ComPtr<ID3D11DeviceContext> d3dContext = context->GetD3D11Context();
        d3dContext->IASetIndexBuffer(m_d3dBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    }

    void DX11IndexBuffer::UnBind(DX11Context* context) {
        ComPtr<ID3D11DeviceContext> d3dContext = context->GetD3D11Context();
        d3dContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
    }

    //GENERATE_RTTI_DEFINITIONS(DX11AbstractConstBuffer)
    GENERATE_INSTANTIATION(DX11AbstractConstBuffer)

    DX11AbstractConstBuffer::DX11AbstractConstBuffer(const ObjectArgument& argument)
        : Super(argument) {

    }

    ComPtr<ID3D11Buffer> DX11AbstractConstBuffer::GetD3D11Buffer() const {
        return m_d3dBuffer;
    }

    UInt32 DX11AbstractConstBuffer::GetBufferNumBytes() const {
        D3D11_BUFFER_DESC d3dBufferDesc;
        m_d3dBuffer->GetDesc(&d3dBufferDesc);
        return d3dBufferDesc.ByteWidth;
    }

    UInt32 DX11AbstractConstBuffer::GetBufferNumElements() const {
        return 1;
    }

    void DX11AbstractConstBuffer::Create(const DX11Context* context, const void* buffer, UInt32 bufferSize, UInt32 typeSize) {
        HRESULT hr = 0;
        ComPtr<ID3D11Device> d3dDevice = context->GetD3D11Device();

        D3D11_BUFFER_DESC d3dBufferDesc;
        ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
        d3dBufferDesc.ByteWidth = bufferSize * typeSize;
        d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        D3D11_SUBRESOURCE_DATA d3dSubresourceData;
        ZeroMemory(&d3dSubresourceData, sizeof(d3dSubresourceData));
        d3dSubresourceData.pSysMem = buffer;

        D3D11_SUBRESOURCE_DATA* data = buffer != nullptr ? &d3dSubresourceData : NULL;
        if (FAILED(hr = d3dDevice->CreateBuffer(&d3dBufferDesc, data, &m_d3dBuffer))) {
            throw new EngineException("[DX11AbstractConstBuffer] ID3DDevice::CreateBuffer() failed");
        }
    }

    void DX11AbstractConstBuffer::Update(const BufferInfo& info) {
        ComPtr<ID3D11DeviceContext> d3dContext = static_cast<DX11Context*>(EngineConfig::GetInstance().GetContext())->GetD3D11Context();

        D3D11_MAPPED_SUBRESOURCE d3dMappedSubresources;
        ZeroMemory(&d3dMappedSubresources, sizeof(d3dMappedSubresources));
        d3dContext->Map(m_d3dBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedSubresources);
        memcpy(d3dMappedSubresources.pData, info.data, info.bufferSize * info.typeSize);
        d3dContext->Unmap(m_d3dBuffer.Get(), 0);
    }

    //GENERATE_RTTI_DEFINITIONS(DX11VSConstBuffer)
    GENERATE_INSTANTIATION(DX11VSConstBuffer)

    DX11VSConstBuffer::DX11VSConstBuffer(const ObjectArgument& argument)
        : Super(argument) {

    }

    void DX11VSConstBuffer::Bind(DX11Context* context, UInt32 slot) {
        ComPtr<ID3D11DeviceContext> d3dContext = context->GetD3D11Context();
        ComPtr<ID3D11Buffer> d3dBuffer = GetD3D11Buffer();
        d3dContext->VSSetConstantBuffers(slot, 1, d3dBuffer.GetAddressOf());
    }

    void DX11VSConstBuffer::UnBind(DX11Context* context, UInt32 slot) {
        ComPtr<ID3D11DeviceContext> d3dContext = context->GetD3D11Context();
        d3dContext->VSSetConstantBuffers(slot, 0, NULL);
    }

    //GENERATE_RTTI_DEFINITIONS(DX11PSConstBuffer)
    GENERATE_INSTANTIATION(DX11PSConstBuffer)

    DX11PSConstBuffer::DX11PSConstBuffer(const ObjectArgument& argument)
        : Super(argument) {

    }

    void DX11PSConstBuffer::Bind(DX11Context* context, UInt32 slot) {
        ComPtr<ID3D11DeviceContext> d3dContext = context->GetD3D11Context();
        ComPtr<ID3D11Buffer> d3dBuffer = GetD3D11Buffer();
        d3dContext->PSSetConstantBuffers(slot, 1, d3dBuffer.GetAddressOf());
    }

    void DX11PSConstBuffer::UnBind(DX11Context* context, UInt32 slot) {
        ComPtr<ID3D11DeviceContext> d3dContext = context->GetD3D11Context();
        d3dContext->PSSetConstantBuffers(slot, 0, NULL);
    }
}
