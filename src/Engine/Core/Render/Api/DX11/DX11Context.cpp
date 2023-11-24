#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include "Engine/Core/Render/Api/DX11/DX11Buffer.h"
#include "Engine/Core/Render/Api/DX11/DX11Shader.h"
#include "Engine/Core/Render/Api/DX11/DX11Target.h"
#include "Engine/Core/Render/Api/DX11/DX11Stage.h"

namespace Engine {
    DX11Context::DX11Context(IWindow* window) {
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
        dxgiSwapChainDesc.Windowed = FALSE;

        if (FAILED(hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flag, features, ARRAYSIZE(features), D3D11_SDK_VERSION, &dxgiSwapChainDesc,
            &m_dxgiSwapChain, &m_d3dDevice, &m_d3dCurrentFeatureLevel, &m_d3dContext))) {
            throw EngineException("[DX11Context] D3D11CreateDevice() failed");
        }

        ID3D11Texture2D* backBuffer;
        if (FAILED(hr = m_dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)))) {
            throw new EngineException("[DX11Context] IDXGISwapChain::GetBuffer() failed.");
        }

        Float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        m_backTarget = new DX11RenderTarget(m_d3dDevice, new DX11Texture2D(backBuffer), color);
    }

    ComPtr<ID3D11Device> DX11Context::GetD3D11Device() const {
        return m_d3dDevice;
    }

    ComPtr<ID3D11DeviceContext> DX11Context::GetD3D11Context() const {
        return m_d3dContext;
    }

//    void DX11Context::DebugReport() {
//#if defined(DEBUG) || defined(_DEBUG)
//        ComPtr<IDXGIDebug1> dxgiDebug;
//        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_IDXGIDebug1, &dxgiDebug))) {
//            dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
//        }
//#endif
//    }

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

    ITextureResourceData* DX11Context::CreateTexture(TextureType type, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) {
        return m_staticTextureFactory.Create(type, m_d3dDevice, format, width, height, data);
    }

    ITargetResourceData* DX11Context::CreateTarget(TextureType type, TextureFormat format, Int32 width, Int32 height) {
        return m_renderTargetFactory.Create(type, m_d3dDevice, format, width, height);
    }

    IBufferResourceData* DX11Context::CreateBuffer(BufferType type, Int32 size, Int32 strides, const void* data) {
        return m_bufferFactory.Create(type, m_d3dDevice, size, strides, data);
    }

    IShaderResourceData* DX11Context::CreateShader(ShaderType type, Size codeLength, const void* code) {
        return m_shaderFactory.Create(type, m_d3dDevice, codeLength, code);
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

    void DX11Context::SetTargets(const Array<ITargetResourceData*>& targets) {
        Array<ID3D11RenderTargetView*> rendertargets;
        ID3D11DepthStencilView* depthstencil;

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

    IRenderStage* DX11Context::GetStage(RenderStage stage) {
        switch (stage) {
        case RenderStage::RS_VERTEX: {
            static DX11StageVS vsStage(m_d3dDevice, m_d3dContext);
            return &vsStage;
        }
        case RenderStage::RS_PIXEL: {
            static DX11StagePS psStage(m_d3dDevice, m_d3dContext);
            return &psStage;
        }
        default:
            break;
        }
        throw EngineException("[DX11Context] DX11Context::GetStage() failed. Selected render stage is not supported");
    }

    ITargetResourceData* DX11Context::GetOutputTarget() const {
        return m_backTarget;
    }

    void DX11Context::Swap() {
        m_dxgiSwapChain->Present(1, 0);
    }

    void DX11Context::Draw(IBufferResourceData* vertexBuffer, IBufferResourceData* indexBuffer) {
        DX11Buffer* vertex = dynamic_cast<DX11Buffer*>(vertexBuffer);
        DX11Buffer* index = dynamic_cast<DX11Buffer*>(indexBuffer);

        m_d3dContext->IASetVertexBuffers(0, 1, vertex->GetD3D11Buffer().GetAddressOf(), vertex->GetStrides(), vertex->GetOffset());
        m_d3dContext->IASetIndexBuffer(index->GetD3D11Buffer().Get(), DXGI_FORMAT_R32_UINT, 0);

        m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_d3dContext->DrawIndexed(indexBuffer->GetNumElements(), 0, 0);
    }

    void DX11Context::Update(IBufferResourceData* buffer, const void* data, Int32 size) {
        DX11ConstantBuffer* dxBuffer = dynamic_cast<DX11ConstantBuffer*>(buffer);
        void* mem = dxBuffer->Lock(m_d3dContext);
        memcpy(mem, data, dxBuffer->GetNumBytes());
        dxBuffer->Unlock(m_d3dContext);
    }

    void DX11Context::RegisterTextureFactory() {
        m_staticTextureFactory.Register(TextureType::TT_DEFAULT, [&](ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) { 
            return new DX11Texture2D(d3dDevice, format, width, height, data[0]); });
        m_staticTextureFactory.Register(TextureType::TT_CUBE, [&](ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) { 
            return new DX11Texture2D(d3dDevice, format, width, height, data); });

        Float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

        m_renderTargetFactory.Register(TextureType::TT_DEFAULT, [&](ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height) {
            return new DX11RenderTarget(d3dDevice, new DX11Texture2D(d3dDevice, format, width, height, false), color); });
        m_renderTargetFactory.Register(TextureType::TT_CUBE, [&](ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height) {
            return new DX11RenderTarget(d3dDevice, new DX11Texture2D(d3dDevice, format, width, height, true), color); });
        m_renderTargetFactory.Register(TextureType::TT_DEPTH, [&](ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height) {
            return new DX11DepthStencil(d3dDevice, new DX11Texture2D(d3dDevice, format, width, height, false), 1.0f, 0); });
        m_renderTargetFactory.Register(TextureType::TT_DEPTH_CUBE, [&](ComPtr<ID3D11Device> d3dDevice, TextureFormat format, Int32 width, Int32 height) {
            return new DX11DepthStencil(d3dDevice, new DX11Texture2D(d3dDevice, format, width, height, true), 1.0f, 0); });
    }

    void DX11Context::RegisterBufferFactory() {
        m_bufferFactory.Register(BufferType::BT_VERTEX, [&](ComPtr<ID3D11Device> d3dDevice, Int32 size, Int32 strides, const void* data) {
            return new DX11VertexBuffer(d3dDevice, size, strides, data); });
        m_bufferFactory.Register(BufferType::BT_INDEX, [&](ComPtr<ID3D11Device> d3dDevice, Int32 size, Int32 strides, const void* data) {
            return new DX11IndexBuffer(d3dDevice, size, strides, data); });
        m_bufferFactory.Register(BufferType::BT_UNIFORM, [&](ComPtr<ID3D11Device> d3dDevice, Int32 size, Int32 strides, const void* data) {
            return new DX11ConstantBuffer(d3dDevice, size, strides, data); });
    }

    void DX11Context::RegisterShaderFactory() {
        m_shaderFactory.Register(ShaderType::ST_VERTEX, [&](ComPtr<ID3D11Device> d3dDevice, Size codeLength, const void* code) {
            return new DX11VertexShader(d3dDevice, codeLength, code); });
        m_shaderFactory.Register(ShaderType::ST_PIXEL, [&](ComPtr<ID3D11Device> d3dDevice, Size codeLength, const void* code) {
            return new DX11PixelShader(d3dDevice, codeLength, code); });
    }
}
