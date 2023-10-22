#ifndef DX11CONTEXT_H
#define DX11CONTEXT_H

#include "Engine/Core/Render/Api/DX11/DX11Def.h"
#include "Engine/Core/Render/Api/Interface/IContext.h"
#include "Engine/Core/Utils/Factory.h"
//#include "Engine/Tools/Factory.h"

namespace Engine {
    //struct DX11ContextFactory {
    //    Factory<RenderBinderBase> binderFactory;
    //    Factory<CubeTexture2D> cubeTexture2DFactory;
    //    Factory<RenderState> stateFactory;
    //    Factory<Texture2D> texture2DFactory;
    //    Factory<Buffer> bufferFactory;
    //    Factory<Shader> shaderFactory;
    //    Factory<Target> targetFactory;

    //    DX11ContextFactory();
    //    virtual ~DX11ContextFactory() = default;

    //private:
    //    void InitializeRenderBinderFactory();
    //    void InitializeCubeTexture2DFactory();
    //    void InitializeRenderStateFactory();
    //    void InitializeTexture2DFactory();
    //    void InitializeBufferFactory();
    //    void InitializeShaderFactory();
    //    void InitializeTargetFactory();
    //};

    class DX11Context : public IContext, IRenderResourceFactory, IRenderPipeline {
    private:
        ComPtr<ID3D11Device> m_d3dDevice;
        ComPtr<ID3D11DeviceContext> m_d3dContext;

        D3D_FEATURE_LEVEL m_d3dCurrentFeatureLevel;
        DX11ContextFactory m_factory;

    public:
        DX11Context();
        DX11Context(const DX11Context&) = default;
        virtual ~DX11Context() = default;

        ComPtr<ID3D11Device> GetD3D11Device() const;
        ComPtr<ID3D11DeviceContext> GetD3D11Context() const;

        void DebugReport() override;
        void ReadCubeTexture2D(CubeTexture2D* texture, Int32 face, Int8* outBits, Size outBitsSize) override;
        void DrawIndexed(UInt32 count) override;
        void WaitEndOfDraw() override;

        void SetViewport(Int32 width, Int32 height) override;
        void GetViewport(Int32& width, Int32& height) override;

        SwapChain* CreateSwapChain(UInt32 width, UInt32 height, UInt64 winId) override;
        RenderBinderBase* CreateBinder(const String& tag) override;
        RenderState* CreateState(const String& tag) override;

        DynamicBuffer* CreateDynamicBuffer(const String& tag, const BufferInfo& info) override;
        //CubeTexture2D* CreateCubeTexture2D(const String& tag, const TextureInfo& info) override;
        //Texture2D* CreateTexture2D(const String& tag, const TextureInfo& info) override;
        Shader* CreateShader(const String& tag, const ShaderInfo& info) override;
        Buffer* CreateBuffer(const String& tag, const BufferInfo& info) override;
        //Target* CreateTarget(const String& tag, Texture2D* texture) override;

        Texture2D* Copy(Texture2D* srcTexture) override;

        IRenderResourceFactory* QueryResourceFactory() override;
        IRenderPipeline* QueryPipeline() override;

        ITextureResourceData* CreateTexture(TextureType type, TextureFormat format, Int32 width, Int32 height, Array<Int8*> data) override;
        ITextureResourceData* CreateTexture(TextureType type, TextureFormat format, Int32 width, Int32 height) override;
        IBufferResourceData* CreateBuffer(BufferType type, Int32 size, Int32 strides, const void* data) override;

        void SetBuffers(const Array<IBufferResourceData*>& resources) override;

        void Draw(IBufferResourceData* vertexBuffer, IBufferResourceData* indexBuffer) override;
        void Update(IBufferResourceData* buffer, Int32 size, const void* data) override;

    private:
        void RegisterTextureFactory();
        void RegisterBufferFactory();

        Factory<TextureType, ITextureResourceData, ComPtr<ID3D11Device>, TextureFormat, Int32, Int32, Array<Int8*>> m_staticTextureFactory;
        Factory<TextureType, ITextureResourceData, ComPtr<ID3D11Device>, TextureFormat, Int32, Int32> m_renderTextureFactory;
        Factory<BufferType, IBufferResourceData, ComPtr<ID3D11Device>, Int32, Int32, const void*> m_bufferFactory;
    };
}

#endif // DX11CONTEXT_H
