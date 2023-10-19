#include "Engine/Object/Global/EngineConfig.h"

#include "Engine/Core/Render/Api/DX11/DX11Context.h"
#include "Engine/Core/Render/Gui/DX11/DX11GuiContext.h"
#include "Engine/Core/System/Input/Windows/WinInput.h"

namespace Engine {
    EngineConfig::EngineConfig()
        : m_context(nullptr), m_devguiContext(nullptr),
        m_input(nullptr) {

    }

    IContext* EngineConfig::GetContext() const {
        return m_context;
    }

    GuiContext* EngineConfig::GetGuiContext() const {
        return m_devguiContext;
    }

    Input* EngineConfig::GetInput() const {
        return m_input;
    }

    void EngineConfig::Initialize(UInt64 winId) {
        InitializeContext(winId);
        InitializeInput();
    }

    EngineConfig& EngineConfig::GetInstance() {
        static EngineConfig config;
        return config;
    }

    void EngineConfig::InitializeContext(UInt64 winId) {
        m_context = new DX11Context();
        m_devguiContext = new DX11GuiContext(reinterpret_cast<HWND>(winId), dynamic_cast<DX11Context*>(m_context));
    }

    void EngineConfig::InitializeInput() {
        m_input = new WinInput();
    }
}
