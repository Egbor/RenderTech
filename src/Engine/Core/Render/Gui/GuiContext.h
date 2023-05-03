#ifndef GUICONTEXT_H
#define GUICONTEXT_H

#include "Engine/Core/Render/Api/Context.h"

typedef void* GuiResourceId;

namespace Engine {
    class GuiContext {
    public:
        GuiContext() = default;
        virtual ~GuiContext() = default;

        virtual void PrepareLayout() = 0;
        virtual void Draw() = 0;

        virtual Context* GetContext() const = 0;

        virtual void AllocateGuiImageResource(Texture2D* texture, GuiResourceId* resourceId) = 0;
        virtual void FreeGuiResource(GuiResourceId* resourceId) = 0;

        virtual void Close() = 0;
    };
}

#endif // GUICONTEXT_H
