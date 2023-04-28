#ifndef ENGINECONFIG_H
#define ENGINECONFIG_H

#include "Engine/Core/Render/Api/Context.h"
#include "Engine/Core/Render/Gui/GuiContext.h"
#include "Engine/Core/System/Input/Input.h"

namespace Engine {
    class EngineConfig {
    private:
        EngineConfig();
        virtual ~EngineConfig() = default;

        Context* m_context;
        GuiContext* m_devguiContext;
        Input* m_input;

    public:
        Context* GetContext() const;
        GuiContext* GetGuiContext() const;
        Input* GetInput() const;

        static EngineConfig& GetInstance();

        void Initialize(UInt64 winId);

    private:
        void InitializeContext(UInt64 winId);
        void InitializeInput();
    };
}

#endif // ENGINECONFIG_H
