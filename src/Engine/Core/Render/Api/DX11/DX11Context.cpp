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

    DX11ContextFactory::DX11ContextFactory() {
        InitializeRenderBinderFactory();
        InitializeCubeTexture2DFactory();
        InitializeRenderStateFactory();
        InitializeTexture2DFactory();
        InitializeBufferFactory();
        InitializeShaderFactory();
        InitializeTargetFactory();
    }

    void DX11ContextFactory::InitializeRenderBinderFactory() {
        binderFactory.Register(tagREALTIME_BUFFER_BINDER, new DefaultCreatorWith1Args<RenderBinderBase, DX11BufferBinder, DX11Context*>());
        binderFactory.Register(tagREALTIME_SHADER_BINDER, new DefaultCreatorWith1Args<RenderBinderBase, DX11ShaderBinder, DX11Context*>());
        binderFactory.Register(tagREALTIME_TARGET_BINDER, new DefaultCreatorWith1Args<RenderBinderBase, DX11RenderTargetBinder, DX11Context*>());
        binderFactory.Register(tagREALTIME_TEXTURE_BINDER, new DefaultCreatorWith1Args<RenderBinderBase, DX11TextureBinder, DX11Context*>());
        binderFactory.Register(tagREALTIME_VIEWPORT_BINDER, new DefaultCreatorWith1Args<RenderBinderBase, DX11ViewportBinder, DX11Context*>());
        binderFactory.Register(tagREALTIME_DYNAMIC_BUFFER_BINDER, new DefaultCreatorWith1Args<RenderBinderBase, DX11DynamicBufferBinder, DX11Context*>());
    }

    void DX11ContextFactory::InitializeCubeTexture2DFactory() {
        cubeTexture2DFactory.Register(tagCubeTexture, new DefaultCreatorWith1Args<CubeTexture2D, DX11ResourceCubeTexture2D, ObjectArgument>());
        cubeTexture2DFactory.Register(tagCubeMapTargetTexture, new DefaultCreatorWith1Args<CubeTexture2D, DX11OutputCubeTexture2D, ObjectArgument>());
    }

    void DX11ContextFactory::InitializeRenderStateFactory() {
        stateFactory.Register(tagREALTIME_RASTERIZER_STATE, new DefaultCreatorWith1Args<RenderState, DX11RasterizerState, DX11Context*>());
        stateFactory.Register(tagREALTIME_SAMPLER_STATE, new DefaultCreatorWith1Args<RenderState, DX11SamplerState, DX11Context*>());
        stateFactory.Register(tagREALTIME_DEPTHSTENCIL_STATE, new DefaultCreatorWith1Args<RenderState, DX11DepthStencilState, DX11Context*>());
        stateFactory.Register(tagREALTIME_BLEND_STATE, new DefaultCreatorWith1Args<RenderState, DX11BlendState, DX11Context*>());
    }

    void DX11ContextFactory::InitializeBufferFactory() {
        bufferFactory.Register(tagREALTIME_BUFFER_VERTEX, new DefaultCreatorWith1Args<Buffer, DX11VertexBuffer, ObjectArgument>());
        bufferFactory.Register(tagREALTIME_BUFFER_INDEX, new DefaultCreatorWith1Args<Buffer, DX11IndexBuffer, ObjectArgument>());
        bufferFactory.Register(tagREALTIME_BUFFER_VS_CONST_BUFFER, new DefaultCreatorWith1Args<Buffer, DX11VSConstBuffer, ObjectArgument>());
        bufferFactory.Register(tagREALTIME_BUFFER_PS_CONST_BUFFER, new DefaultCreatorWith1Args<Buffer, DX11PSConstBuffer, ObjectArgument>());
    }

    void DX11ContextFactory::InitializeShaderFactory() {
        shaderFactory.Register(tagREALTIME_SHADER_VERTEX, new DefaultCreatorWith1Args<Shader, DX11VertexShader, ObjectArgument>());
        shaderFactory.Register(tagREALTIME_SHADER_FRAGMENT, new DefaultCreatorWith1Args<Shader, DX11PixelShader, ObjectArgument>());
    }

    void DX11ContextFactory::InitializeTexture2DFactory() {
        texture2DFactory.Register(tagTexture, new DefaultCreatorWith1Args<Texture2D, DX11ResourceTexture2D, ObjectArgument>());
        texture2DFactory.Register(tagTargetTexture, new DefaultCreatorWith1Args<Texture2D, DX11OutputTexture2D, ObjectArgument>());
        texture2DFactory.Register(tagDepthStencilTargetTexture, new DefaultCreatorWith1Args<Texture2D, DX11OutputDepthStencilTexture2D, ObjectArgument>());
    }

    void DX11ContextFactory::InitializeTargetFactory() {
        targetFactory.Register(tagTargetTexture, new DefaultCreatorWith1Args<Target, DX11OutputTarget, ObjectArgument>());
        targetFactory.Register(tagCubeMapTargetTexture, new DefaultCreatorWith1Args<Target, DX11OutputCubeTarget, ObjectArgument>());
        targetFactory.Register(tagDepthStencilTargetTexture, new DefaultCreatorWith1Args<Target, DX11DepthStencilTarget, ObjectArgument>());
    }
}
