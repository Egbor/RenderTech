#ifndef DX11GUICONTEXT_H
#define DX11GUICONTEXT_H

#include "Engine/Core/Render/Gui/GuiContext.h"
#include "Engine/Core/Render/Api/DX11/DX11Context.h"

namespace Engine {
    class DX11GuiContext : public GuiContext {
    public:
        DX11GuiContext(HWND hWnd, DX11Context* dxContext);
        virtual ~DX11GuiContext() = default;
    };
}

#endif // DX11GUICONTEXT_H
