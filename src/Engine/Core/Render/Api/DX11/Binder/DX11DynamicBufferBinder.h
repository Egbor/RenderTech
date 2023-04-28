#ifndef DX11DYNAMICBUFFERBINDER_H
#define DX11DYNAMICBUFFERBINDER_H

#include "Engine/Core/Render/Api/RenderBinder.h"
#include "Engine/Core/Render/Api/DX11/DX11Buffer.h"

namespace Engine {
    class DX11DynamicBufferBinder : public RenderBinder<RenderDynamicBuffer, DX11DynamicBuffer> {
    private:
        DX11Context* m_dxContext;
        Array<DX11DynamicBuffer*> m_dxBuffers;

    public:
        DX11DynamicBufferBinder(DX11Context* context);
        DX11DynamicBufferBinder(const DX11DynamicBufferBinder&) = default;
        virtual ~DX11DynamicBufferBinder() = default;

        void Bind() override;
        void UnBind() override;

        void AttachRenderItem(RenderDynamicBuffer id, DX11DynamicBuffer* buffer) override;
        void DetachRenderItem(RenderDynamicBuffer id) override;
    };
}

#endif // DX11DYNAMICBUFFERBINDER_H
