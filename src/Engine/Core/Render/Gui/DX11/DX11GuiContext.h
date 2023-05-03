#ifndef DX11GUICONTEXT_H
#define DX11GUICONTEXT_H

#include "Engine/Core/Render/Gui/GuiContext.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11GuiContext : public GuiContext {
    private:
        HWND m_hWnd;
        DX11Context* m_dxContext;

    public:
        DX11GuiContext(HWND hWnd, DX11Context* dxContext);
        virtual ~DX11GuiContext();

        void PrepareLayout() override;
        void Draw() override;

        Context* GetContext() const override;

        void AllocateGuiImageResource(Texture2D* texture, GuiResourceId* resourceId) override;
        void FreeGuiResource(GuiResourceId* resourceId) override;

        void Close() override;
    };
}

#endif // DX11GUICONTEXT_H
