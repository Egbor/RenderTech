#include "Engine/Core/Render/Api/DX11/DX11Target.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include "Engine/Object/Global/EngineConfig.h"

namespace Engine {
    //GENERATE_RTTI_DEFINITIONS(DX11Target)
    GENERATE_INSTANTIATION(DX11Target)

    DX11Target::DX11Target(const ObjectArgument& argument)
        : Super(argument), m_dxTexture(nullptr) {
    }

    DX11Target::~DX11Target() {
        DELETE_OBJECT(m_dxTexture);
    }

    ComPtr<ID3D11View> DX11Target::GetD3D11View() const {
        throw EngineException("[DX11Target] DX11Target::GetD3D11View() s abstaract method without any features");
    }

    void DX11Target::Clear(TargetClear clear) {
        throw EngineException("[DX11Target] DX11Target::Clear() s abstaract method without any features");
    }

    void DX11Target::Create(Texture2D* texture) {
        throw EngineException("[DX11Target] DX11Target::Create() s abstaract method without any features");
    }

    Texture2D* DX11Target::GetTexture2D() const {
        return m_dxTexture;
    }

    //GENERATE_RTTI_DEFINITIONS(DX11OutputTarget)
    GENERATE_INSTANTIATION(DX11OutputTarget)

    DX11OutputTarget::DX11OutputTarget(const ObjectArgument& argument)
        : Super(argument) {

    }

    ComPtr<ID3D11View> DX11OutputTarget::GetD3D11View() const {
        return m_d3dView;
    }

    void DX11OutputTarget::Clear(TargetClear clear) {
        DX11Context* dxContext = static_cast<DX11Context*>(EngineConfig::GetInstance().GetContext());
        ComPtr<ID3D11DeviceContext> context = dxContext->GetD3D11Context();
        context->ClearRenderTargetView(m_d3dView.Get(), clear.forTarget.rgba);
    }

    void DX11OutputTarget::Create(Texture2D* texture) {
        DX11Context* dxContext = static_cast<DX11Context*>(EngineConfig::GetInstance().GetContext());
        DX11Texture2D* dxTexture = texture->As<DX11Texture2D>();

        ComPtr<ID3D11Device> d3dDevice = dxContext->GetD3D11Device();

        D3D11_RENDER_TARGET_VIEW_DESC d3dRenderTargetViewDesc;
        ZeroMemory(&d3dRenderTargetViewDesc, sizeof(d3dRenderTargetViewDesc));
        d3dRenderTargetViewDesc.Format = dxTexture->Data().GetD3D11Format();
        d3dRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        d3dRenderTargetViewDesc.Texture2D.MipSlice = 0;

        CreateD3D11View<DX11OutputTexture2D>(this, &dxTexture->Data(), [&](){
            return CreateD3D11RenderTargetView(d3dDevice, dxTexture->Data().GetD3D11Texture2D(), &d3dRenderTargetViewDesc, this);
        });
    }

    bool DX11OutputTarget::CreateD3D11RenderTargetView(ComPtr<ID3D11Device> d3dDevice,
                                                       ComPtr<ID3D11Texture2D> d3dTexture2D,
                                                       D3D11_RENDER_TARGET_VIEW_DESC* d3dRenderTargetViewDesc,
                                                       DX11OutputTarget* owner) {
        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateRenderTargetView(d3dTexture2D.Get(), d3dRenderTargetViewDesc, &owner->m_d3dView))) {
            throw EngineException("[DX11OutputTarget] ID3DDevice::CreateRenderTargetView() failed");
        }
        return true;
    }

    //GENERATE_RTTI_DEFINITIONS(DX11DepthStencilTarget)
    GENERATE_INSTANTIATION(DX11DepthStencilTarget)

    DX11DepthStencilTarget::DX11DepthStencilTarget(const ObjectArgument& argument)
        : Super(argument) {

    }

    ComPtr<ID3D11View> DX11DepthStencilTarget::GetD3D11View() const {
        return m_d3dView;
    }

    void DX11DepthStencilTarget::Clear(TargetClear clear) {
        DX11Context* dxContext = static_cast<DX11Context*>(EngineConfig::GetInstance().GetContext());
        ComPtr<ID3D11DeviceContext> context = dxContext->GetD3D11Context();

        UINT clearFlags = 0;
        if (clear.forDepthStencil.depth) {
            clearFlags |= D3D11_CLEAR_DEPTH;
        }
        if (clear.forDepthStencil.stencil) {
            clearFlags |= D3D11_CLEAR_STENCIL;
        }

        context->ClearDepthStencilView(m_d3dView.Get(), clearFlags, clear.forDepthStencil.depth, clear.forDepthStencil.stencil);
    }

    bool DX11DepthStencilTarget::CreateD3D11DepthStencilView(ComPtr<ID3D11Device> d3dDevice,
                                                             ComPtr<ID3D11Texture2D> d3dTexture2D,
                                                             D3D11_DEPTH_STENCIL_VIEW_DESC* d3dDepthStencilViewDesc,
                                                             DX11DepthStencilTarget* owner) {
        HRESULT hr = 0;
        if (FAILED(hr = d3dDevice->CreateDepthStencilView(d3dTexture2D.Get(), d3dDepthStencilViewDesc, &owner->m_d3dView))) {
            throw EngineException("[DX11DepthStencilTarget] ID3DDevice::CreateDepthStencilView() failed");
        }
        return true;
    }

    void DX11DepthStencilTarget::Create(Texture2D* texture) {
        DX11Context* dxContext = static_cast<DX11Context*>(EngineConfig::GetInstance().GetContext());
        DX11Texture2D* dxTexture = texture->As<DX11Texture2D>();

        ComPtr<ID3D11Device> d3dDevice = dxContext->GetD3D11Device();

        D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
        ZeroMemory(&d3dDepthStencilViewDesc, sizeof(d3dDepthStencilViewDesc));
        d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;

        CreateD3D11View<DX11OutputDepthStencilTexture2D>(this, &dxTexture->Data(), [&](){
            return CreateD3D11DepthStencilView(d3dDevice, dxTexture->Data().GetD3D11Texture2D(), &d3dDepthStencilViewDesc, this);
        });
    }

    //GENERATE_RTTI_DEFINITIONS(DX11OutputCubeTarget)
    GENERATE_INSTANTIATION(DX11OutputCubeTarget)

    DX11OutputCubeTarget::DX11OutputCubeTarget(const ObjectArgument& argument)
        : Super(argument) {
    }

    void DX11OutputCubeTarget::Create(Texture2D* texture) {
        DX11Context* dxContext = static_cast<DX11Context*>(EngineConfig::GetInstance().GetContext());
        DX11CubeTexture2D* dxTexture = texture->As<DX11CubeTexture2D>();

        ComPtr<ID3D11Device> d3dDevice = dxContext->GetD3D11Device();

        D3D11_RENDER_TARGET_VIEW_DESC d3dRenderTargetViewDesc;
        ZeroMemory(&d3dRenderTargetViewDesc, sizeof(d3dRenderTargetViewDesc));
        d3dRenderTargetViewDesc.Format = dxTexture->Data().GetD3D11Format();
        d3dRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
        d3dRenderTargetViewDesc.Texture2DArray.MipSlice = 0;
        d3dRenderTargetViewDesc.Texture2DArray.ArraySize = 1;
        d3dRenderTargetViewDesc.Texture2DArray.FirstArraySlice = dxTexture->GetFaceId();

        CreateD3D11View<DX11OutputCubeTexture2D>(this, &dxTexture->Data(), [&](){
            return CreateD3D11RenderTargetView(d3dDevice, dxTexture->Data().GetD3D11Texture2D(), &d3dRenderTargetViewDesc, this);
        });
    }
}
