#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "EngineThread.h"
#include "Engine/EngineDef.h"

#include "Engine/Rendering/Engine/RenderPass/RenderPassContext.h"
#include "Engine/Core/System/Time/Time.h"
#include "Engine/Object/World/World.h"

namespace Engine {
    class EngineClass {
    public:
        EngineClass(IWindow* window, IContext* context);
        virtual ~EngineClass();

        void Run(World* world);

    private:
        //void OnWorldStart(Entity* entity);
        //void OnWorldUpdate(Entity* entity);

        void RunGameThread();
        void RunRenderThread();

        Time* m_time;
        RenderPassContext* m_passContext;

        EngineThread* m_gameThread;
        EngineThread* m_renderThread;
    };
}

#endif // RENDERENGINE_H
