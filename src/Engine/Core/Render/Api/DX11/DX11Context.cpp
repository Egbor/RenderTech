#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include "Engine/Core/Render/Api/DX11/Binder/DX11BufferBinder.h"
#include "Engine/Core/Render/Api/DX11/Binder/DX11ShaderBinder.h"
#include "Engine/Core/Render/Api/DX11/Binder/DX11RenderTargetBinder.h"
#include "Engine/Core/Render/Api/DX11/Binder/DX11TextureBinder.h"
#include "Engine/Core/Render/Api/DX11/Binder/DX11ViewportBinder.h"
#include "Engine/Core/Render/Api/DX11/Binder/DX11DynamicBufferBinder.h"

#include "Engine/Core/Render/Api/DX11/DX11RasterizerState.h"
#include "Engine/Core/Render/Api/DX11/DX11SamplerState.h"
#include "Engine/Core/Render/Api/DX11/DX11DepthStencilState.h"
#include "Engine/Core/Render/Api/DX11/DX11BlendState.h"

#include "Engine/Core/Render/Api/DX11/DX11SwapChain.h"

namespace Engine {
    DX11Context::DX11Context() {
        HRESULT hr = 0;
        Int32 flag = 0;

        const D3D_FEATURE_LEVEL features[] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
        };

#if defined(DEBUG) || defined(_DEBUG)
        flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        if (FAILED(hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flag, features,
            ARRAYSIZE(features), D3D11_SDK_VERSION,&m_d3dDevice, &m_d3dCurrentFeatureLevel, &m_d3dContext))) {
            throw EngineException("[DX11Context] D3D11CreateDevice() failed");
        }
    }

    ComPtr<ID3D11Device> DX11Context::GetD3D11Device() const {
        return m_d3dDevice;
    }

    ComPtr<ID3D11DeviceContext> DX11Context::GetD3D11Context() const {
        return m_d3dContext;
    }

    void DX11Context::DebugReport() {
#if defined(DEBUG) || defined(_DEBUG)
        ComPtr<IDXGIDebug1> dxgiDebug;
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_IDXGIDebug1, &dxgiDebug))) {
            dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
        }
#endif
    }

    void DX11Context::ReadCubeTexture2D(CubeTexture2D* texture, Int32 face, Int8* outBits, Size outBitsSize) {
        DX11OutputCubeTexture2D* dxTexture2D = texture->As<DX11OutputCubeTexture2D>();
        dxTexture2D->SetFaceId(face);
        dxTexture2D->ReadBits(this, outBits, outBitsSize);
        dxTexture2D->SetFaceId(0);
    }

    void DX11Context::DrawIndexed(UInt32 count) {
        m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_d3dContext->DrawIndexed(count, 0, 0);
    }

    void DX11Context::WaitEndOfDraw() {
        m_d3dContext->Flush();
    }

    void DX11Context::SetViewport(Int32 width, Int32 height) {
        D3D11_VIEWPORT viewport;
        viewport.Width = static_cast<Float>(width);
        viewport.Height = static_cast<Float>(height);
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        m_d3dContext->RSSetViewports(1, &viewport);
    }

    void DX11Context::GetViewport(Int32& width, Int32& height) {
        D3D11_VIEWPORT viewport;
        UINT num = 1;
        m_d3dContext->RSGetViewports(&num, &viewport);

        width = (Int32)viewport.Width;
        height = (Int32)viewport.Height;
    }

    SwapChain* DX11Context::CreateSwapChain(UInt32 width, UInt32 height, UInt64 winId) {
        return new DX11SwapChain(this, width, height, reinterpret_cast<HWND>(winId));
    }

    RenderBinderBase* DX11Context::CreateBinder(const String& tag) {
        return m_factory.binderFactory.Create(tag, this);
    }

    RenderState* DX11Context::CreateState(const String& tag) {
        return m_factory.stateFactory.Create(tag, this);
    }

    Shader* DX11Context::CreateShader(const String& tag, const ShaderInfo& info) {
        DX11Shader* shader = m_factory.shaderFactory.CreateAs<DX11Shader>(tag, ObjectArgument::Dummy());
        shader->Create(this, info.data, info.dataSize);
        return shader;
    }

    Buffer* DX11Context::CreateBuffer(const String& tag, const BufferInfo& info) {
        DX11Buffer* buffer = m_factory.bufferFactory.CreateAs<DX11Buffer>(tag, ObjectArgument::Dummy());
        buffer->Create(this, info.data, info.bufferSize, info.typeSize);
        return buffer;
    }

    DynamicBuffer* DX11Context::CreateDynamicBuffer(const String& tag, const BufferInfo& info) {
        DX11DynamicBuffer* buffer = m_factory.bufferFactory.CreateAs<DX11DynamicBuffer>(tag, ObjectArgument::Dummy());
        buffer->Create(this, info.data, info.bufferSize, info.typeSize);
        return buffer;
    }
    CubeTexture2D* DX11Context::CreateCubeTexture2D(const String& tag, const TextureInfo& info) {
        DX11CubeTexture2D* texture = m_factory.cubeTexture2DFactory.CreateAs<DX11CubeTexture2D>(tag, ObjectArgument::Dummy());
        texture->Create(this, info);
        return texture;
    }

    Texture2D* DX11Context::CreateTexture2D(const String& tag, const TextureInfo& info) {
        DX11Texture2D* texture = m_factory.texture2DFactory.CreateAs<DX11Texture2D>(tag, ObjectArgument::Dummy());
        texture->Create(this, info);
        return texture;
    }

    Target* DX11Context::CreateTarget(const String& tag, Texture2D* texture) {
        DX11Target* target = m_factory.targetFactory.CreateAs<DX11Target>(tag, ObjectArgument::Dummy());
        target->Create(static_cast<DX11Texture2D*>(texture));
        return target;
    }

    Texture2D* DX11Context::Copy(Texture2D* srcTexture) {
        TextureInfo info;
        info.width = srcTexture->GetWidth();
        info.height = srcTexture->GetHeight();
        info.format = srcTexture->GetFormat();

        Texture2D* newTexture = CreateTexture2D(srcTexture->GetTag(), info);

        ComPtr<ID3D11Texture2D> srcD3DTexture2D = static_cast<DX11Texture2D*>(srcTexture)->Data().GetD3D11Texture2D();
        ComPtr<ID3D11Texture2D> dstD3DTexture2D = static_cast<DX11Texture2D*>(newTexture)->Data().GetD3D11Texture2D();
        m_d3dContext->CopyResource(dstD3DTexture2D.Get(), srcD3DTexture2D.Get());

        return newTexture;
    }

    IRenderResourceFactory* DX11Context::QueryResourceFactory() {
        return dynamic_cast<IRenderResourceFactory*>(this);
    }

    IRenderPipeline* DX11Context::QueryPipeline() {
        return dynamic_cast<IRenderPipeline*>(this);
    }

    ITextureResourceData* DX11Context::CreateTexture(TextureType type, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) {
        return m_staticTextureFactory.Create(type, m_d3dDevice, format, width, height, data);
    }

    ITextureResourceData* DX11Context::CreateTexture(TextureType type, TextureFormat format, Int32 width, Int32 height) {
        return m_renderTextureFactory.Create(type, m_d3dDevice, format, width, height);
    }

    IBufferResourceData* DX11Context::CreateBuffer(BufferType type, Int32 size, Int32 strides, const void* data) {
        return m_bufferFactory.Create(type, m_d3dDevice, size, strides, data);
    }

    void DX11Context::SetBuffers(const Array<IBufferResourceData*>& resources) {

    }

    void DX11Context::Draw(IBufferResourceData* vertexBuffer, IBufferResourceData* indexBuffer) {
        SetBuffer(vertexBuffer);
        SetBuffer(indexBuffer);

        m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_d3dContext->DrawIndexed(indexBuffer->GetNumElements(), 0, 0);
    }

    void DX11Context::Update(IBufferResourceData* buffer, Int32 size, const void* data) {
        DX11Buffer* dxBuffer = dynamic_cast<DX11Buffer*>(buffer);
        dxBuffer->Update(m_d3dContext, static_cast<UINT>(size), data);
    }

    void DX11Context::RegisterTextureFactory() {
        m_staticTextureFactory.Register(TextureType::TT_DEFAULT, [&](ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) { 
            return new DX11Texture2DResourceData(d3dDevice, format, width, height, data[0]); });
        m_staticTextureFactory.Register(TextureType::TT_CUBE, [&](ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) { 
            return new DX11Texture2DResourceData(d3dDevice, format, width, height, data); });

        m_renderTextureFactory.Register(TextureType::TT_DEFAULT, [&](ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height) {
            return new DX11TextureRenderResourceData(d3dDevice, format, width, height, false); });
        m_renderTextureFactory.Register(TextureType::TT_CUBE, [&](ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height) {
            return new DX11TextureRenderResourceData(d3dDevice, format, width, height, true); });
        m_renderTextureFactory.Register(TextureType::TT_DEPTH, [&](ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height) {
            return new DX11DepthStencilResourceData(d3dDevice, format, width, height, false); });
        m_renderTextureFactory.Register(TextureType::TT_DEPTH_CUBE, [&](ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height) {
            return new DX11DepthStencilResourceData(d3dDevice, format, width, height, true); });
    }

    void DX11Context::RegisterBufferFactory() {
        m_bufferFactory.Register(BufferType::BT_VERTEX, [&](ComPtr<ID3D11Device> d3dDevice, Int32 size, Int32 strides, const void* data) {
            return new DX11Buffer(d3dDevice, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, size, strides, data); });
        m_bufferFactory.Register(BufferType::BT_INDEX, [&](ComPtr<ID3D11Device> d3dDevice, Int32 size, Int32 strides, const void* data) {
            return new DX11Buffer(d3dDevice, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, size, strides, data); });
        m_bufferFactory.Register(BufferType::BT_UNIFORM, [&](ComPtr<ID3D11Device> d3dDevice, Int32 size, Int32 strides, const void* data) {
            return new DX11Buffer(d3dDevice, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, size, strides, data); });
    }

    //DX11ContextFactory::DX11ContextFactory() {
    //    InitializeRenderBinderFactory();
    //    InitializeCubeTexture2DFactory();
    //    InitializeRenderStateFactory();
    //    InitializeTexture2DFactory();
    //    InitializeBufferFactory();
    //    InitializeShaderFactory();
    //    InitializeTargetFactory();
    //}

    //void DX11ContextFactory::InitializeRenderBinderFactory() {
    //    binderFactory.Register(tagREALTIME_BUFFER_BINDER, new DefaultCreatorWith1Args<RenderBinderBase, DX11BufferBinder, DX11Context*>());
    //    binderFactory.Register(tagREALTIME_SHADER_BINDER, new DefaultCreatorWith1Args<RenderBinderBase, DX11ShaderBinder, DX11Context*>());
    //    binderFactory.Register(tagREALTIME_TARGET_BINDER, new DefaultCreatorWith1Args<RenderBinderBase, DX11RenderTargetBinder, DX11Context*>());
    //    binderFactory.Register(tagREALTIME_TEXTURE_BINDER, new DefaultCreatorWith1Args<RenderBinderBase, DX11TextureBinder, DX11Context*>());
    //    binderFactory.Register(tagREALTIME_VIEWPORT_BINDER, new DefaultCreatorWith1Args<RenderBinderBase, DX11ViewportBinder, DX11Context*>());
    //    binderFactory.Register(tagREALTIME_DYNAMIC_BUFFER_BINDER, new DefaultCreatorWith1Args<RenderBinderBase, DX11DynamicBufferBinder, DX11Context*>());
    //}

    //void DX11ContextFactory::InitializeCubeTexture2DFactory() {
    //    cubeTexture2DFactory.Register(tagCubeTexture, new DefaultCreatorWith1Args<CubeTexture2D, DX11ResourceCubeTexture2D, ObjectArgument>());
    //    cubeTexture2DFactory.Register(tagCubeMapTargetTexture, new DefaultCreatorWith1Args<CubeTexture2D, DX11OutputCubeTexture2D, ObjectArgument>());
    //}

    //void DX11ContextFactory::InitializeRenderStateFactory() {
    //    stateFactory.Register(tagREALTIME_RASTERIZER_STATE, new DefaultCreatorWith1Args<RenderState, DX11RasterizerState, DX11Context*>());
    //    stateFactory.Register(tagREALTIME_SAMPLER_STATE, new DefaultCreatorWith1Args<RenderState, DX11SamplerState, DX11Context*>());
    //    stateFactory.Register(tagREALTIME_DEPTHSTENCIL_STATE, new DefaultCreatorWith1Args<RenderState, DX11DepthStencilState, DX11Context*>());
    //    stateFactory.Register(tagREALTIME_BLEND_STATE, new DefaultCreatorWith1Args<RenderState, DX11BlendState, DX11Context*>());
    //}

    //void DX11ContextFactory::InitializeBufferFactory() {
    //    bufferFactory.Register(tagREALTIME_BUFFER_VERTEX, new DefaultCreatorWith1Args<Buffer, DX11VertexBuffer, ObjectArgument>());
    //    bufferFactory.Register(tagREALTIME_BUFFER_INDEX, new DefaultCreatorWith1Args<Buffer, DX11IndexBuffer, ObjectArgument>());
    //    bufferFactory.Register(tagREALTIME_BUFFER_VS_CONST_BUFFER, new DefaultCreatorWith1Args<Buffer, DX11VSConstBuffer, ObjectArgument>());
    //    bufferFactory.Register(tagREALTIME_BUFFER_PS_CONST_BUFFER, new DefaultCreatorWith1Args<Buffer, DX11PSConstBuffer, ObjectArgument>());
    //}

    //void DX11ContextFactory::InitializeShaderFactory() {
    //    shaderFactory.Register(tagREALTIME_SHADER_VERTEX, new DefaultCreatorWith1Args<Shader, DX11VertexShader, ObjectArgument>());
    //    shaderFactory.Register(tagREALTIME_SHADER_FRAGMENT, new DefaultCreatorWith1Args<Shader, DX11PixelShader, ObjectArgument>());
    //}

    //void DX11ContextFactory::InitializeTexture2DFactory() {
    //    texture2DFactory.Register(tagTexture, new DefaultCreatorWith1Args<Texture2D, DX11ResourceTexture2D, ObjectArgument>());
    //    texture2DFactory.Register(tagTargetTexture, new DefaultCreatorWith1Args<Texture2D, DX11OutputTexture2D, ObjectArgument>());
    //    texture2DFactory.Register(tagDepthStencilTargetTexture, new DefaultCreatorWith1Args<Texture2D, DX11OutputDepthStencilTexture2D, ObjectArgument>());
    //}

    //void DX11ContextFactory::InitializeTargetFactory() {
    //    targetFactory.Register(tagTargetTexture, new DefaultCreatorWith1Args<Target, DX11OutputTarget, ObjectArgument>());
    //    targetFactory.Register(tagCubeMapTargetTexture, new DefaultCreatorWith1Args<Target, DX11OutputCubeTarget, ObjectArgument>());
    //    targetFactory.Register(tagDepthStencilTargetTexture, new DefaultCreatorWith1Args<Target, DX11DepthStencilTarget, ObjectArgument>());
    //}
}
