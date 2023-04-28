#ifndef DX11VIEWPORTBINDER_H
#define DX11VIEWPORTBINDER_H

#include "Engine/Core/Render/Api/RenderBinder.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11ViewportBinder : public RenderBinder<UInt32, RenderViewport> {
    private:
        DX11Context* m_dxContext;
        Array<RenderViewport*> m_viewports;

    public:
        DX11ViewportBinder(DX11Context* context);
        DX11ViewportBinder(const DX11ViewportBinder&) = default;
        virtual ~DX11ViewportBinder() = default;

        void Bind() override;
        void UnBind() override;

        void AttachRenderItem(UInt32 id, RenderViewport* item) override;
        void DetachRenderItem(UInt32 id) override;

    private:
        void AttachRenderItem(Int32 id, void* item) override;
        void DetachRenderItem(Int32 id) override;
    };
}

#endif // DX11VIEWPORTBINDER_H
