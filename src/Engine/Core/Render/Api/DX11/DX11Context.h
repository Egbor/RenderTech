#ifndef DX11CONTEXT_H
#define DX11CONTEXT_H

#include "Engine/Core/Render/Api/DX11/DX11Def.h"
#include "Engine/Core/Render/Base/RenderBase.h"
#include "Engine/Core/Render/Base/Interface/IContext.h"

#include "Engine/Core/Utils/Factory.h"
#include "Engine/Core/System/Platform/Interface/IWindow.h"

namespace Engine {
    class DX11Context : public IContext, public IRenderResourceFactory, public IRenderPipeline, public ISwapChain {
    private:
        ComPtr<ID3D11Device> m_d3dDevice;
        ComPtr<ID3D11DeviceContext> m_d3dContext;
        ComPtr<IDXGISwapChain> m_dxgiSwapChain;

        D3D_FEATURE_LEVEL m_d3dCurrentFeatureLevel;

    public:
        DX11Context(IWindow* window);
        virtual ~DX11Context();

        void Init(const _Core* core) override;

        ComPtr<ID3D11Device> GetD3D11Device() const;
        ComPtr<ID3D11DeviceContext> GetD3D11Context() const;

        Int32 GetWidth() const override;
        Int32 GetHeight() const override;

        IRenderResourceFactory* QueryResourceFactory() override;
        IRenderPipeline* QueryPipeline() override;
        ISwapChain* QuerySwapChain() override;

        StateResource* CreateState(StateType type, const String& name, StateData data) override;
        TargetResource* CreateTarget(TextureType type, const String& name, TextureFormat format, Int32 width, Int32 height) override;
        BufferResource* CreateBuffer(BufferType type, const String& name, Int32 size, Int32 strides, const void* data) override;
        ShaderResource* CreateShader(RenderStage stage, const String& name, Size codeLength, const void* code) override;
        TextureResource* CreateTexture(TextureType type, const String& name, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) override;

        void SetViewport(Int32 width, Int32 height) override;
        void SetTargets(const Array<TargetResource*>& targets) override;
        void SetStates(const Array<StateResource*>& states) override;

        void WipeTargets() override;

        void GetViewport(Viewport& viewport) override;
        IRenderStage* GetStage(RenderStage stage) override;

        TargetResource* GetOutputTarget() const override;

        void Swap() override;
        void Draw(BufferResource* vertexBuffer, BufferResource* indexBuffer) override;
        void DrawWaveframe(BufferResource* vertexBuffer, BufferResource* indexBuffer) override;

    private:
        void RegisterStateFactory();
        void RegisterTextureFactory();
        void RegisterBufferFactory();
        void RegisterShaderFactory();

        Factory<StateType, StateResource, const String&, StateData> m_stateFactory;
        Factory<RenderStage, ShaderResource, const String&, Size, const void*> m_shaderFactory;
        Factory<BufferType, BufferResource, const String&, Int32, Int32, const void*> m_bufferFactory;
        Factory<TextureType, TargetResource, const String&, TextureFormat, Int32, Int32> m_renderTargetFactory;
        Factory<TextureType, TextureResource, const String&, TextureFormat, Int32, Int32, Array<Int8*>> m_staticTextureFactory;

        TargetResource* m_backTarget;
    };
}

#endif // DX11CONTEXT_H
