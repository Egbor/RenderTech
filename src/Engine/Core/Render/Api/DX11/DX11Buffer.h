#ifndef DX11BUFFER_H
#define DX11BUFFER_H

#include "Engine/Core/Render/Api/DX11/DX11Def.h"

#include "Engine/Core/Render/Api/Interface/IBufferResource.h"
#include "Engine/Core/Render/Api/Interface/IUpdatableBufferResource.h"

//#include "Engine/Object/Class/Buffer.h"
//#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11Buffer : public IBufferResourceData {
    public:
        DX11Buffer(ComPtr<ID3D11Device> d3dDevice, UInt32 size, UInt32 strides, const void* data);
        virtual ~DX11Buffer() = default;

        Int32 GetNumBytes() const override;
        Int32 GetNumElements() const override;

    private:
        ComPtr<ID3D11Buffer> m_d3dBuffer;
    };

    class DX11ConstBuffer : public DX11Buffer, IUpdatableBufferResource {
    public:
        DX11ConstBuffer(ComPtr<ID3D11Device> d3dDevice, UInt32 size, UInt32 strides, const void* data);
        virtual ~DX11ConstBuffer() = default;

        void Update() override;
    };

    //CLASSTYPE(DX11Buffer)
    //class DX11Buffer : public Buffer {
    //    GENERATE_BODY(DX11Buffer, Buffer);

    //public:
    //    DX11Buffer(const ObjectArgument& argument);
    //    DX11Buffer(const DX11Buffer&) = default;
    //    virtual ~DX11Buffer() = default;

    //    virtual ComPtr<ID3D11Buffer> GetD3D11Buffer() const;

    //    virtual void Bind(DX11Context* context);
    //    virtual void UnBind(DX11Context* context);

    //    virtual void Create(const DX11Context* context, const void* buffer, UInt32 bufferSize, UInt32 typeSize);
    //};

    //CLASSTYPE(DX11DynamicBuffer)
    //class DX11DynamicBuffer : public DynamicBuffer {
    //    GENERATE_BODY(DX11DynamicBuffer, DynamicBuffer);

    //public:
    //    DX11DynamicBuffer(const ObjectArgument& argument);
    //    DX11DynamicBuffer(const DX11DynamicBuffer&) = default;
    //    virtual ~DX11DynamicBuffer() = default;

    //    virtual ComPtr<ID3D11Buffer> GetD3D11Buffer() const;

    //    virtual void Bind(DX11Context* context, UInt32 slot);
    //    virtual void UnBind(DX11Context* context, UInt32 slot);

    //    virtual void Create(const DX11Context* context, const void* buffer, UInt32 bufferSize, UInt32 typeSize);
    //};

    //CLASSTYPE(DX11VertexBuffer)
    //class DX11VertexBuffer : public DX11Buffer {
    //    GENERATE_BODY(DX11VertexBuffer, DX11Buffer)

    //private:
    //    ComPtr<ID3D11Buffer> m_d3dBuffer;

    //    UINT m_strides;
    //    UINT m_offsets;

    //public:
    //    DX11VertexBuffer(const ObjectArgument& argument);
    //    DX11VertexBuffer(const DX11VertexBuffer&) = default;
    //    virtual ~DX11VertexBuffer() = default;

    //    ComPtr<ID3D11Buffer> GetD3D11Buffer() const override;

    //    UInt32 GetBufferNumBytes() const override;
    //    UInt32 GetBufferNumElements() const override;

    //    void Create(const DX11Context* context, const void* buffer, UInt32 bufferSize, UInt32 typeSize) override;

    //private:
    //    void Bind(DX11Context* context) override;
    //    void UnBind(DX11Context* context) override;
    //};

    //CLASSTYPE(DX11IndexBuffer)
    //class DX11IndexBuffer : public DX11Buffer {
    //    GENERATE_BODY(DX11IndexBuffer, DX11Buffer)

    //private:
    //    ComPtr<ID3D11Buffer> m_d3dBuffer;

    //    UINT m_countOfIndexes;

    //public:
    //    DX11IndexBuffer(const ObjectArgument& argument);
    //    DX11IndexBuffer(const DX11IndexBuffer&) = default;
    //    virtual ~DX11IndexBuffer() = default;

    //    ComPtr<ID3D11Buffer> GetD3D11Buffer() const override;

    //    UInt32 GetBufferNumBytes() const override;
    //    UInt32 GetBufferNumElements() const override;

    //    void Create(const DX11Context* context, const void* buffer, UInt32 bufferSize, UInt32 typeSize) override;

    //private:
    //    void Bind(DX11Context* context) override;
    //    void UnBind(DX11Context* context) override;
    //};

    //CLASSTYPE(DX11AbstractConstBuffer)
    //class DX11AbstractConstBuffer : public DX11DynamicBuffer {
    //    GENERATE_BODY(DX11AbstractConstBuffer, DX11DynamicBuffer)

    //private:
    //    ComPtr<ID3D11Buffer> m_d3dBuffer;

    //public:
    //    DX11AbstractConstBuffer(const ObjectArgument& argument);
    //    DX11AbstractConstBuffer(const DX11AbstractConstBuffer&) = default;
    //    virtual ~DX11AbstractConstBuffer() = default;

    //    ComPtr<ID3D11Buffer> GetD3D11Buffer() const override;

    //    UInt32 GetBufferNumBytes() const override;
    //    UInt32 GetBufferNumElements() const override;

    //    void Create(const DX11Context* context, const void* buffer, UInt32 bufferSize, UInt32 typeSize) override;
    //    void Update(const BufferInfo& info) override;
    //};

    //CLASSTYPE(DX11VSConstBuffer)
    //class DX11VSConstBuffer : public DX11AbstractConstBuffer {
    //    GENERATE_BODY(DX11VSConstBuffer, DX11AbstractConstBuffer)

    //public:
    //    DX11VSConstBuffer(const ObjectArgument& argument);
    //    DX11VSConstBuffer(const DX11VSConstBuffer&) = default;
    //    virtual ~DX11VSConstBuffer() = default;

    //private:
    //    void Bind(DX11Context* context, UInt32 slot) override;
    //    void UnBind(DX11Context* context, UInt32 slot) override;
    //};

    //CLASSTYPE(DX11PSConstBuffer)
    //class DX11PSConstBuffer : public DX11AbstractConstBuffer {
    //    GENERATE_BODY(DX11PSConstBuffer, DX11AbstractConstBuffer)

    //public:
    //    DX11PSConstBuffer(const ObjectArgument& argument);
    //    DX11PSConstBuffer(const DX11PSConstBuffer&) = default;
    //    virtual ~DX11PSConstBuffer() = default;

    //private:
    //    void Bind(DX11Context* context, UInt32 slot) override;
    //    void UnBind(DX11Context* context, UInt32 slot) override;
    //};
}

#endif // DX11BUFFER_H
