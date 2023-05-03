#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "Engine/EngineDef.h"
#include <thread>


#include "Engine/Core/System/Time/Time.h"
#include "Engine/Core/System/Input/Input.h"

#include "Engine/Object/World/World.h"
#include "Engine/Object/World/WorldMode.h"

#include "Engine/Rendering/RenderSet/SceneRenderSet.h"
#include "Engine/Rendering/RenderSet/GuiRenderSet.h"

using namespace Engine;

class EngineClass {
private:
    SwapChain* m_swapchain;
    SceneRenderSet* m_rsScene;
    GuiRenderSet* m_rsGui;

    Time* m_time;
    World* m_world;
    WorldMode* m_mode;

public:
    EngineClass(UInt64 winId, UInt32 width, UInt32 height);
    virtual ~EngineClass();

    void InvokeStartStage();
    void InvokeUpdateStage();
    void InvokeRenderStage();

    Context* GetContext() const;
    GuiContext* GetGuiContext() const;

private:
    void InitializeWorld();
    void InitializeInput(Input* input);
};

#endif // RENDERENGINE_H
