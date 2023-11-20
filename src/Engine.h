#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "Engine/EngineDef.h"
#include "Engine/Core/System/Platform/Interface/IWindow.h"
#include "Engine/Rendering/Engine/Interface/IRenderPassContext.h"

#include "Engine/Core/System/Time/Time.h"
#include "Engine/Object/World/World.h"

class EngineClass {
public:
    EngineClass(Engine::IWindow* window);
    virtual ~EngineClass();

    void Run(Engine::World* world);

    void OnWorldStart(Engine::Entity* entity);
    void OnWorldUpdate(Engine::Entity* entity);

private:
    Engine::Time* m_time;
    Engine::IWindow* m_window;
    Engine::IRenderPassContext* m_context;
};

#endif // RENDERENGINE_H
