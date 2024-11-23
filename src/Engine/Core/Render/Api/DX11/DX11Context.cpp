#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include "Engine/Core/Render/Api/DX11/DX11Buffer.h"
#include "Engine/Core/Render/Api/DX11/DX11Shader.h"
#include "Engine/Core/Render/Api/DX11/DX11Target.h"
#include "Engine/Core/Render/Api/DX11/DX11Stage.h"
#include "Engine/Core/Render/Api/DX11/DX11BlendState.h"
#include "Engine/Core/Render/Api/DX11/DX11DepthStencilState.h"
#include "Engine/Core/Render/Api/DX11/DX11RasterizerState.h"
#include "Engine/Core/Render/Api/DX11/DX11SamplerState.h"

#include "Engine/Core/Render/Api/DX11/Interface/IDX11SelfBindable.h"

namespace Engine {
    DX11Context::DX11Context(IWindow* window) {
        HRESULT hr = 0;
        Int32 flag = 0;

#if defined(DEBUG) || defined(_DEBUG)
        flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        const D3D_FEATURE_LEVEL features[] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
        };

        DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
        ZeroMemory(&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
        dxgiSwapChainDesc.BufferCount = 1;
        dxgiSwapChainDesc.BufferDesc.Width = static_cast<UINT>(window->GetWidth());
        dxgiSwapChainDesc.BufferDesc.Height = static_cast<UINT>(window->GetHeight());
        dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
        dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        dxgiSwapChainDesc.OutputWindow = reinterpret_cast<HWND>(window->WinId());
        dxgiSwapChainDesc.SampleDesc.Count = 1;
        dxgiSwapChainDesc.SampleDesc.Quality = 0;
        dxgiSwapChainDesc.Windowed = TRUE;

        ComPtr<IDXGIFactory> dxgiFactory;

        if (FAILED(hr = CreateDXGIFactory(__uuidof(IDXGIFactory), &dxgiFactory))) {
            throw EngineException("[DX11Context] CreateDXGIFactory() failed");
        }

        ComPtr<IDXGIAdapter> dxgiAdapter;
        for (UINT i = 0; !FAILED(hr = dxgiFactory->EnumAdapters(i, &dxgiAdapter)); i++) {
            DXGI_ADAPTER_DESC dxgiAdapterDesc;
            dxgiAdapter->GetDesc(&dxgiAdapterDesc);

            if ((dxgiAdapterDesc.VendorId == 0x10DE) || (dxgiAdapterDesc.VendorId == 0x1002)) {
                break;
            }
        }

        if (FAILED(hr = D3D11CreateDeviceAndSwapChain(dxgiAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, NULL, flag, features, ARRAYSIZE(features), D3D11_SDK_VERSION, &dxgiSwapChainDesc,
            &m_dxgiSwapChain, &m_d3dDevice, &m_d3dCurrentFeatureLevel, &m_d3dContext))) {
            throw EngineException("[DX11Context] D3D11CreateDevice() failed");
        }

        ID3D11Texture2D* backBuffer;
        if (FAILED(hr = m_dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)))) {
            throw new EngineException("[DX11Context] IDXGISwapChain::GetBuffer() failed.");
        }

        Float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        m_backTarget = new DX11RenderTarget("OutputTarget", this, new DX11Texture2D("OutputTarget_tex2d", this, backBuffer), color);
        
        RegisterStateFactory();
        RegisterTextureFactory();
        RegisterBufferFactory();
        RegisterShaderFactory();
    }

    DX11Context::~DX11Context() {
        m_d3dContext->ClearState();
        m_d3dContext->Flush();

        DELETE_OBJECT(m_backTarget);
    }

    ComPtr<ID3D11Device> DX11Context::GetD3D11Device() const {
        return m_d3dDevice;
    }

    ComPtr<ID3D11DeviceContext> DX11Context::GetD3D11Context() const {
        return m_d3dContext;
    }

    Int32 DX11Context::GetWidth() const {
        DXGI_SWAP_CHAIN_DESC desc;
        m_dxgiSwapChain->GetDesc(&desc);
        return static_cast<Int32>(desc.BufferDesc.Width);
    }

    Int32 DX11Context::GetHeight() const {
        DXGI_SWAP_CHAIN_DESC desc;
        m_dxgiSwapChain->GetDesc(&desc);
        return static_cast<Int32>(desc.BufferDesc.Height);
    }

    IRenderResourceFactory* DX11Context::QueryResourceFactory() {
        return dynamic_cast<IRenderResourceFactory*>(this);
    }

    IRenderPipeline* DX11Context::QueryPipeline() {
        return dynamic_cast<IRenderPipeline*>(this);
    }

    ISwapChain* DX11Context::QuerySwapChain() {
        return dynamic_cast<ISwapChain*>(this);
    }

    StateResource* DX11Context::CreateState(StateType type, const String& name, StateData data) {
        return m_stateFactory.Create(type, name, data);
    }

    TextureResource* DX11Context::CreateTexture(TextureType type, const String& name, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) {
        return m_staticTextureFactory.Create(type, name, format, width, height, data);
    }

    TargetResource* DX11Context::CreateTarget(TextureType type, const String& name, TextureFormat format, Int32 width, Int32 height) {
        return m_renderTargetFactory.Create(type, name, format, width, height);
    }

    BufferResource* DX11Context::CreateBuffer(BufferType type, const String& name, Int32 size, Int32 strides, const void* data) {
        return m_bufferFactory.Create(type, name, size, strides, data);
    }

    ShaderResource* DX11Context::CreateShader(RenderStage stage, const String& name, Size codeLength, const void* code) {
        return m_shaderFactory.Create(stage, name, codeLength, code);
    }

    void DX11Context::SetViewport(Int32 width, Int32 height) {
        D3D11_VIEWPORT d3dViewport;
        d3dViewport.TopLeftX = 0;
        d3dViewport.TopLeftY = 0;
        d3dViewport.Width = static_cast<FLOAT>(width);
        d3dViewport.Height = static_cast<FLOAT>(height);
        d3dViewport.MinDepth = 0.0f;
        d3dViewport.MaxDepth = 1.0f;

        m_d3dContext->RSSetViewports(1, &d3dViewport);
    }

    void DX11Context::SetTargets(const Array<TargetResource*>& targets) {
        Array<ID3D11RenderTargetView*> rendertargets;
        ID3D11DepthStencilView* depthstencil = nullptr;

        for (Size i = 0; i < targets.size(); i++) {
            if (targets[i]->IsDepth()) {
                DX11DepthStencil* target = dynamic_cast<DX11DepthStencil*>(targets[i]);
                depthstencil = target->GetD3D11DepthStencilView().Get();
            } else {
                DX11RenderTarget* target = dynamic_cast<DX11RenderTarget*>(targets[i]);
                rendertargets.push_back(target->GetD3D11RenderTargetView().Get());
            }
        }

        m_d3dContext->OMSetRenderTargets(static_cast<UINT>(rendertargets.size()), rendertargets.data(), depthstencil);
    }

    void DX11Context::SetStates(const Array<StateResource*>& states) {
        for (Size i = 0; i < states.size(); i++) {
            StandaloneStateResource* state = dynamic_cast<StandaloneStateResource*>(states[i]);
            state->Bind();
        }
    }

    void DX11Context::GetViewport(Viewport& viewport) {
        D3D11_VIEWPORT d3dViewport;
        UINT d3dViewportNum = 1;

        m_d3dContext->RSGetViewports(&d3dViewportNum, &d3dViewport);
        viewport = Viewport(static_cast<Int32>(d3dViewport.Width), static_cast<Int32>(d3dViewport.Height));
    }

    IRenderStage* DX11Context::GetStage(RenderStage stage) {
        switch (stage) {
        case RenderStage::RS_VERTEX: {
            static DX11StageVS vsStage(this);
            return &vsStage;
        }
        case RenderStage::RS_PIXEL: {
            static DX11StagePS psStage(this);
            return &psStage;
        }
        default:
            break;
        }
        throw EngineException("[DX11Context] DX11Context::GetStage() failed. Selected render stage is not supported");
    }

    TargetResource* DX11Context::GetOutputTarget() const {
        return m_backTarget;
    }

    void DX11Context::Swap() {
        m_dxgiSwapChain->Present(0, 0);
    }

    void DX11Context::Draw(BufferResource* vertexBuffer, BufferResource* indexBuffer) {
        DX11Buffer* vertex = dynamic_cast<DX11Buffer*>(vertexBuffer);
        DX11Buffer* index = dynamic_cast<DX11Buffer*>(indexBuffer);

        m_d3dContext->IASetVertexBuffers(0, 1, vertex->GetD3D11Buffer().GetAddressOf(), vertex->GetStrides(), vertex->GetOffset());
        m_d3dContext->IASetIndexBuffer(index->GetD3D11Buffer().Get(), DXGI_FORMAT_R32_UINT, 0);

        m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_d3dContext->DrawIndexed(indexBuffer->GetNumberOfElements(), 0, 0);
    }

    void DX11Context::DrawWaveframe(BufferResource* vertexBuffer, BufferResource* indexBuffer) {
        DX11Buffer* vertex = dynamic_cast<DX11Buffer*>(vertexBuffer);
        DX11Buffer* index = dynamic_cast<DX11Buffer*>(indexBuffer);

        m_d3dContext->IASetVertexBuffers(0, 1, vertex->GetD3D11Buffer().GetAddressOf(), vertex->GetStrides(), vertex->GetOffset());
        m_d3dContext->IASetIndexBuffer(index->GetD3D11Buffer().Get(), DXGI_FORMAT_R32_UINT, 0);

        m_d3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
        m_d3dContext->DrawIndexed(indexBuffer->GetNumberOfElements(), 0, 0);
    }

    void DX11Context::RegisterStateFactory() {
        m_stateFactory.Register(StateType::ST_BLEND, [&](const String& name, StateData& data) {
            return new DX11BlendState(name, data, this); });
        m_stateFactory.Register(StateType::ST_DEPTH_STENCIL, [&](const String& name, StateData& data) {
            return new DX11DepthStencilState(name, data, this); });
        m_stateFactory.Register(StateType::ST_RASTERIZER, [&](const String& name, StateData& data) {
            return new DX11RasterizerState(name, data, this); });
        m_stateFactory.Register(StateType::ST_SAMPLER, [&](const String& name, StateData& data) {
            return new DX11SamplerState(name, data, this); });
    }

    void DX11Context::RegisterTextureFactory() {
        m_staticTextureFactory.Register(TextureType::TT_DEFAULT, [&](const String& name, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) {
            return new DX11Texture2D(name, this, format, width, height, data[0]); });
        m_staticTextureFactory.Register(TextureType::TT_CUBE, [&](const String& name, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) {
            return new DX11Texture2D(name, this, format, width, height, data); });

        static Float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

        m_renderTargetFactory.Register(TextureType::TT_DEFAULT, [&](const String& name, TextureFormat format, Int32 width, Int32 height) {
            return new DX11RenderTarget(name, this, new DX11Texture2D(name + "_tex2d", this, format, width, height, false), color); });
        m_renderTargetFactory.Register(TextureType::TT_CUBE, [&](const String& name, TextureFormat format, Int32 width, Int32 height) {
            return new DX11RenderTarget(name, this, new DX11Texture2D(name + "_tex2d", this, format, width, height, true), color); });
        m_renderTargetFactory.Register(TextureType::TT_DEPTH, [&](const String& name, TextureFormat format, Int32 width, Int32 height) {
            return new DX11DepthStencil(name, this, new DX11Texture2D(name + "_tex2d", this, format, width, height, false), 1.0f, 0); });
        m_renderTargetFactory.Register(TextureType::TT_DEPTH_CUBE, [&](const String& name, TextureFormat format, Int32 width, Int32 height) {
            return new DX11DepthStencil(name, this, new DX11Texture2D(name + "_tex2d", this, format, width, height, true), 1.0f, 0); });
    }

    void DX11Context::RegisterBufferFactory() {
        m_bufferFactory.Register(BufferType::BT_VERTEX, [&](const String& name, Int32 size, Int32 strides, const void* data) {
            return new DX11Buffer(name, this, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, size, strides, data); });
        m_bufferFactory.Register(BufferType::BT_INDEX, [&](const String& name, Int32 size, Int32 strides, const void* data) {
            return new DX11Buffer(name, this, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, size, strides, data); });
        m_bufferFactory.Register(BufferType::BT_UNIFORM, [&](const String& name, Int32 size, Int32 strides, const void* data) {
            return new DX11Buffer(name, this, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, size, strides, data); });
    }

    void DX11Context::RegisterShaderFactory() {
        m_shaderFactory.Register(RenderStage::RS_VERTEX, [&](const String& name, Size codeLength, const void* code) {
            return new DX11VertexShader(name, this, codeLength, code); });
        m_shaderFactory.Register(RenderStage::RS_PIXEL, [&](const String& name, Size codeLength, const void* code) {
            return new DX11PixelShader(name, this, codeLength, code); });
    }
}
