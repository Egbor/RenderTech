#ifndef DX11BUFFERBINDER_H
#define DX11BUFFERBINDER_H

#include "Engine/Core/Render/Api/RenderBinder.h"
#include "Engine/Core/Render/Api/DX11/DX11Buffer.h"

namespace Engine {
    class DX11BufferBinder : public RenderBinder<RenderBuffer, DX11Buffer> {
    private:
        DX11Context* m_dxContext;
        Array<DX11Buffer*> m_dxBuffers;

    public:
        DX11BufferBinder(DX11Context* context);
        DX11BufferBinder(const DX11BufferBinder&) = default;
        virtual ~DX11BufferBinder() = default;

        void Bind() override;
        void UnBind() override;

        void AttachRenderItem(RenderBuffer id, DX11Buffer* buffer) override;
        void DetachRenderItem(RenderBuffer id) override;

    private:
        void AttachRenderItem(Int32 id, void* item) override;
        void DetachRenderItem(Int32 id) override;
    };
}

#endif // DX11BUFFERBINDER_H
