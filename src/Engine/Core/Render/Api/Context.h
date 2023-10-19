#ifndef CONTEXT_H
#define CONTEXT_H

#include "Engine/Core/Render/Api/RenderDef.h"
#include "Engine/Core/Render/Api/RenderBinder.h"
#include "Engine/Core/Render/Api/SwapChain.h"
#include "Engine/Core/Render/Api/State/RenderState.h"

#include "Engine/Object/Class/Buffer.h"
#include "Engine/Object/Class/Shader.h"
#include "Engine/Object/Class/Texture.h"
#include "Engine/Object/Class/Target.h"

namespace Engine {
    class Context {
    public:
        Context() = default;
        virtual ~Context() = default;

        virtual void DebugReport() = 0;
        virtual void ReadCubeTexture2D(CubeTexture2D* texture, Int32 face, Int8* outBits, Size outBitsSize) = 0;
        virtual void DrawIndexed(UInt32 count) = 0;
        virtual void WaitEndOfDraw() = 0;

        virtual void SetViewport(Int32 width, Int32 height) = 0;
        virtual void GetViewport(Int32& width, Int32& height) = 0;

        virtual SwapChain* CreateSwapChain(UInt32 width, UInt32 height, UInt64 winId) = 0;
        virtual RenderBinderBase* CreateBinder(const String& tag) = 0;
        virtual RenderState* CreateState(const String& tag) = 0;

        virtual DynamicBuffer* CreateDynamicBuffer(const String& tag, const BufferInfo& info) = 0;
        virtual CubeTexture2D* CreateCubeTexture2D(const String& tag, const TextureInfo& info) = 0;
        virtual Texture2D* CreateTexture2D(const String& tag, const TextureInfo& info) = 0;
        virtual Buffer* CreateBuffer(const String& tag, const BufferInfo& info) = 0;
        virtual Shader* CreateShader(const String& tag, const ShaderInfo& info) = 0;
        virtual Target* CreateTarget(const String& tag, Texture2D* texture) = 0;

        virtual Texture2D* Copy(Texture2D* srcTexture) = 0;
    };
}

#endif // CONTEXT_H
