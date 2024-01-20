#ifndef DX11CONTEXT_H
#define DX11CONTEXT_H

#include "Engine/Core/Render/Api/DX11/DX11Def.h"
#include "Engine/Core/Render/Api/Interface/IContext.h"

#include "Engine/Core/Utils/Factory.h"
#include "Engine/Core/System/Platform/Interface/IWindow.h"

namespace Engine {
    class DX11Context : public IContext, IRenderResourceFactory, IRenderPipeline, ISwapChain {
    private:
        ComPtr<ID3D11Device> m_d3dDevice;
        ComPtr<ID3D11DeviceContext> m_d3dContext;
        ComPtr<IDXGISwapChain> m_dxgiSwapChain;

        D3D_FEATURE_LEVEL m_d3dCurrentFeatureLevel;

    public:
        DX11Context(IWindow* window);
        virtual ~DX11Context();

        ComPtr<ID3D11Device> GetD3D11Device() const;
        ComPtr<ID3D11DeviceContext> GetD3D11Context() const;

        Int32 GetWidth() const override;
        Int32 GetHeight() const override;

        IRenderResourceFactory* QueryResourceFactory() override;
        IRenderPipeline* QueryPipeline() override;
        ISwapChain* QuerySwapChain() override;

        IStateResourceData* CreateState(StateType type) override;
        ITextureResourceData* CreateTexture(TextureType type, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) override;
        ITargetResourceData* CreateTarget(TextureType type, TextureFormat format, Int32 width, Int32 height) override;
        IBufferResourceData* CreateBuffer(BufferType type, Int32 size, Int32 strides, const void* data) override;
        IShaderResourceData* CreateShader(ShaderType type, Size codeLength, const void* code) override;

        void SetViewport(Int32 width, Int32 height) override;
        void SetTargets(const Array<ITargetResourceData*>& targets) override;

        void GetViewport(Viewport& viewport) override;
        IRenderStage* GetStage(RenderStage stage) override;

        ITargetResourceData* GetOutputTarget() const override;

        void Swap() override;
        void Draw(IBufferResourceData* vertexBuffer, IBufferResourceData* indexBuffer) override;

    private:
        void RegisterStateFactory();
        void RegisterTextureFactory();
        void RegisterBufferFactory();
        void RegisterShaderFactory();

        Factory<StateType, IStateResourceData> m_stateFactory;
        Factory<TextureType, ITextureResourceData, ComPtr<ID3D11Device>, TextureFormat, Int32, Int32, Array<Int8*>> m_staticTextureFactory;
        Factory<TextureType, ITargetResourceData, ComPtr<ID3D11Device>, TextureFormat, Int32, Int32> m_renderTargetFactory;
        Factory<BufferType, IBufferResourceData, ComPtr<ID3D11Device>, Int32, Int32, const void*> m_bufferFactory;
        Factory<ShaderType, IShaderResourceData, ComPtr<ID3D11Device>, Size, const void*> m_shaderFactory;

        ITargetResourceData* m_backTarget;
    };
}

#endif // DX11CONTEXT_H
